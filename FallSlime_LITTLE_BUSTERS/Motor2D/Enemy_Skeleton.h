#ifndef __Enemy_Skeleton_H__
#define __Enemy_Skeleton_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Player.h"
#include "Entity.h"
#include "Enemy.h"

struct SDL_Texture;
struct Collider;
struct Entity_Info;

enum class Enemy_Skeleton_State
{
	walking,
	attack,
	dead
};


class Enemy_Skeleton : public Enemy
{
public:
	Enemy_Skeleton(fPoint position, Entity_Info info);

	virtual ~Enemy_Skeleton();

	bool Update(float dt);

	bool Draw();

	bool Enemy_Skeleton::Reset(fPoint pos);

	bool OnCollision(Collider* c1, Collider* c2);

public:
	// States ================================
	Enemy_Skeleton_State	current_state = Enemy_Skeleton_State::walking;
	// Collision =============================
	Collider				*enemy_collider = nullptr;

private:

	//Bool ===================================
	bool				    flip_x = false;
	//Vars ===================================
	int						margin_flip;
	fPoint					skeleton_pos;
	//-----------Textures-------------------
	SDL_Texture      *tex_skeleton = nullptr;

	//----------Animations-----------------
	Animation         skeleton_attack_anim;
	Animation         skeleton_dead_anim;
	Animation         skeleton_walking_anim;



};

#endif // __Enemy_Skeleton_H__