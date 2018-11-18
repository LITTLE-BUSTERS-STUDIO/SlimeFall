#ifndef __Enemy_Bat_H__
#define __Enemy_Bat_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Player.h"
#include "Entity.h"
#include "Enemy.h"

struct SDL_Texture;
struct Collider;
struct Entity_Info;

enum class Enemy_Bat_State
{
	flying,
	dead
};


class Enemy_Bat: public Enemy
{
public:
	Enemy_Bat(fPoint position, Entity_Info info);

	~Enemy_Bat();

	bool Update(float dt) ;

	bool Draw();

	bool Reset(Entity_Info  info);

	bool OnCollision(Collider* c1, Collider* c2) ;

public:
	// States ================================
	Enemy_Bat_State				current_state = Enemy_Bat_State::flying;

private:
	//Bool ===================================
	bool              flip_x = false;
	//Vars ===================================
	int               margin_flip;

	//-----------Textures-------------------
	SDL_Texture		  *tex_smoke = nullptr;
	SDL_Texture		  *tex_bat = nullptr;

	//----------Animations-----------------
	Animation         smoke_anim;
	Animation         bat_anim;


};

#endif // __Enemy_Bat_H__

