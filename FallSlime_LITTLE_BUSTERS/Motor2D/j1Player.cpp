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
	//Values=======================================
	rect_collider.w = node.child("collider").attribute("width").as_uint(0);
	rect_collider.h = node.child("collider").attribute("height").as_uint(0);
	gravity = node.child("physics").attribute("gravity").as_float(0);
	speed_ground = node.child("physics").attribute("speed_ground").as_float(0);
	speed_air = node.child("physics").attribute("speed_air").as_float(0);
	speed_jump = node.child("physics").attribute("speed_jump").as_float(0);
	speed_gummy_jump = node.child("physics").attribute("speed_gummy_jump").as_float(0);

	//Assets=======================================
	//----------Textures---------------------------
	path_tex_player.create(node.child("texture").attribute("path").as_string(""));
	path_death_splash.create(node.child("texture_death").attribute("path").as_string(""));
	//----------Animations-------------------------
	pugi::xml_document jumping_an_doc;
	pugi::xml_node jumping_an_node;
	jumping_an_doc.load_file("animations/player_animation.xml");
	jumping_an_node = jumping_an_doc.child("tileset");
	jumping_anim.LoadAnimation(jumping_an_node, "pink_slime");

	pugi::xml_document death_an_doc;
	pugi::xml_node death_an_node;
	death_an_doc.load_file("animations/player_death_animation.xml");
	death_an_node = death_an_doc.child("tileset");
	death_anim.LoadAnimation(death_an_node, "pink_splash");

	//----------SFX--------------------------------
	path_jump_fx1.create(node.child("jump_fx").child("jump1").attribute("path").as_string(""));
	path_jump_fx2.create(node.child("jump_fx").child("jump2").attribute("path").as_string(""));
	path_jump_fx3.create(node.child("jump_fx").child("jump3").attribute("path").as_string(""));
	path_jump_fx4.create(node.child("jump_fx").child("jump4").attribute("path").as_string(""));
	path_jump_fx5.create(node.child("jump_fx").child("jump5").attribute("path").as_string(""));
	//=============================================

	return true;
}

// Called before the first frame
bool j1Player::Start()
{
	// Add all components 

	collider = App->collision->AddCollider( rect_collider, COLLIDER_PLAYER, this);
	ground_detector = App->collision->AddCollider(rect_collider, COLLIDER_PLAYER, this);
	
	tex_player = App->tex->Load(path_tex_player.GetString());
	death_splash = App->tex->Load(path_death_splash.GetString());

	fx_jump1 = App->audio->LoadFx(path_jump_fx1.GetString());
	fx_jump2 = App->audio->LoadFx(path_jump_fx2.GetString());
	fx_jump3 = App->audio->LoadFx(path_jump_fx3.GetString());
	fx_jump4 = App->audio->LoadFx(path_jump_fx4.GetString());
	fx_jump5 = App->audio->LoadFx(path_jump_fx5.GetString());

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

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && current_state == State::boucing && gummy_jump ==false)
	{
		gummy_jump = true;
		App->audio->PlayFx(fx_jump5);
	}



	if (apply_jump_speed == true)
	{
		velocity.y = -speed_jump - gummy_jump* speed_gummy_jump;
		on_ground = false;
		check_fall = false;
		apply_jump_speed = false;
		gummy_jump = false;
	}


	if (on_ground && current_state == State::jumping)
	{
		current_state = State::boucing;

		uint random_jump = rand() % 4 + 1;
		uint random_secret = rand() % 99;
		
		switch (random_jump)
		{
		case 1:
			App->audio->PlayFx(fx_jump1);
			break;
		case 2:
			App->audio->PlayFx(fx_jump2);
			break;
		case 3:
			App->audio->PlayFx(fx_jump3);
			break;
		case 4:
			App->audio->PlayFx(fx_jump4);
			break;

		default:
			App->audio->PlayFx(fx_jump1);
			break;
		}
		//switch (random_secret)
		//{
		//case 0://Sorprise
		//	App->audio->PlayFx(fx_jump5);
		//default:

		//	break;
		//}
	}


	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	if (current_state == State::boucing || current_state == State::dying) //================================================================
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
	SDL_Texture* texture;
	jumping_anim.speed = 0.7f;

	switch ((State)current_state)
	{
	case State::jumping:
		frame = jumping_anim.GetLastFrame();
		texture = tex_player;
		break;
	case State::boucing:
		if (jumping_anim.GetFrameNumber() > 9)
		{
			current_state = State::jumping;
			apply_jump_speed = true;
			jumping_anim.Reset();
			frame = jumping_anim.GetCurrentFrame();
			break;
		}
		frame = jumping_anim.GetCurrentFrame();
		texture = tex_player;
		break;
	case State::dying:
		frame = death_anim.GetLastFrame();
		texture = death_splash;
		break;
	}

	App->render->Blit(texture, position.x - frame.w/2 , position.y - frame.h / 2, &frame  , flip_x );
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(tex_player);
	tex_player = nullptr;
	//App->audio->UnLoadFx(test_fx);

	return true;
}

//Save and Load
bool j1Player::Load(pugi::xml_node& node)
{
	bool ret = true;
	position.x = node.child("position").attribute("x").as_float(0);
	position.y = node.child("position").attribute("y").as_float(0);
	velocity.x = node.child("velocity").attribute("x").as_float(0);
	velocity.y = node.child("velocity").attribute("y").as_float(0);
	acceleration.x = node.child("acceleration").attribute("x").as_float(0);
	acceleration.y = node.child("acceleration").attribute("y").as_float(0);

	apply_jump_speed = node.child("conditions").attribute("apply_jump_speed").as_bool(false);
	on_ground = node.child("conditions").attribute("on_ground").as_bool(false);
	check_fall = node.child("conditions").attribute("check_fall").as_bool(false);
	flip_x = node.child("conditions").attribute("flip_x").as_bool(false);
	gummy_jump = node.child("conditions").attribute("gummy_jump").as_bool(false);

	p2SString state_string(node.child("state").attribute("current_state").as_string(""));

	if (state_string == "jumping")
	{
		current_state = State::jumping;
	}
	else if  (state_string == "boucing")
	{
		current_state = State::boucing;
	}
	else if (state_string == "dying")
	{
		current_state = State::dying;
	}

	p2SString collider_string(node.child("state").attribute("collider_type").as_string(""));

	if (collider_string == "collider_player")
	{
		collider->type = COLLIDER_TYPE::COLLIDER_PLAYER;
	}
	else if (collider_string == "collider_none")
	{
		collider->type = COLLIDER_TYPE::COLLIDER_NONE;
	}
	else if (collider_string == "collider_god")
	{
		collider->type = COLLIDER_TYPE::COLLIDER_GOD;
	}

	return ret;
}

bool j1Player::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node pos = node.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	pugi::xml_node vel = node.append_child("velocity");

	vel.append_attribute("x") = velocity.x;
	vel.append_attribute("y") = velocity.y;

	pugi::xml_node conditions = node.append_child("conditions");

	conditions.append_attribute("apply_jump_speed") = apply_jump_speed;
	conditions.append_attribute("on_ground") = on_ground;
	conditions.append_attribute("check_fall") = check_fall;
	conditions.append_attribute("flip_x") = flip_x;
	conditions.append_attribute("gummy_jump") = gummy_jump;

	pugi::xml_node state_node = node.append_child("state");

	p2SString state_string;
	switch ((State)current_state)
	{
	case State::jumping:
		state_string.create("jumping");
		break;
	case State::boucing:
		state_string.create("boucing");
		break;
	case State::dying:
		state_string.create("dying");
		break;
	}
	state_node.append_attribute("current_state") = state_string.GetString();

	p2SString collider_string;
	switch (collider->type)
	{
	case COLLIDER_TYPE::COLLIDER_PLAYER:
		collider_string.create("collider_player");
		break;
	case COLLIDER_TYPE::COLLIDER_NONE:
		collider_string.create("collider_none");
		break;
	case COLLIDER_TYPE::COLLIDER_GOD:
		collider_string.create("collider_god");
		break;
	}

	state_node.append_attribute("collider_type") = collider_string.GetString();





	return ret;
}

// Remove Colliders Overlap
bool j1Player::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = true;

	// Switch all collider types
	if (c1 == collider)
	{
		int offset_direction;

		switch (c2->type)
		{
		case COLLIDER_GOD:
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

			offset_direction = -1;

			for (uint i = 0; i < (uint)Direction::max; ++i)
			{
				if (directions[i]) {

					if (offset_direction == -1)
						offset_direction = i;
					else if (distances[i] < distances[(uint)offset_direction])
						offset_direction = i;
				}
			}

			switch ((Direction)offset_direction)
			{
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

		case COLLIDER_DEATH:
			current_state = State::dying;
			collider->type = COLLIDER_NONE;
			//Death Sfx
			break;
		}
	}
	
	if (c1 == ground_detector && check_fall) {
		on_ground = true;
	}

	return ret;
}
