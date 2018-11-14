#ifndef __Enemy_Test_H__
#define __Enemy_Test_H__

#include "Entity.h"

struct SDL_Texture;
struct Collider;

class Enemy_Test: public Entity
{
public:
	Enemy_Test(iPoint position, SDL_Rect collider_rect);

	bool Move(float dt) ;
	bool Draw(SDL_Texture* sprites) ;
	bool OnCollision(Collider* collider) ;
};

#endif // __Enemy_Test_H__

