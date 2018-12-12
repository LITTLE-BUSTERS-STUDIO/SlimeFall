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
#include "j1Map.h"



Coin::Coin(fPoint pos, Entity_Info info) : Entity(pos, info)
{
	name.create("coin");

	// Add components ===================================================================
	Coin_Properties* coin_properties = (Coin_Properties *)info.properties;
	collider_rect = coin_properties->collider_rect;

	// Values ---------------------------------------------

	// Colliders ------------------------------------------
	
	/*collider = App->collision->AddCollider(collider_rect, COLLIDER_COIN, App->entity_manager);
	
	colliders.add(collider);
*/
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

	/*p2List<Direction> directions;
	App->collision->CheckOverlap(directions, collider, COLLIDER_WALL, position, velocity);
	collider->SetPos(position.x - collider_rect.w / 2, position.y - collider_rect.h / 2);*/
	
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

	App->render->Blit(texture, 0, 0, &frame);

	return true;
}


// Remove Colliders Overlap
bool Coin::OnCollision(Collider* c1, Collider* c2)
{
	BROFILER_CATEGORY("Player OnCollision", Profiler::Color::LightGreen);

	bool ret = true;

	return ret;
}
