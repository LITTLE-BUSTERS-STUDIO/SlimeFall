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



Coin::Coin(fPoint pos, Entity_Info info) :Enemy(position, info)
{
	Coin_Properties* coin_properties = (Coin_Properties *)info.properties;

	// Values ---------------------------------------------

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
	frame = coin_anim.GetLastFrame();
	texture = tex_coin;
	if (coin_anim.GetFrameValue() > 9)
	{
		frame = coin_anim.GetCurrentFrame();
		texture = tex_coin;
		coin_anim.Reset();
	}
	App->render->Blit(texture, (int)position.x - frame.w / 2, (int)position.y - frame.h / 2, &frame);

	return true;
}

bool Coin::Reset(Entity_Info  info)
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
			break;
		default:
			break;
		}
	}

	return ret;
}
