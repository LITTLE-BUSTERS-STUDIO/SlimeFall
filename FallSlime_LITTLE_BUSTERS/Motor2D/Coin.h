#ifndef __Coin_H__
#define __Coin_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "Entity.h"
#include "j1Timer.h"

struct SDL_Texture;



class Coin : public Entity
{
public:

	Coin(fPoint pos, Entity_Info info);

	virtual ~Coin();


	bool Update(float dt);

	bool Draw();

	bool OnCollision(Collider* c1, Collider* c2);


private:
	//Debug====================================
	// Physics= ===================================
	// States ================================
	//Bool =====================================
	// Collision ================================
	Collider           *collider = nullptr;
	SDL_Rect            collider_rect;

	// Assets ===================================

	//-----------Textures-------------------
	SDL_Texture      *tex_coin = nullptr;

	//----------Animations-----------------
	Animation         coin_anim;

	//------------Sfx----------------------
	uint		      fx_pick_up;
};


#endif // !__Coin_H__