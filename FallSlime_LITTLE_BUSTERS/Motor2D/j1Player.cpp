#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Map.h"
#include <math.h>
#include "j1FadeToBlack.h"

j1Player::j1Player() 
{
	name.create("player");
}


j1Player::~j1Player()
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
	speed_attack = node.child("physics").attribute("speed_attack").as_float(0);
	god_mode = node.child("debug").attribute("god_mode").as_bool(false);

	//Assets=======================================
	//----------Textures---------------------------
	path_tex_player.create(node.child("textures").child("jumping").attribute("path").as_string(""));
	path_death_splash.create(node.child("textures").child("death").attribute("path").as_string(""));
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
	path_jump_fx1.create(node.child("sfx").child("jump1").attribute("path").as_string(""));
	path_jump_fx2.create(node.child("sfx").child("jump2").attribute("path").as_string(""));
	path_jump_fx3.create(node.child("sfx").child("jump3").attribute("path").as_string(""));
	path_jump_fx4.create(node.child("sfx").child("jump4").attribute("path").as_string(""));
	path_jump_fx5.create(node.child("sfx").child("jump5").attribute("path").as_string(""));
	path_death_sfx.create(node.child("sfx").child("death").attribute("path").as_string(""));
	//=============================================

	return true;
}

// Called before the first frame
bool j1Player::Start()
{
	// Add all components 
	ground_detector = App->collision->AddCollider(rect_collider, COLLIDER_PLAYER, this);

	if (god_mode)
		collider = App->collision->AddCollider(rect_collider, COLLIDER_GOD, this);
	else
		collider = App->collision->AddCollider(rect_collider, COLLIDER_PLAYER, this);
	
	tex_player = App->tex->Load(path_tex_player.GetString());
	death_splash = App->tex->Load(path_death_splash.GetString());

	id_death_sfx = App->audio->LoadFx(path_death_sfx.GetString());
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
	
	if (current_state != State::dead)
	{
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			if (god_mode)
			{
				god_mode = false;
				collider->type = COLLIDER_PLAYER;
			}
			else
			{
				god_mode = true;
				collider->type = COLLIDER_GOD;
			}
		}
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
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		App->fade_to_black->FadeToBlack(0.5F);
	//Only if player is dead
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && current_state == State::dead)
	{
		reset = true;
		App->render->reset = true;
	}

	// Only if player is on ground 
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && current_state == State::boucing && gummy_jump ==false)
	{
		gummy_jump = true;
		App->audio->PlayFx(fx_jump5);
	}

	//Only if player is jumping
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && current_state == State::jumping && attack == false)
	{
		attack = true;
		//Add sfx attack
		//Little tremble
		//if kill enemy, auto gummyjump
		//Floor dust is trembled
	}

	if (apply_jump_speed)
	{
		velocity.y = -speed_jump - gummy_jump * speed_gummy_jump;
		on_ground = false;
		check_fall = false;
		apply_jump_speed = false;
		gummy_jump = false;
		
	}
	if (apply_attack)
	{
		velocity.y += attack * speed_attack;
		attack = false;
		apply_attack = false;
	}

	if (on_ground && current_state == State::jumping)
	{
		current_state = State::boucing;

		uint random_jump = rand() % 4 + 1;
		
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
	}

	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	if (reset) 
	{
		Reset(App->map->data.initial_position);
		reset = false;
	}

	if (current_state != State::jumping)
		return true;

	//Basic God Mode movement 
	if (god_mode)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			position.y -= speed_air;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			position.y += speed_air;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			position.x -= speed_air;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			position.x += speed_air;

		velocity.x = velocity.y = 0;
		return true;
	}

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
	SDL_Texture* texture = nullptr;
	jumping_anim.speed = 0.7f;

	switch ((State)current_state)
	{
	case State::jumping:
		frame = jumping_anim.GetLastFrame();
		texture = tex_player;
		apply_attack = true;
		break;
	case State::boucing:
		if (jumping_anim.GetFrameNumber() > 9)
		{
			current_state = State::jumping;
			apply_jump_speed = true;
			jumping_anim.Reset();
			frame = jumping_anim.GetCurrentFrame();
			texture = tex_player;
			break;
		}
		frame = jumping_anim.GetCurrentFrame();
		texture = tex_player;
		break;
	case State::dead:
		frame = death_anim.GetLastFrame();
		texture = death_splash;
		flip_x = false;
		break;
	default:
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

bool j1Player::Reset( fPoint pos)
{
	position = pos;
	velocity.x = 0;
	velocity.y = 0;
	acceleration.x = 0;
	acceleration.y = 0;
	current_state = State::jumping;
	jumping_anim.Reset();

	if (collider != nullptr && collider->type != COLLIDER_GOD)
	{
		collider->type = COLLIDER_PLAYER;
	}
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
		current_state = State::dead;
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
	case State::dead:
		state_string.create("dying");
		break;
	default:
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
			App->audio->PlayFx(id_death_sfx);
			current_state = State::dead;
			collider->type = COLLIDER_NONE;
			break;
		case COLLIDER_NEXT_LEVEL:
			App->current_level->NextPhase();
			App->fade_to_black->FadeToBlack(0.5f);
		
			break;
		}
	}
	
	if (c1 == ground_detector && check_fall && collider->type != COLLIDER_NONE) {
		on_ground = true;
	}

	return ret;
}
