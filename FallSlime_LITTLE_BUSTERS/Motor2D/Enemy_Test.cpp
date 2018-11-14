#include "Enemy_Test.h"

Enemy_Test::Enemy_Test(iPoint position, SDL_Rect collider_rect) :Entity( position, collider_rect)
{

}

bool Enemy_Test::Move(float dt)
{

	return true;
}

bool Enemy_Test::Draw(SDL_Texture* sprites)
{
	return true;
}

bool Enemy_Test::OnCollision(Collider* collider)
{
	return true;
}