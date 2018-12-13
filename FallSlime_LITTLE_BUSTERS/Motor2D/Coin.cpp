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

#include "Brofiler/Brofiler.h"



Coin::Coin(fPoint position, fPoint spawn_pos, Properties *properties) :Entity(position, spawn_pos, properties)
{
	Coin_Properties* coin_properties = (Coin_Properties *)properties;

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

	SDL_Rect frame;
	SDL_Texture* texture = nullptr;
	coin_anim.speed = 7;

	switch ((Coin_States)current_state)
	{
	case Coin_States::enable:
		//main_collider->type = COLLIDER_COIN;
		if (coin_anim.GetFrameValue() > 6)
		{
			frame = coin_anim.GetCurrentFrame();
			texture = tex_coin;
			coin_anim.Reset();
		}

		frame = coin_anim.GetCurrentFrame();
		texture = tex_coin;
		break;

	case Coin_States::disable:
		main_collider->type = COLLIDER_NONE;
		
		if (!enable_fx)
		{
			App->audio->PlayFx(fx_pick_up);
			enable_fx = true;
		}
		break;

	default:
		break;
	}


	if (position.y > moving_pos.y - 5 && moving)
	{
		position.y-=0.5F;
		if (position.y == moving_pos.y - 5)
			moving = false;
	}
	else if (position.y < moving_pos.y + 5 && !moving)
	{
		position.y+=0.5F;
		if (position.y == moving_pos.y + 5)
			moving = true;
	}
	

	App->render->Blit(texture, (int)position.x - frame.w / 2, (int)position.y - frame.h / 2, &frame);

	return true;
}

bool Coin::Reset()
{
	BROFILER_CATEGORY("Coin Reset", Profiler::Color::LightGray);

	coin_anim.Reset();
	fx_pick_up = false;

	return true;
}

// Remove Colliders Overlap
bool Coin::OnCollision(Collider* c1, Collider* c2)
{
	BROFILER_CATEGORY("Player OnCollision", Profiler::Color::LightGreen);

	bool ret = true;

	if (c1 == main_collider)
	{
		switch (c2->type)
		{
		case COLLIDER_ATTACK:
			current_state = Coin_States::disable;
			coin_counter++;
			break;
		case COLLIDER_PLAYER:
			current_state = Coin_States::disable;
			coin_counter++;
			break;
		default:
			break;
		}
	}

	return ret;
}
