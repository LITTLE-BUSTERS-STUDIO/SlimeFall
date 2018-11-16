#ifndef __Enemy_H__
#define __Enemy_H__

#include "Entity.h"
#include "j1Timer.h"
#include "p2DynArray.h"

struct SDL_Texture;
struct Collider;
struct Entity_Info;

class Enemy : public Entity
{
public:
	Enemy(fPoint position, Entity_Info info);

	virtual bool Update(float dt) { return true; };

	virtual bool Draw(SDL_Texture* sprites) { return true; };

	virtual bool OnCollision(Collider* c1, Collider* c2 ) { return true; };

	float                 path_interval_time = 1000.0f;
	j1Timer               path_timer;
	SDL_Rect              detection_ratio;
	p2DynArray<iPoint>    last_path;
};

#endif // __Enemy_H__
