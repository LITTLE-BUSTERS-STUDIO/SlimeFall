#ifndef __j1Collision_H__
#define __j1Collision_H__

#define MAX_COLLIDERS 50

#include "j1Module.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "SDL/include/SDL.h"

enum class Direction : uint
{
	right,
	left,
	up,
	down,
	unknown
};

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_DEATH,
	COLLIDER_NEXT_LEVEL,
	COLLIDER_GOD,
	COLLIDER_ATTACK,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	COLLIDER_TYPE type = COLLIDER_NONE;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};


class j1Collision : public j1Module
{
public:

	j1Collision();

	virtual ~j1Collision();

	bool Update(float dt);

	bool PostUpdate(float dt);

	bool CleanUp();

	Collider  *AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);

	Direction  SolveOverlap(Collider *dynamic_col, Collider *static_col, fPoint &position ,fPoint &velocity);

	bool CheckOverlap(p2List<Direction> &directions, Collider *dynamic_col, COLLIDER_TYPE type, fPoint &position, fPoint &velocity);

	bool DeleteCollider(Collider* collider);

private:
	p2List<Collider*> colliders;
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};

#endif // __j1Collision_H__
