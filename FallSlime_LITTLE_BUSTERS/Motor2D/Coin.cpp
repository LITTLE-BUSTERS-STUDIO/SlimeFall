#include "Enemy_Bat.h"
#include "j1App.h"
#include "EntityManager.h"
#include "j1Collision.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "Coin.h"
#include "j1Map.h"
#include "j1Render.h"
#include  "Hud.h"
#include "Brofiler/Brofiler.h"



Coin::Coin(fPoint position, fPoint spawn_pos, Properties *properties) :Entity(position, spawn_pos, properties)
{
	name.create("coin");

	Coin_Properties* coin_properties = (Coin_Properties *)properties;

	main_collider = App->collision->AddCollider(coin_properties->collider_rect, COLLIDER_NONE,  App->entity_manager);

	// Values ---------------------------------------------
	moving_pos = position;
	
	// Textures ------------------------------------------
	tex_coin = coin_properties->tex_coin;
	
	// Animations ----------------------------------------
	coin_anim = coin_properties->anim_coin;

	// Sfx ----------------------------------------------
	fx_pick_up = coin_properties->pick_up_coin_fx;


}

Coin::~Coin()
{
}

bool Coin::Update(float dt)
{
	BROFILER_CATEGORY("Coin Update", Profiler::Color::LightCyan);

	if (position.y > moving_pos.y - 5.0F && moving)
	{
		position.y -= 15.15F * dt;
		if (position.y <= moving_pos.y - 5.0F)
			moving = false;
	}
	else if (position.y < moving_pos.y + 5.0F && !moving)
	{
		position.y += 15.15F * dt;
		if (position.y >= moving_pos.y + 5.0F)
			moving = true;
	}

	if (main_collider)
	{
		main_collider->SetPos(position.x - main_collider->rect.w / 2, position.y - main_collider->rect.h / 2);
	}

	return true;
}

// Called each loop iteration

bool Coin::Draw()
{
	BROFILER_CATEGORY("Coin Draw", Profiler::Color::LightGoldenRodYellow);

	SDL_Rect frame = coin_anim.GetCurrentFrame();
	coin_anim.speed =7.0F;
	coin_anim.loop = true;

	if (coin_anim.GetFrameValue() > 6)
	{
		coin_anim.Reset();
	}

	App->render->Blit(tex_coin, (int)position.x - frame.w / 2, (int)position.y - frame.h / 2, &frame);

	return true;
}

bool Coin::Reset()
{
	BROFILER_CATEGORY("Coin Reset", Profiler::Color::LightGray);

	coin_anim.Reset();
	fx_pick_up = false;

	return true;
}

bool Coin::OnCollision(Collider* c1, Collider* c2)
{
	BROFILER_CATEGORY("Player OnCollision", Profiler::Color::LightGreen);

	bool ret = true;

	if (c1 == main_collider)
	{
		if (c2->type == COLLIDER_ATTACK || c2->type == COLLIDER_PLAYER)
		{
			if (!enable_coin)
			{
				App->audio->PlayFx(fx_pick_up);
				enable_coin = true;
				App->hud->AddCoin();
			}
			Desactive();
		}
	}

	return ret;
}

bool Coin::Active()
{
	if (main_collider)
	{
		main_collider->type = COLLIDER_COIN;
	}
	active = true;
	return true;
}

bool Coin::Desactive()
{
	if (main_collider)
	{
		main_collider->type = COLLIDER_NONE;
	}
	active = false;
	return true;
}

//Save and Load
bool Coin::Load(pugi::xml_node& node)
{
	bool ret = true;


	coin_counter = node.child("coin_counter").attribute("value").as_int(0);
	enable_coin = node.child("enable_coin").attribute("bool").as_bool(false);
	
	if (main_collider != nullptr)
	{
		return false;
	}

	p2SString collider_string(node.child("state").attribute("main_collider").as_string(""));

	if (collider_string == "collider_coin")
	{
		main_collider->type = COLLIDER_TYPE::COLLIDER_COIN;
	}
	else if (collider_string == "collider_none")
	{
		main_collider->type = COLLIDER_TYPE::COLLIDER_NONE;
	}

	return ret;
}

bool Coin::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node counter = node.append_child("coin_counter");

	counter.append_attribute("value") = coin_counter;

	pugi::xml_node conditions = node.append_child("enable_coin");

	conditions.append_attribute("bool") = coin_counter;
	
	pugi::xml_node state_node = node.append_child("state");

	p2SString collider_string;
	switch (main_collider->type)
	{
	case COLLIDER_TYPE::COLLIDER_COIN:
		collider_string.create("collider_coin");
		break;
	case COLLIDER_TYPE::COLLIDER_NONE:
		collider_string.create("collider_none");
		break;
	default:
		break;
	}

	state_node.append_attribute("main_collider") = collider_string.GetString();

	return ret;
}
