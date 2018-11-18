#include "Enemy_Bat.h"
#include "j1App.h"
#include "EntityManager.h"
#include "j1Collision.h"
#include "j1Textures.h"
#include "j1Player.h"

#include "j1Map.h"
#include "j1Render.h"

#include "Brofiler/Brofiler.h"


Enemy_Bat::Enemy_Bat(fPoint position, Entity_Info info) :Enemy( position, info)
{
	Enemy_Bat_Properties* properties = (Enemy_Bat_Properties *)info.properties;

	// Textures ------------------------------------------
	tex_smoke = properties->smoke_tex;
	tex_bat = properties->bat_tex;
	// Animations ----------------------------------------
	bat_anim = properties->bat_anim;
	smoke_anim = properties->smoke_anim;
}

Enemy_Bat::~Enemy_Bat()
{

}

bool Enemy_Bat::Update(float dt)
{
	target = (Entity*)App->entity_manager->GetPlayer();

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

	switch ((Enemy_Bat_State)current_state)
	{
	case Enemy_Bat_State::flying:
		if (bat_anim.GetFrameValue() > 9)
		{
			frame = bat_anim.GetCurrentFrame();
			texture = tex_bat;
			bat_anim.Reset();
		}
		frame = bat_anim.GetCurrentFrame();
		texture = tex_bat;
		break;

	case Enemy_Bat_State::dead:
		if (smoke_anim.GetFrameValue() > 9)
		{
			current_state = Enemy_Bat_State::flying;
			frame = smoke_anim.GetCurrentFrame();
			texture = tex_smoke;
			smoke_anim.Reset();
		}
		frame = smoke_anim.GetCurrentFrame();
		texture = tex_smoke;
		break;

	default:
		break;
	}


	if (position.x < App->entity_manager->GetPlayer()->position.x)
	{
		flip_x = true;
		margin_flip = main_collider->rect.w + main_collider->rect.w/2;
	}
	else
	{
		flip_x = false;
		margin_flip = main_collider->rect.w;
	}
	
	if(current_state == Enemy_Bat_State::dead)
		margin_flip = main_collider->rect.w;

	// Draw =================================================
	App->render->Blit(texture, position.x - margin_flip, position.y - main_collider->rect.h, &frame, flip_x);
	return true;
}

bool Enemy_Bat::Reset(Entity_Info  info)
{
	BROFILER_CATEGORY("Enemy_Bat Reset", Profiler::Color::LightGray);

	
	velocity.x = 0;
	velocity.y = 0;
	acceleration.x = 0;
	acceleration.y = 0;
	bat_anim.Reset();
	return true;
}



bool Enemy_Bat::OnCollision(Collider* c1, Collider* c2)
{

	if (c1 == main_collider)
	{
		switch (c2->type)
		{
		case COLLIDER_ATTACK:
			current_state = Enemy_Bat_State::dead;
			break;
		default:
			break;
		}
	}
	return true;
}