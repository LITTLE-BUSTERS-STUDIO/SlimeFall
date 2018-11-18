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
#include "Enemy_Test.h"
#include "j1Map.h"
#include <math.h>
#include "j1FadeToBlack.h"
#include "EntityManager.h"
#include "Brofiler/Brofiler.h"



j1Player::j1Player(fPoint pos, Entity_Info info) : Entity(pos, info)
{
	name.create("player");

	// Add components ===================================================================
	Player_Properties* player_properties = (Player_Properties *)info.properties;
	collider_rect = player_properties->collider_rect;

	// Values ---------------------------------------------
	gravity = player_properties->gravity;
	speed_air = player_properties->speed_air;
	speed_ground = player_properties->speed_ground;
	speed_jump = player_properties->speed_jump;
	speed_gummy_jump = player_properties->speed_gummy_jump;
	speed_attack = player_properties->speed_attack;

	// Colliders ------------------------------------------
	if (god_mode)
	{
		collider = App->collision->AddCollider(collider_rect, COLLIDER_GOD, App->entity_manager);
	}
	else
	{
		collider = App->collision->AddCollider(collider_rect, COLLIDER_PLAYER, App->entity_manager);
	}
	colliders.add(collider);

	// Textures ------------------------------------------
	tex_player = App->tex->Load(player_properties->path_tex_player.GetString());
	death_splash = App->tex->Load(player_properties->path_death_splash.GetString());
	attack_splash = App->tex->Load(player_properties->path_attack_splash.GetString());

	//tex_bat = App->tex->Load(player_properties->path_tex_bat.GetString());
	//tex_smoke = App->tex->Load(player_properties->path_tex_smoke.GetString());
	tex_skeleton = App->tex->Load(player_properties->path_tex_skeleton.GetString());

	// Animations ----------------------------------------
	jumping_anim = player_properties->jumping_anim;
	death_anim = player_properties->death_anim;
	attack_anim = player_properties->attack_anim;

	//bat_anim = player_properties->bat_anim;
	//smoke_anim = player_properties->smoke_anim;
	skeleton_attack_anim = player_properties->skeleton_attack_anim;
	skeleton_walking_anim = player_properties->skeleton_walking_anim;
	skeleton_dead_anim = player_properties->skeleton_dead_anim;

	// Sfx ----------------------------------------------
	id_death_fx = App->audio->LoadFx(player_properties->path_death_fx.GetString());
	fx_jump1 = App->audio->LoadFx(player_properties->path_jump_fx1.GetString());
	fx_jump2 = App->audio->LoadFx(player_properties->path_jump_fx2.GetString());
	fx_jump3 = App->audio->LoadFx(player_properties->path_jump_fx3.GetString());
	fx_jump4 = App->audio->LoadFx(player_properties->path_jump_fx4.GetString());
	fx_jump5 = App->audio->LoadFx(player_properties->path_jump_fx5.GetString());
	fx_attack = App->audio->LoadFx(player_properties->path_attack_fx.GetString());
}

j1Player::~j1Player()
{
	App->tex->UnLoad(tex_player);
	App->tex->UnLoad(death_splash);
	App->tex->UnLoad(attack_splash);

	//App->tex->UnLoad(tex_bat);
	//App->tex->UnLoad(tex_smoke);
	App->tex->UnLoad(tex_skeleton);
}

bool j1Player::HandleInput()
{
	BROFILER_CATEGORY("Player HandleInput", Profiler::Color::LightCoral);

	if (current_state != Player_State::dead)
	{
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			if (god_mode)
			{
				collider->type = COLLIDER_PLAYER;
				god_mode = false;
			}
			else
			{
				collider->type = COLLIDER_GOD;
				god_mode = true;
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

	//Only if player is dead
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && current_state == Player_State::dead)
	{
		reset = true;
		App->render->reset = true;
	}

	// Only if player is on ground 
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && current_state == Player_State::boucing && gummy_jump ==false)
	{
		gummy_jump = true;
		App->audio->PlayFx(fx_jump5);
	}

	//Only if player is jumping
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && current_state == Player_State::jumping && attack == false)
	{
		attack = true;
		current_state = Player_State::attack;
		collider->type = COLLIDER_ATTACK;

		App->audio->PlayFx(fx_attack);

	}
	//Physics applied

	if (apply_attack)
	{
		if(is_gummy_jumping && attack)
			velocity.y += ((float)attack * speed_attack) + speed_gummy_jump;

		else
			velocity.y += (float)attack * speed_attack;
		attack = false;
		apply_attack = false;		
	}

	if (apply_jump_speed)
	{
		velocity.y = -speed_jump - (float)gummy_jump * speed_gummy_jump;


		if (gummy_jump)
			is_gummy_jumping = true;

		on_ground = false;

		check_fall = false;
		apply_jump_speed = false;
		gummy_jump = false;
	}


	if (apply_invulnerability && Invulnerability(0.5F))
		apply_invulnerability = false;

	//Random Jump Fx
	if (on_ground && current_state == Player_State::jumping)
	{
		current_state = Player_State::boucing;

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

bool j1Player::Update(float dt)
{
	BROFILER_CATEGORY("Player Update", Profiler::Color::LightCyan);

	if (reset) 
	{
		Reset(App->map->data.initial_position);
		reset = false;
	}

	if (current_state != Player_State::jumping && current_state != Player_State::attack)
	{
		return true;
	}
	
	// God Mode movement =======================================
	if (god_mode)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			position.y -= speed_air * dt;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			position.y += speed_air *dt;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			position.x -= speed_air * dt;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			position.x += speed_air *dt;

		velocity.x = velocity.y = 0;
		return true;
	}
	
	// Normal movement =======================================+
	if (on_ground == false)
	{
		acceleration.y = gravity;
		check_fall = false;
	}
	else
	{
		acceleration.y = 0;
	}
	
	velocity += {acceleration.x *dt, acceleration.y *dt};
	position += {velocity.x *dt, velocity.y *dt};

	collider->SetPos(position.x - collider_rect.w / 2, position.y - collider_rect.h / 2);
	on_ground = false;

	p2List<Direction> directions;
	App->collision->CheckOverlap(directions, collider, COLLIDER_WALL, position, velocity);

	if (directions.count() > 0)
	{
		for (p2List_item<Direction> * item = directions.start; item; item = item->next)
		{
			switch (item->data)
			{
			case Direction::down:
				check_fall = true;
				on_ground = true;
				collider->type = COLLIDER_PLAYER;
				break;
			case Direction::up:
				velocity.y = 0;
				break;
			}
		}
		collider->SetPos(position.x - collider_rect.w / 2, position.y - collider_rect.h / 2);
	}
	return true;
}

// Called each loop iteration

bool j1Player::Draw()
{
	BROFILER_CATEGORY("Player Draw", Profiler::Color::LightGoldenRodYellow);

	SDL_Rect frame; 
	SDL_Texture* texture = nullptr;
	jumping_anim.speed = 43.75F;


	switch ((Player_State)current_state)
	{
	case Player_State::jumping:
		frame = jumping_anim.GetLastFrame();
		texture = tex_player;
		apply_attack = true;   

		break;

	case Player_State::boucing:
		if (jumping_anim.GetFrameValue() > 9)
		{
			current_state = Player_State::jumping;
			apply_jump_speed = true;
			is_gummy_jumping = false;
			frame = jumping_anim.GetCurrentFrame();
			jumping_anim.Reset();
			texture = tex_player;
			break;
		}
		frame = jumping_anim.GetCurrentFrame();
		texture = tex_player;
		break;

	case Player_State::dead:
		frame = death_anim.GetLastFrame();
		texture = death_splash;
		flip_x = false;
		if (god_mode)
		{
			collider->type = COLLIDER_PLAYER;
			god_mode = false;
		}
			
		if (!dead_fx)
		{
			App->audio->PlayFx(id_death_fx);
			dead_fx = true;
		}
		break;
	case Player_State::attack:
		frame = attack_anim.GetCurrentFrame();
		texture = attack_splash;
		
		if (on_ground)
			current_state = Player_State::jumping;
		break;
	default:
		break;
	}
	
	App->render->Blit(texture, (int)position.x - frame.w/2 , (int)position.y - frame.h / 2, &frame  , flip_x );

	
	skeleton_attack_anim.speed = skeleton_dead_anim.speed =skeleton_walking_anim.speed = 15.0F;

	if (skeleton_attack_anim.GetFrameValue() > 20)
		skeleton_attack_anim.Reset();
	if (skeleton_dead_anim.GetFrameValue() > 20)
		skeleton_dead_anim.Reset();
	if (skeleton_walking_anim.GetFrameValue() > 20)
		skeleton_walking_anim.Reset();

	App->render->Blit(tex_skeleton, 250, 140, &skeleton_attack_anim.GetCurrentFrame());
	App->render->Blit(tex_skeleton, 300, 140, &skeleton_dead_anim.GetCurrentFrame());
	App->render->Blit(tex_skeleton, 350, 140, &skeleton_walking_anim.GetCurrentFrame());
	return true;
}

bool j1Player::Reset( fPoint pos)
{
	BROFILER_CATEGORY("Player Reset", Profiler::Color::LightGray);

	position = pos;
	collider->SetPos(pos.x, pos.y);
	velocity.x = 0;
	velocity.y = 0;
	acceleration.x = 0;
	acceleration.y = 0;
	dead_fx = false;
	current_state = Player_State::jumping;
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
		current_state = Player_State::jumping;
	}
	else if  (state_string == "boucing")
	{
		current_state = Player_State::boucing;
	}
	else if (state_string == "dying")
	{
		current_state = Player_State::dead;
	}
	else if (state_string == "attack")
	{
		current_state = Player_State::attack;
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
	else if (collider_string == "collider_attack")
	{
		collider->type = COLLIDER_TYPE::COLLIDER_ATTACK;
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
	switch ((Player_State)current_state)
	{
	case Player_State::jumping:
		state_string.create("jumping");
		break;
	case Player_State::boucing:
		state_string.create("boucing");
		break;
	case Player_State::dead:
		state_string.create("dying");
		break;
	case Player_State::attack:
		state_string.create("attack");
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
	case COLLIDER_TYPE::COLLIDER_ATTACK:
		collider_string.create("collider_attack");
		break;
	default:
		break;
	}

	state_node.append_attribute("collider_type") = collider_string.GetString();

	return ret;
}

// Remove Colliders Overlap
bool j1Player::OnCollision(Collider* c1, Collider* c2)
{
	BROFILER_CATEGORY("Player OnCollision", Profiler::Color::LightGreen);

	bool ret = true;

	switch (c2->type)
	{
	case COLLIDER_DEATH:
		current_state = Player_State::dead;
		collider->type = COLLIDER_NONE;
		break;
	case COLLIDER_NEXT_LEVEL:
		App->current_level->NextPhase();
		break;
	case COLLIDER_ENEMY:

		if (current_state == Player_State::attack)
		{
			on_ground = true;
			apply_invulnerability = true;
			attack_tremble = true;
			break;
		}
		
		current_state = Player_State::dead;
		collider->type = COLLIDER_NONE;
		break;
	}
	return ret;
}

bool j1Player::Invulnerability(float time)
{
	// Reset
	if (apply_timer)
		timer_invulnerability.Start();
	
	// Timer
	if (timer_invulnerability.ReadSec() > time)
	{
		collider->type = COLLIDER_PLAYER;
		apply_timer = true;
		return true;
	}
	else
		collider->type = COLLIDER_GOD;
	
	return apply_timer = false;
}