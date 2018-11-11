#include "Enemy_Test.h"

Enemy_Test::Enemy_Test(int x, int y) :Enemy(x, y)
{

}
Enemy_Test::~Enemy_Test()
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