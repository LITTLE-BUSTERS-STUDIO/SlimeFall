#include "Enemy_Test.h"
#include "j1App.h"
#include "EntityManager.h"
#include "j1Collision.h"
#include "j1Textures.h"

#include "j1Map.h"
#include "j1Render.h"

#include "Brofiler/Brofiler.h"


Enemy_Bat::Enemy_Bat(fPoint position, Entity_Info info) :Enemy( position, info)
{
	//path_interval_time = 500;
//detection_ratio = 20;
	target = (Entity*) App->entity_manager->GetPlayer();
	/*path_timer.Start();*/




	Enemy_Properties* enemy_properties = (Enemy_Properties *)info.properties;

	// Textures ------------------------------------------
	tex_smoke = App->tex->Load(enemy_properties->path_tex_smoke.GetString());
	tex_bat = App->tex->Load(enemy_properties->path_tex_bat.GetString());

	// Animations ----------------------------------------
	smoke_anim = enemy_properties->smoke_anim;
	bat_anim = enemy_properties->bat_anim;

}

Enemy_Bat::~Enemy_Bat()
{
	App->tex->UnLoad(tex_smoke);
	App->tex->UnLoad(tex_bat);

}

bool Enemy_Bat::Update(float dt)
{

	velocity = { 100,100 };

	if (CheckTargetRatio())
	{
		UpdateLogic();
		FollowPath(dt);
	}


	if (main_collider)
	{
		main_collider->SetPos(position.x - main_collider->rect.w / 2, position.y - main_collider->rect.h / 2);
	}

	return true;
}

bool Enemy_Bat::Draw()
{
	for (uint i = 0; i < last_path.Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(last_path.At(i)->x, last_path.At(i)->y);
		App->render->DrawQuad({ pos.x, pos.y ,16,16 }, 0, 0, 0, 200);

	}

	SDL_Rect frame;
	SDL_Texture* texture = nullptr;
	smoke_anim.speed = 15.0F;
	bat_anim.speed = 10.0F;

	if (smoke_anim.GetFrameValue() > 9)
		smoke_anim.Reset();

	if (bat_anim.GetFrameValue() > 9)
		bat_anim.Reset();

	App->render->Blit(tex_smoke, 200, 140, &smoke_anim.GetCurrentFrame());
	App->render->Blit(tex_bat, 100, 140, &bat_anim.GetCurrentFrame());
	return true;
}

bool Enemy_Bat::Reset(fPoint pos)
{
	BROFILER_CATEGORY("Enemy_Bat Reset", Profiler::Color::LightGray);

	position = pos;
	velocity.x = 0;
	velocity.y = 0;
	acceleration.x = 0;
	acceleration.y = 0;
	bat_anim.Reset();


	return true;
}



bool Enemy_Bat::OnCollision(Collider* c1, Collider* c2)
{
	if (main_collider == c1 && c2->type == COLLIDER_TYPE::COLLIDER_WALL) 
	{
      	App->collision->ResolveOverlap(c1, c2, position, velocity);
	}
	return true;
}