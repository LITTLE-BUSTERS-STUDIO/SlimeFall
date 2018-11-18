#include "Enemy_Skeleton.h"
#include "j1App.h"
#include "EntityManager.h"
#include "j1Collision.h"
#include "j1Textures.h"
#include "j1Player.h"

#include "j1Map.h"
#include "j1Render.h"

#include "Brofiler/Brofiler.h"


Enemy_Skeleton::Enemy_Skeleton(fPoint position, Entity_Info info) :Enemy(position, info)
{
	target = (Entity*)App->entity_manager->GetPlayer();

	Enemy_Properties* enemy_properties = (Enemy_Properties *)info.properties;

	// Textures ------------------------------------------

	// Animations ----------------------------------------
	
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

	if (main_collider)
	{
		main_collider->SetPos(position.x - main_collider->rect.w / 2, position.y - main_collider->rect.h / 2);
	}

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
	SDL_Texture* texture = nullptr;
	
	switch ((Enemy_Skeleton_State)current_state)
	{
	case Enemy_Skeleton_State::walking:

		break;
	case Enemy_Skeleton_State::attack:
		break;

	case Enemy_Skeleton_State::dead:
		break;

	default:
		break;
	}


	if (position.x < App->entity_manager->GetPlayer()->position.x)
		flip_x = true;
	else
		flip_x = false;
		
	
	App->render->Blit(texture, position.x , position.y, &frame, flip_x);
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
	//bat_anim.Reset();

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