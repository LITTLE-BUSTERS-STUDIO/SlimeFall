#include "Enemy_Test.h"
#include "j1Collision.h"
#include "EntityManager.h"

Enemy_Bat::Enemy_Bat(fPoint position, Entity_Info info) :Enemy( position, info)
{
	//path_interval_time = 500;
	//detection_ratio = 20;
	target = (Entity*)App->entity_manager->GetPlayer();
	/*path_timer.Start();*/
}

bool Enemy_Bat::Update(float dt)
{
	CheckTargetRatio();
	UpdateLogic();
	FollowPath(dt);

	if (main_collider) 
	{
		main_collider->SetPos(position.x - main_collider->rect.w/2, position.y - main_collider->rect.h / 2);
	}
	
	return true;
}

bool Enemy_Bat::Draw(SDL_Texture* sprites)
{
	return true;
}

bool Enemy_Bat::OnCollision(Collider* c1, Collider* c2)
{
	return true;
}