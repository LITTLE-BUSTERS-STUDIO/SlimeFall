#include "j1App.h"
#include "Enemy.h"
#include "Enemies.h"
#include "j1Render.h"
#include "j1Collision.h"

Enemy::Enemy(int x, int y, int width, int height) : position(x, y), collider(nullptr)
{
	collider = App->collision->AddCollider({x - width/2 , y - height/2, width, height}, COLLIDER_ENEMY, App->enemies);
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

