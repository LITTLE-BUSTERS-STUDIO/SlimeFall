#include "Enemy.h"
#include "EntityManager.h"
#include "J1Player.h"
#include "j1PathFinding.h"

#define MAX_DETECTION_RATIO 50

Enemy::Enemy(fPoint position, Entity_Info info) :Entity(position, info)
{

	path_interval_time = 0u;
	path_timer;

	if (path_interval_time) 
	{
		path_timer.Start();
	}
}

bool Enemy::UpdateLogic() 
{
	if (path_timer.Read() > path_interval_time &&  path_interval_time && target) 
	{
		App->path_finding->CreatePath(
			iPoint(position.x, position.y),
			iPoint (target->position.x , target->position.y) 
		);
		last_path = *App->path_finding->GetLastPath();
		path_timer.Start();
	}
	return true;
}

bool  Enemy::FollowPath( )
{
	if (last_path.Count())
	{

	}

	return true;
}

bool Enemy::CheckTargetRatio()
{
	bool ret = false;

	if (detection_ratio < MAX_DETECTION_RATIO && target)
	{
		if (position.DistanceManhattan(App->entity_manager->GetPlayer()->position) < detection_ratio)
		{
			ret = true;
		}
	}
	return ret;
}