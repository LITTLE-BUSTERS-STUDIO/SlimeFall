#include "j1App.h"
#include "Enemy.h"
#include "j1Render.h"
#include "j1Collision.h"

Enemy::Enemy(int x, int y) : position(x, y), collider(nullptr)
{

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

