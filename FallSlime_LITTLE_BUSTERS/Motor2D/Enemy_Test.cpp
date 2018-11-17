#include "Enemy_Test.h"
#include "j1Collision.h"
#include "Brofiler/Brofiler.h"

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
	if (main_collider == c1 && c2->type == COLLIDER_TYPE::COLLIDER_WALL) 
	{
      	App->collision->ResolveOverlap(c1, c2, position, velocity);
	}
	return true;
}