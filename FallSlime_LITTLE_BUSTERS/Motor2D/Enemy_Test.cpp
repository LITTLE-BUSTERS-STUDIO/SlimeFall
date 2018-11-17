#include "Enemy_Test.h"
#include "j1App.h"
#include "EntityManager.h"
#include "j1Collision.h"

#include "j1Map.h"
#include "j1Render.h"

#include "Brofiler/Brofiler.h"

Enemy_Bat::Enemy_Bat(fPoint position, Entity_Info info) :Enemy( position, info)
{
	//path_interval_time = 500;
//detection_ratio = 20;
	target = (Entity*) App->entity_manager->GetPlayer();
	/*path_timer.Start();*/
}

bool Enemy_Bat::Update(float dt)
{
	CheckTargetRatio();
	UpdateLogic();
	FollowPath(dt);

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