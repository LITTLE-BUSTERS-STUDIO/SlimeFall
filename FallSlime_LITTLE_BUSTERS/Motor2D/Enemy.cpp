#include "Enemy.h"
#include "EntityManager.h"
#include "J1Player.h"
#include "j1PathFinding.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "p2Log.h"
#include "Brofiler/Brofiler.h"

#define MAX_DETECTION_RATIO 400

Enemy::Enemy(fPoint position, Entity_Info info) :Entity(position, info)
{
	main_collider = App->collision->AddCollider(info.properties->collider_rect, COLLIDER_ENEMY, App->entity_manager);
	colliders.add(main_collider);

	iPoint pos = { (int)position.x, (int)position.y};
	current_point = App->map->WorldToMap(pos.x, pos.y);
	
	velocity = { 40, 40  };
	path_interval_time = 400u;
	detection_ratio = 300;

}


bool Enemy::UpdateLogic() 
{
	if (  path_timer.Read() > path_interval_time ) 
	{
		iPoint map_target(target->position.x, target->position.y);
		iPoint map_pos(position.x, position.y);

		map_target = App->map->WorldToMap(map_target.x, map_target.y);
		map_pos = App->map->WorldToMap(map_pos.x, map_pos.y);
		App->path_finding->CreatePath( map_pos, map_target, last_path);
		new_path = true;
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

	if (new_path)
	{
		last_path.Pop(current_point);
		last_path.Pop(current_point);
		new_path = false;
	}

	fPoint velocity_to_follow;
	iPoint node_in_world;
	node_in_world = App->map->MapToWorld(current_point.x, current_point.y);

	velocity_to_follow.x = (float)node_in_world.x - position.x;
	velocity_to_follow.y = (float)node_in_world.y - position.y;

	velocity_to_follow.Normalize();

	position.x += velocity_to_follow.x * 100.0f * dt;
	position.y += velocity_to_follow.y * 100.0f * dt;

	if (position.x > node_in_world.x - 10 && position.x < node_in_world.x + 10 && position.y > node_in_world.y - 10 && position.y < node_in_world.y + 10)
	{
		if (last_path.Count())
		{
			last_path.Pop(current_point);
		}
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
			target_detected = ret = true;
		}
	}
	return target_detected = ret;
}

