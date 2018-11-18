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
	~Enemy();

	// Common methods ===============================

	bool UpdateLogic();   // Update only the path

	virtual bool FollowPath(float dt);

	bool CheckTargetRatio();

	bool  Active();

	bool  Desactive();

	// Variables  ===================================

	Collider*             main_collider = nullptr;
	j1Timer               path_timer;
	p2DynArray<iPoint>    last_path;
	Entity*               target = nullptr;                 // Entity to follow
           
	Animation             animation;
	uint32                path_interval_time = 0u;          // Time between every path creation (ms)
	int                   detection_ratio;

	iPoint                current_point = {-1,-1};
	fPoint				  previous_position;
	iPoint                error_margin;

	bool                  add_error_margin = false;
	bool                  target_detected;
	bool                  new_path = true;
};

#endif // __Enemy_H__
