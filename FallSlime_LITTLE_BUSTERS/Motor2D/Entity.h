#ifndef __Enemy_H__
#define __Enemy_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;
class Properties;
struct Entity_Info;

class Properties
{
public:
	Properties() {}
	~Properties() {}

	p2SString            name;
	SDL_Rect             collider_rect;
	SDL_Rect             spawn_rect;
};


class Entity_Info
{
public:
	p2SString            name;
	iPoint               position;
	Properties           properties;
	bool                 spawned = false;

	Entity_Info() {}

	Entity_Info(iPoint spawn_position, Properties properties)
	{
		position = spawn_position;
		name = properties.name;

		this->properties.spawn_rect = properties.spawn_rect;
		this->properties.spawn_rect.x = spawn_position.x - properties.spawn_rect.w / 2;
		this->properties.spawn_rect.y = spawn_position.y - properties.spawn_rect.h / 2;

		this->properties.collider_rect = properties.collider_rect;
		this->properties.collider_rect.x = spawn_position.x - properties.collider_rect.w / 2;
		this->properties.collider_rect.y = spawn_position.y - properties.collider_rect.h / 2;
	}
};


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
	Entity(iPoint position, Entity_Info info);
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
