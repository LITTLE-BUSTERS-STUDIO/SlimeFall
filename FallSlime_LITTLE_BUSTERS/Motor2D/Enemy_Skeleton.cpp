#include "Enemy_Skeleton.h"
#include "j1Window.h"
#include "j1App.h"
#include "EntityManager.h"
#include "j1Collision.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1PathFinding.h"
#include "p2Log.h"

#include "j1Map.h"
#include "j1Render.h"

#include "Brofiler/Brofiler.h"


Enemy_Skeleton::Enemy_Skeleton(fPoint position, Entity_Info info) :Enemy(position, info)
{
	Enemy_Skeleton_Properties* properties = (Enemy_Skeleton_Properties *)info.properties;

	target = (Entity*)App->entity_manager->GetPlayer();


	// Textures ------------------------------------------
	tex_skeleton = properties->skeleton_tex;

	// Animations ------------------------------
	skeleton_walking_anim = properties->skeleton_walking_anim;
	skeleton_dead_anim = properties->skeleton_dead_anim;
	skeleton_attack_anim = properties->skeleton_attack_anim;

}

Enemy_Skeleton::~Enemy_Skeleton()
{
}

bool Enemy_Skeleton::Update(float dt)
{
	velocity = { 100,100 };

	if (CheckTargetRatio())
	{
		UpdateLogic();
		FollowPath(dt);
	}

	iPoint check_ground;

	check_ground.x = position.x;
	check_ground.y = position.y + 2;
	if(App->path_finding->IsWalkable(check_ground))


	return true;
}


bool Enemy_Skeleton::Draw()
{
	for (uint i = 0; i < last_path.Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(last_path.At(i)->x, last_path.At(i)->y);
		App->render->DrawQuad({ pos.x, pos.y ,16,16 }, 0, 0, 0, 200);
	}

	SDL_Rect frame;
	SDL_Texture* texture = tex_skeleton;
	
	switch ((Enemy_Skeleton_State)current_state)
	{
	case Enemy_Skeleton_State::walking:
		if (skeleton_walking_anim.GetFrameValue() > 9)
		{
			frame = skeleton_walking_anim.GetCurrentFrame();
			skeleton_walking_anim.Reset();
		}
		frame = skeleton_walking_anim.GetCurrentFrame();
		break;

	case Enemy_Skeleton_State::attack:
		//frame = skeleton_attack_anim.GetCurrentFrame();
		break;

	case Enemy_Skeleton_State::dead:
		if (skeleton_dead_anim.GetFrameValue() > 20)
		{
			current_state = Enemy_Skeleton_State::walking;
			frame = skeleton_dead_anim.GetCurrentFrame();
			skeleton_dead_anim.Reset();
		}
		frame = skeleton_dead_anim.GetCurrentFrame();
		break;

	default:
		break;
	}
	skeleton_attack_anim.speed = skeleton_dead_anim.speed = skeleton_walking_anim.speed = 15.0F;

	if (skeleton_attack_anim.GetFrameValue() > 20)
		skeleton_attack_anim.Reset();
	if (skeleton_dead_anim.GetFrameValue() > 20)
		skeleton_dead_anim.Reset();
	if (skeleton_walking_anim.GetFrameValue() > 20)
		skeleton_walking_anim.Reset();



	if (position.x < App->entity_manager->GetPlayer()->position.x)
	{
		margin_flip = main_collider->rect.w - main_collider->rect.w / 2;
		flip_x = false;
	}
	else
	{
		flip_x = true;
		margin_flip = main_collider->rect.w + main_collider->rect.w / 2;
	}

		
	
	App->render->Blit(texture, position.x - margin_flip, position.y - main_collider->rect.h *3/2 + 5, &frame, flip_x);
	return true;
}

bool Enemy_Skeleton::Reset(fPoint pos)
{
	BROFILER_CATEGORY("Enemy_Bat Reset", Profiler::Color::LightGray);

	position = pos;
	velocity.x = 0;
	velocity.y = 0;
	acceleration.x = 0;
	acceleration.y = 0;

	return true;
}


bool Enemy_Skeleton::FollowPath(float dt)
{
	if (!last_path.Count() > 0)
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

	iPoint check_ground_right;
	//iPoint check_ground_left;

	check_ground_right.x = node_in_world.x /*+ main_collider->rect.w*/;
	check_ground_right.y = node_in_world.y + main_collider->rect.h + 30;

	//check_ground_left.x = node_in_world.x;
	//check_ground_left.y = node_in_world.y + main_collider->rect.h - 100;

	check_ground_right = App->map->WorldToMap(check_ground_right.x, check_ground_right.y);
	//check_ground_left = App->map->WorldToMap(check_ground_left.x, check_ground_left.y);

	//LOG("%f   %f", check_ground_left.x, check_ground_left.y);
	/*LOG("position %i   %i", node_in_world.x, node_in_world.y);
	LOG("collider %i   %i", main_collider->rect.w, main_collider->rect.h);*/

	if (node_in_world.x > position.x && !App->path_finding->IsWalkable(check_ground_right))
	{
		velocity_to_follow.x = 50;
		LOG("RIGHT");
	}
	

	else if (node_in_world.x < position.x && !App->path_finding->IsWalkable(check_ground_right))
	{
		LOG("LEFT");
		velocity_to_follow.x = -50;

	}

	else
	{
		velocity_to_follow.x = 0;
		check_ground_right.y = node_in_world.y + main_collider->rect.h;
		LOG("IDLE");

	}

	position.x += velocity_to_follow.x  * dt;

	main_collider->SetPos(position.x - main_collider->rect.w / 2, position.y - main_collider->rect.h / 2);

	p2List<Direction> directions;
	App->collision->CheckOverlap(directions, main_collider, COLLIDER_WALL, position, velocity_to_follow);

	main_collider->SetPos(position.x - main_collider->rect.w / 2, position.y - main_collider->rect.h / 2);

	if (position.x > node_in_world.x - 10 && position.x < node_in_world.x + 10)
	{
		if (last_path.Count())
		{
			last_path.Pop(current_point);
		}
	}

	return true;
}

bool Enemy_Skeleton::OnCollision(Collider* c1, Collider* c2)
{

	if (c1 == main_collider)
	{
		switch (c2->type)
		{
		case COLLIDER_ATTACK:
			current_state = Enemy_Skeleton_State::dead;
			break;

		default:
			break;
		}
	}
	return true;
}