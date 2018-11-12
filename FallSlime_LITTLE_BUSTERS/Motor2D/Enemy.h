#ifndef __Enemy_H__
#define __Enemy_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class Enemy
{
public:
	Enemy(fPoint position, SDL_Rect collider_rect);
	virtual ~Enemy();

	virtual bool Move(float dt) { return true; };
	virtual bool Draw(SDL_Texture* sprites) { return true; };
	virtual bool OnCollision(Collider* collider) { return true; };

	const Collider* GetCollider() const;

protected:
	fPoint      position;
	Animation   animation;
	Collider*   collider = nullptr;

};

#endif // __Enemy_H__
