#include "Enemy.h"
#include "EntityManager.h"
#include "J1Player.h"
#include "j1PathFinding.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "p2Log.h"

#define MAX_DETECTION_RATIO 400

Enemy::Enemy(fPoint position, Entity_Info info) :Entity(position, info)
{

	main_collider = App->collision->AddCollider(info.properties->collider_rect, COLLIDER_ENEMY, App->entity_manager);
	colliders.add(main_collider);


	iPoint pos = { (int)position.x, (int)position.y};
	pos = App->map->WorldToMap(pos.x, pos.y);
	current_point = {pos.x, pos.y};
	
	//path_interval_time = 0u;
	velocity = { 100, 100 };
	path_interval_time = 200u;
	detection_ratio = 300;

	if (path_interval_time) 
	{
		path_timer.Start();
	}
}

bool Enemy::UpdateLogic() 
{
	if (  path_timer.Read() > path_interval_time  && target_detected ) 
	{
		iPoint map_target(target->position.x, target->position.y);
		iPoint map_pos(position.x, position.y);

		map_target = App->map->WorldToMap(map_target.x, map_target.y);
		map_pos = App->map->WorldToMap(map_pos.x, map_pos.y);

		App->path_finding->DeleteLastPath();
		App->path_finding->CreatePath( map_pos, map_target, last_path);

		path_timer.Start();
	}
	return true;
}

bool  Enemy::FollowPath( float dt)
{
	if (!last_path.Count())
	{
		return false;
	}
	iPoint pos = { (int)position.x, (int)position.y };
	pos = App->map->WorldToMap(pos.x, pos.y);

	if (pos.x > current_point.x - 1 && pos.x < current_point.x + 1 && pos.y > current_point.y - 1 && pos.y < current_point.y + 1)
	{
		/*previous_point =  current_point;*/
		if (last_path.Count()) 
		{
			last_path.Pop(current_point);
		}
	}
	

	float   distance;
	distance = fPoint(pos.x, pos.y).DistanceTo(fPoint(current_point.x, current_point.y));
	vector = { current_point.x - (float)pos.x , current_point.y - (float)pos.y };
	vector = { vector.x / distance , vector.y / distance };

   	position += { velocity.x * vector.x * dt, velocity.y * vector.y * dt};

	return true;
}

bool Enemy::CheckTargetRatio()
{
	bool ret = false;

	if (detection_ratio < MAX_DETECTION_RATIO && target)
	{
		if (position.DistanceManhattan(App->entity_manager->GetPlayer()->position) < detection_ratio)
		{
			target_detected = ret = true;
		}
	}
	return target_detected = ret;
}