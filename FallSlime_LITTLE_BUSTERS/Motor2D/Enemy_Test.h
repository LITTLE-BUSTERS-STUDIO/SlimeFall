#ifndef __Enemy_Test_H__
#define __Enemy_Test_H__

#include "Entity.h"

struct SDL_Texture;
struct Collider;
struct Entity_Info;

class Enemy_Test: public Entity
{
public:
	Enemy_Test(fPoint position, Entity_Info info);

	bool Update(float dt) ;
	bool Draw(SDL_Texture* sprites) ;
	bool OnCollision(Collider* collider) ;
};

#endif // __Enemy_Test_H__

