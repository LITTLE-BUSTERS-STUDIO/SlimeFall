#ifndef __Enemy_H__
#define __Enemy_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

enum class EntityType 
{
	Player,
	Enemy,
	Object,
	Unknown
};

class Entity
{
public:
	Entity(iPoint position, SDL_Rect collider_rect);
	virtual ~Entity();

	virtual bool HandleInput() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool Draw() { return true; };
	virtual bool OnCollision(Collider* collider) { return true; };

	const Collider* GetCollider() const;

protected:
	iPoint      position;
	Collider*   collider = nullptr;
	EntityType  type;
};

#endif // __Enemy_H__
