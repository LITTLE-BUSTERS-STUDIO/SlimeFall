#include "j1App.h"
#include "Enemy.h"
#include "Enemies.h"
#include "j1Render.h"
#include "j1Collision.h"

Enemy::Enemy(fPoint position, SDL_Rect collider_rect) : position( position), collider(nullptr)
{
	collider = App->collision->AddCollider(collider_rect, COLLIDER_ENEMY, App->enemies);
}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

