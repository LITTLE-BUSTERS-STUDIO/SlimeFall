#ifndef __Coin_H__
#define __Coin_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "Entity.h"

struct SDL_Texture;
struct Collider;
struct Entity_Info;

enum class Coin_States
{
	enable,
	disable
};

class Coin : public Enemy
{
public:

	Coin(fPoint pos, Entity_Info info);

	~Coin();


	bool Update(float dt);

	bool Draw();

	bool Reset(Entity_Info  info);

	bool OnCollision(Collider* c1, Collider* c2);


private:

	Coin_States				current_state = Coin_States::enable;
	//-----------Vars-----------------------
	bool			  enable_fx = false;
	uint  			  coin_counter = NULL;

	//-----------Textures-------------------
	SDL_Texture      *tex_coin = nullptr;

	//----------Animations-----------------
	Animation         coin_anim;

	//------------Sfx----------------------
	uint		      fx_pick_up;
};


#endif // !__Coin_H__