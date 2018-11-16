#include "Enemy_Test.h"

Enemy_Bat::Enemy_Bat(fPoint position, Entity_Info info) :Enemy( position, info)
{

}

bool Enemy_Bat::Update(float dt)
{

	return true;
}

bool Enemy_Bat::Draw(SDL_Texture* sprites)
{
	return true;
}

bool Enemy_Bat::OnCollision(Collider* c1, Collider* c2)
{
	return true;
}