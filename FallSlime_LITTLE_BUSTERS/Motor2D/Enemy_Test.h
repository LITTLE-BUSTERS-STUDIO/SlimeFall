#ifndef __Enemy_Test_H__
#define __Enemy_Test_H__

#include "Entity.h"
#include "Enemy.h"

struct SDL_Texture;
struct Collider;
struct Entity_Info;

class Enemy_Bat: public Enemy
{
public:
	Enemy_Bat(fPoint position, Entity_Info info);

	bool Update(float dt) ;

	bool Draw(SDL_Texture* sprites) ;

	bool OnCollision(Collider* c1, Collider* c2) ;
};

#endif // __Enemy_Test_H__

