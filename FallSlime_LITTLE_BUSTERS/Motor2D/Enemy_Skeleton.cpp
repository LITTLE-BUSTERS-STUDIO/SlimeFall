#include "Enemy_Skeleton.h"
#include "j1Window.h"
#include "j1App.h"
#include "EntityManager.h"
#include "j1Collision.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1PathFinding.h"
#include "p2Log.h"
#include "j1Audio.h"

#include "j1Map.h"
#include "j1Render.h"
#include "Brofiler/Brofiler.h"


Enemy_Skeleton::Enemy_Skeleton(fPoint position, fPoint spawn_pos, Properties *properties) :Enemy(position, spawn_pos, properties)
{
	name.create("enemy_skeleton");

	Enemy_Skeleton_Properties* enemy_properties = (Enemy_Skeleton_Properties *)properties;
	target = (Entity*)App->entity_manager->GetPlayer();

	// Textures ------------------------------------------
	tex_skeleton = enemy_properties->skeleton_tex;

	// Animations ------------------------------
	skeleton_walking_anim = enemy_properties->skeleton_walking_anim;
	skeleton_dead_anim = enemy_properties->skeleton_dead_anim;
	skeleton_attack_anim = enemy_properties->skeleton_attack_anim;
	skeleton_idle_anim = enemy_properties->skeleton_idle_anim;

	// Sfx ----------------------------------------------
	fx_skaleton_death = enemy_properties->id_skeleton_death_fx;
}

Enemy_Skeleton::~Enemy_Skeleton()
{
}

bool Enemy_Skeleton::Update(float dt)
{
	target = (Entity*)App->entity_manager->GetPlayer();

	if (current_state == Enemy_Skeleton_State::dead)
	{
		return true;
	}

	if (CheckTargetRatio())
	{
		UpdateLogic();
		FollowPath(dt);
	}

	iPoint check_ground;

	check_ground.x = position.x;
	check_ground.y = position.y + 2;
	if (App->path_finding->IsWalkable(check_ground))

	if (main_collider)
	{
		main_collider->SetPos(position.x - main_collider->rect.w / 2, position.y - main_collider->rect.h / 2);
	}


	
	return true;
}


bool Enemy_Skeleton::Draw()
{
	DrawPath();

	SDL_Rect frame;
	SDL_Texture* texture = tex_skeleton;
	skeleton_dead_anim.speed = skeleton_walking_anim.speed = 15.0F;
	skeleton_idle_anim.speed = 5.0F;

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

	case Enemy_Skeleton_State::idle:
		if (skeleton_idle_anim.GetFrameValue() > 9)
		{
			current_state = Enemy_Skeleton_State::walking;
			frame = skeleton_idle_anim.GetCurrentFrame();
			skeleton_idle_anim.Reset();
			if (skeleton_idle_anim.GetFrameValue() >= 9)
				Desactive();
		}
		frame = skeleton_idle_anim.GetCurrentFrame();


		break;

	case Enemy_Skeleton_State::dead:

		main_collider->type = COLLIDER_NONE;
		if (skeleton_dead_anim.GetFrameValue() > 18)
		{
			current_state = Enemy_Skeleton_State::walking;
			frame = skeleton_dead_anim.GetCurrentFrame();
			skeleton_dead_anim.Reset();
			Desactive();
		}
		frame = skeleton_dead_anim.GetCurrentFrame();

		if (!dead_fx)
		{
			App->audio->PlayFx(fx_skaleton_death);
			dead_fx = true;
		}
		break;

	default:
		break;
	}

	if (position.x < App->entity_manager->GetPlayer()->GetPosition().x)
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

bool Enemy_Skeleton::Reset()
{
	BROFILER_CATEGORY("Enemy_Skeleton Reset", Profiler::Color::LightGray);
	last_path.Clear();
	current_state = Enemy_Skeleton_State::walking;
	dead_fx = false;
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

	iPoint check_ground;

	check_ground.x = node_in_world.x;
	check_ground.y = node_in_world.y + main_collider->rect.h + 30; 


	check_ground = App->map->WorldToMap(check_ground.x, check_ground.y);

	//IsWalkabre enemy walking Calculation
	if (node_in_world.x > position.x && !App->path_finding->IsWalkable(check_ground))
	{
		velocity_to_follow.x = 50;
	}
	else if (node_in_world.x < position.x && !App->path_finding->IsWalkable(check_ground))
	{
		velocity_to_follow.x = -50;
	}
	else
	{
		velocity_to_follow.x = 0;
		check_ground.y = node_in_world.y + main_collider->rect.h;
	}

	if (velocity_to_follow.x != 0 && current_state != Enemy_Skeleton_State::dead)
	{
		current_state = Enemy_Skeleton_State::walking;

	}
	else if (current_state != Enemy_Skeleton_State::dead)
		current_state = Enemy_Skeleton_State::idle;


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