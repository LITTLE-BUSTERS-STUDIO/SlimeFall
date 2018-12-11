#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "SceneManager.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "EntityManager.h"
#include "Brofiler/Brofiler.h"
#include "Coin.h"



Coin::Coin(fPoint pos, Entity_Info info) : Entity(pos, info)
{
	name.create("coin");

	// Add components ===================================================================
	Coin_Properties* coin_properties = (Coin_Properties *)info.properties;
	collider_rect = coin_properties->collider_rect;

	// Values ---------------------------------------------

	// Colliders ------------------------------------------
	
	collider = App->collision->AddCollider(collider_rect, COLLIDER_COIN, App->entity_manager);
	
	colliders.add(collider);

	// Textures ------------------------------------------
	tex_coin = coin_properties->tex_coin;
	
	// Animations ----------------------------------------
	coin_anim = coin_properties->anim_coin;

	// Sfx ----------------------------------------------
	fx_pick_up = coin_properties->pick_up_coin_fx;

}

j1Player::~j1Player()
{

}


bool j1Player::Update(float dt)
{
	BROFILER_CATEGORY("Coin Update", Profiler::Color::LightCyan);

	if (reset)
	{
		Reset(App->map->data.initial_position);
		reset = false;
	}

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
	BROFILER_CATEGORY("Coin Draw", Profiler::Color::LightGoldenRodYellow);

	SDL_Rect frame;
	SDL_Texture* texture = nullptr;
	jumping_anim.speed = 43.75F;
	frame = jumping_anim.GetLastFrame();
	texture = tex_player;

	App->render->Blit(texture, (int)position.x - frame.w / 2, (int)position.y - frame.h / 2, &frame, flip_x);

	return true;
}

bool j1Player::Reset(fPoint pos)
{
	BROFILER_CATEGORY("Player Reset", Profiler::Color::LightGray);

	position = pos;
	collider->SetPos(pos.x, pos.y);
	
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
	else if (state_string == "boucing")
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
		App->scene_manager->NextPhase();
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
