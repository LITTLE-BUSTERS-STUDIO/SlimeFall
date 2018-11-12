#ifndef __j1Player_H__
#define __j1Player_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;

enum class State
{
	jumping,
	boucing,
	dead
};

enum class Direction: uint
{
	right,
	left,
	up,
	down,
	max
};

class j1Player : public j1Module
{
public:

	j1Player();
	virtual ~j1Player();

	void Init();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Save & Load
	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	// Reset player values default
	bool Reset(fPoint pos);

	// Called by collision module
	bool OnCollision(Collider*, Collider*);

	//Debug====================================
	bool   god_mode = false;

	//Transform================================
	fPoint position;
	fPoint velocity;
	fPoint acceleration;
	State  current_state = State::jumping;
	bool   reset = false;

	//Physics==================================
	float gravity;
	float speed_air;
	float speed_ground;
	float speed_jump;
	float speed_gummy_jump;
	float speed_attack;
	bool  apply_jump_speed = false;
	bool  apply_attack = false;

	//Collision================================
	Collider         *collider = nullptr;
	Collider         *ground_detector = nullptr;
	bool              on_ground = false;
	bool              check_fall = false;
	SDL_Rect          rect_collider;
	bool              flip_x = false;

	//Abilities================================
	bool              gummy_jump = false;
	bool			  attack = false;

	//Assets===================================
	//-----------Textures-------------------
	p2SString         path_tex_player;
	p2SString         path_death_splash;
	SDL_Texture      *tex_player = nullptr;
	SDL_Texture      *death_splash = nullptr;
	//----------Animations-----------------
	Animation         jumping_anim;
	Animation         death_anim;
	//------------Sfx----------------------
	p2SString         path_jump_fx1;
	p2SString         path_jump_fx2;
	p2SString         path_jump_fx3;
	p2SString         path_jump_fx4;
	p2SString         path_jump_fx5;
	p2SString         path_death_sfx;
	uint fx_jump1;
	uint fx_jump2;
	uint fx_jump3;
	uint fx_jump4;
	uint fx_jump5;
	uint id_death_sfx;
	//=========================================
};


#endif // !__j1Player_H__

