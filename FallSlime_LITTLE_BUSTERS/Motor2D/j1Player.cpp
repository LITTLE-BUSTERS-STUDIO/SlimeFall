#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "j1Player.h"
#include <math.h>

j1Player::j1Player() 
{
	name.create("player");
	position.x = 130 ;
	position.y = 60;
	velocity.x = 0;
	velocity.y = 0;
	acceleration.x = 0;
	acceleration.y = 0;

}

j1Player::~j1Player()
{

}

void j1Player::Init()
{
	
}

// Called before render is available
bool  j1Player::Awake(pugi::xml_node& node )
{


	//Values
	rect_collider.w = node.child("collider").attribute("width").as_uint(0);
	rect_collider.h = node.child("collider").attribute("height").as_uint(0);
	gravity = node.child("physics").attribute("gravity").as_float(0);
	speed_ground = node.child("physics").attribute("speed_ground").as_float(0);
	speed_air = node.child("physics").attribute("speed_air").as_float(0);
	speed_jump = node.child("physics").attribute("speed_jump").as_float(0);


	//Assets
	pugi::xml_document animation_doc;
	pugi::xml_node anim_node;
	animation_doc.load_file("player_animation.xml");
	anim_node = animation_doc.child("tileset");
	path_tex_player.create(node.child("texture").attribute("path").as_string(""));
	idle.LoadAnimation(anim_node, "blue_slime");

	return true;
}

// Called before the first frame
bool j1Player::Start()
{
	// Add all components 

	collider = App->collision->AddCollider( rect_collider, COLLIDER_PLAYER, this);
	ground_detector = App->collision->AddCollider(rect_collider, COLLIDER_PLAYER, this);
	tex_player = App->tex->Load(path_tex_player.GetString());

	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		if (on_ground)
			velocity.x = -speed_ground;
	
		else
			velocity.x = -speed_air;
		
		flip_x = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
	{
		if (on_ground)
			velocity.x = +speed_ground;
		else
			velocity.x = +speed_air;
		flip_x = false;
	}
	else
		velocity.x = 0;

	// Only if player is on ground 
	if (apply_jump_speed == true)
	{
		velocity.y = -speed_jump;
		on_ground = false;
		check_fall = false;
		apply_jump_speed = false;
	}


	if (on_ground && current_state == State::jumping)
	{
		current_state = State::boucing;
	}


	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	if (current_state == State::boucing) //================================================================
		return true;

	if (on_ground == false)
	{
		
		acceleration.y = gravity;
		check_fall = false;
	}
	else
	{
		acceleration.y = 0;
	}
		
	velocity += acceleration;
	position += velocity;
	collider->SetPos(position.x - rect_collider.w / 2, position.y - rect_collider.h / 2);
	ground_detector->SetPos(position.x - rect_collider.w / 2, position.y);
	on_ground = false;
	return true;
}

// Called each loop iteration
bool j1Player::PostUpdate()
{
	SDL_Rect frame; 
	idle.speed = 0.5f;

	switch ((State)current_state)
	{
	case State::jumping:
		frame = idle.GetLastFrame();
		break;
	case State::boucing:
		if (idle.GetFrameNumber() > 9)
		{
			current_state = State::jumping;
			apply_jump_speed = true;
			idle.Reset();
			frame = idle.GetCurrentFrame();
			break;
		}
		frame = idle.GetCurrentFrame();
		break;
	}
	App->render->Blit(tex_player, position.x - frame.w/2 , position.y - frame.h / 2, &frame  , flip_x );
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(tex_player);
	tex_player = nullptr;
	return true;
}

//Save and Load
bool j1Player::Load(pugi::xml_node& node)
{
	bool ret = true;
	return ret;
}

bool j1Player::Save(pugi::xml_node& node) const
{
	bool ret = true;
	return ret;
}

// Remove Colliders Overlap
bool j1Player::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = true;

	// Switch all collider types
	if (c1 == collider)
	{
		switch (c2->type)
		{
		case COLLIDER_WALL:

			SDL_Rect player = c1->rect;
			SDL_Rect coll = c2->rect;

			bool directions[(uint)Direction::max];
			directions[(uint)Direction::left] = velocity.x < 0;
			directions[(uint)Direction::right] = velocity.x > 0;
			directions[(uint)Direction::up] = velocity.y < 0;
			directions[(uint)Direction::down] = velocity.y > 0;

			uint distances[(uint)Direction::max];
			distances[(uint)Direction::right] = player.x + player.w - coll.x;
			distances[(uint)Direction::left] = coll.x + coll.w - player.x;
			distances[(uint)Direction::up] = coll.y + coll.h - player.y;
			distances[(uint)Direction::down] = player.y + player.h - coll.y;

			int offset_direction = -1;

			for (uint i = 0; i < (uint)Direction::max; ++i)
			{
				if (directions[i]) {

					if (offset_direction == -1)
						offset_direction = i;
					else if (distances[i] < distances[(uint)offset_direction])
						offset_direction = i;
				}
			}

			switch ((Direction)offset_direction) {

			case Direction::right:
				position.x = coll.x - player.w / 2;
				velocity.x = 0;
				break;
			case Direction::left:
				position.x = coll.x + coll.w + player.w / 2;
				velocity.x = 0;
				break;
			case Direction::up:
				position.y = coll.y + coll.h + player.h / 2;
				velocity.y = 0;
				break;
			case Direction::down:
				position.y = coll.y - player.h / 2;
				velocity.y = 0;
				check_fall = true;
				on_ground = true;
				break;
			}
			collider->SetPos(position.x - collider->rect.w / 2, position.y - collider->rect.h / 2);
			ground_detector->SetPos(position.x - collider->rect.w / 2, position.y );
			break;
		}
	}
	
	if (c1 == ground_detector && check_fall) {
		on_ground = true;
	}

	return ret;
}
