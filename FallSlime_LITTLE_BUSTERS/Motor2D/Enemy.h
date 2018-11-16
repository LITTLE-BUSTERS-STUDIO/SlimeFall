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
	// Virtual methods ===============================
	Enemy(fPoint position, Entity_Info info);

	virtual bool Update(float dt) { return true; };

	virtual bool Draw(SDL_Texture* sprites) { return true; };

	virtual bool OnCollision(Collider* c1, Collider* c2 ) { return true; };

	// Common methods ===============================
	bool UpdateLogic();   // Update only the path

	bool FollowPath();   

	bool CheckTargetRatio();

	Entity*               target = nullptr;                 // Entity to follow
	uint32                path_interval_time = 0u;          // Time between every path creation (ms)
	int                   detection_ratio;                  

	j1Timer               path_timer;
	p2DynArray<iPoint>    last_path;                      
	Animation             animation;
	
};

#endif // __Enemy_H__
