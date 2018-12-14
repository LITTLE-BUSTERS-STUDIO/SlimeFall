#ifndef __j1Player_H__
#define __j1Player_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "Entity.h"
#include "j1Timer.h"


struct SDL_Texture;

enum class Player_State
{
	jumping,
	boucing,
	dead,
	attack
};



class j1Player : public Entity
{
public:

	j1Player(fPoint position, fPoint spawn_pos, Properties *properties);

	virtual ~j1Player();

	bool HandleInput();

	bool Update(float dt);

	bool Draw();

	bool OnCollision(Collider* c1, Collider* c2);

	// Save & Load =============================
	bool Load(pugi::xml_node& node);

	bool Save(pugi::xml_node& node) const;

	// Methods ================================ 
	bool Invulnerability(float);

	bool Reset();

public :
	bool				attack_tremble = false;

private:
	//Debug====================================
	bool				god_mode = false;
	// Physics= ===================================
	float		        gravity;
	float				speed_air;
	float				speed_ground;
	float				speed_jump;
	float				speed_gummy_jump;
	float				speed_attack;

	// States ==================================
	Player_State				current_state = Player_State::jumping;
	//Bool =====================================
	
	bool                gummy_jump = false;
	bool			    attack = false;
	bool				apply_jump_speed = false;
	bool				apply_attack = false;
	bool				apply_invulnerability = false;
	bool				apply_timer = true;
	bool				is_gummy_jumping = false;
	bool                on_ground = false;
	bool                check_fall = false;
	bool                flip_x = false;
	// Collision ================================
	Collider           *collider = nullptr;
	SDL_Rect            collider_rect;

	// Abilities ================================
	j1Timer             timer_invulnerability;

	// Assets ===================================

	//-----------Textures-------------------
	SDL_Texture      *tex_player = nullptr;
	SDL_Texture      *death_splash = nullptr;
	SDL_Texture      *attack_splash = nullptr;

	//----------Animations-----------------
	Animation         jumping_anim;
	Animation         death_anim;
	Animation         attack_anim;

	//------------Sfx----------------------
	uint			  fx_jump1;
	uint			  fx_jump2;
	uint			  fx_jump3;
	uint			  fx_jump4;
	uint			  fx_jump5;
	uint		      id_death_fx;
	uint		      fx_attack;
	bool			  dead_enable = false;

};


#endif // !__j1Player_H__

