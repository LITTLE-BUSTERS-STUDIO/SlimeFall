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
	name.create("coin");

	Coin_Properties* coin_properties = (Coin_Properties *)properties;

	main_collider = App->collision->AddCollider(coin_properties->collider_rect, COLLIDER_NONE,  App->entity_manager);
	colliders.add(main_collider);

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
		if (c2->type==COLLIDER_ATTACK || c2->type == COLLIDER_PLAYER)
		{
			if (!enable_fx)
			{
				App->audio->PlayFx(fx_pick_up);
				enable_fx = true;
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
		main_collider->type = COLLIDER_COIN;
	}
	active = false;
	return true;
}
