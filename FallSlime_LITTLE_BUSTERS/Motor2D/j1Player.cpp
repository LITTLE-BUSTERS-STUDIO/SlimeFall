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
	collider_rect.x = position.x;
	collider_rect.y = position.y;
	collider_rect.w = node.child("collider").attribute("width").as_uint(0);
	collider_rect.h = node.child("collider").attribute("height").as_uint(0);
	gravity = node.child("physics").attribute("gravity").as_float(0);
	speed_ground = node.child("physics").attribute("speed_ground").as_float(0);
	speed_air = node.child("physics").attribute("speed_air").as_float(0);
	speed_jump = node.child("physics").attribute("speed_jump").as_float(0);

	return true;
}

// Called before the first frame
bool j1Player::Start()
{
	// Add all components 
	collider = App->collision->AddCollider( collider_rect, COLLIDER_PLAYER, this);

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
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
	{
		if (on_ground)
			velocity.x = +speed_ground;
		else
			velocity.x = +speed_air;
	}
	else
		velocity.x = 0;

	// Only if player is on ground 
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && on_ground)
	{
		velocity.y = -speed_jump;
		on_ground = false;
	}

	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{

	if (on_ground == false)
	{
		acceleration.y = gravity;
	}

	velocity += acceleration;
	position += velocity;
	collider->SetPos(position.x - collider_rect.w/2 , position.y);

	return true;
}

// Called each loop iteration
bool j1Player::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
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
	switch (c2->type)
	{
	case COLLIDER_WALL:

		bool directions[(uint)Direction::max];
		directions[(uint)Direction::left] = velocity.x < 0;
		directions[(uint)Direction::right] = velocity.x > 0;
		directions[(uint)Direction::up] = velocity.y < 0;
		directions[(uint)Direction::down] = velocity.y > 0;

		uint distances[(uint)Direction::max];
		distances[(uint)Direction::left] = abs(  c2->rect.x + c2->rect.w - (position.x + c1->rect.w / 2)  );
		distances[(uint)Direction::right] = abs(   position.x + c1->rect.w / 2 - c2->rect.x   );
		distances[(uint)Direction::up] = (c2->rect.y + c2->rect.h) - (position.y - c1->rect.h);
		distances[(uint)Direction::down] = position.y - c2->rect.y;

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

		case Direction::down:
			position.y = c2->rect.y - collider->rect.h /2;
			velocity.y = 0;
			acceleration.y = 0;
			on_ground = true;
			break;
		case Direction::up:
			position.y = c2->rect.y + c2->rect.h + collider->rect.h / 2;
			velocity.y = 0;
			acceleration.y = 0;
			break;
		case Direction::left:
			position.x = c2->rect.y - collider->rect.w / 2;
			velocity.x = 0;
			break;
		case Direction::right:
			position.x = c2->rect.x - collider->rect.w / 2;
			velocity.x = 0;
			break;
		}

		collider->SetPos(position.x - collider->rect.w / 2, position.y - collider->rect.h/2);

		break;
	}
	return ret;
}
