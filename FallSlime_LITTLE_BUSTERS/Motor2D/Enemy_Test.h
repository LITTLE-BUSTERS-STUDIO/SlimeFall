#ifndef __Enemy_Test_H__
#define __Enemy_Test_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Player.h"
#include "Entity.h"
#include "Enemy.h"

struct SDL_Texture;
struct Collider;
struct Entity_Info;

enum class Enemy_State
{
	flying,
	dead
};


class Enemy_Bat: public Enemy
{
public:
	Enemy_Bat(fPoint position, Entity_Info info);

	virtual ~Enemy_Bat();

	bool Update(float dt) ;

	bool Draw();

	bool Enemy_Bat::Reset(fPoint pos);

	bool OnCollision(Collider* c1, Collider* c2) ;

public:
	// States ================================
	Enemy_State				current_state = Enemy_State::flying;
	// Collision =============================
	Collider				*enemy_collider = nullptr;

private:

	//Bool ===================================
	bool                flip_x = false;
	//Vars ===================================
	int margin_flip;

	//-----------Textures-------------------
	SDL_Texture *	tex_smoke = nullptr;
	SDL_Texture *	tex_bat = nullptr;

	//----------Animations-----------------
	Animation         smoke_anim;
	Animation         bat_anim;


};

#endif // __Enemy_Test_H__

