#ifndef __Entity_H__
#define __Entity_H__

#include "p2Point.h"
#include "Animation.h"
#include "p2List.h"

struct SDL_Texture;
struct Collider;
class Properties;
struct Entity_Info;

class Properties
{
public:
	Properties() {}
	virtual ~Properties() {}

	p2SString            name;
	SDL_Rect             collider_rect = {0,0,0,0};
	SDL_Rect             spawn_rect = { 0,0,0,0 };
};

class Player_Properties : public Properties
{
public:
	//------------Valeues-------------------
	float			    gravity = 0.0f;
	float			    speed_ground = 0.0f;
	float			    speed_air = 0.0f;
	float			    speed_jump = 0.0f;
	float			    speed_gummy_jump = 0.0f;
	float               speed_attack = 0.0f;

	//-----------Animations-----------------
	Animation           jumping_anim;
	Animation		    death_anim;
	Animation		    attack_anim;

	//-----------Textures-------------------
	SDL_Texture*        player_tex = nullptr;
	SDL_Texture*        death_tex = nullptr;
	SDL_Texture*        attack_tex = nullptr;
	
	//------------Sfx----------------------
	int                 id_jump_fx1 = 0;
	int                 id_jump_fx2 = 0;
	int                 id_jump_fx3 = 0;
	int                 id_jump_fx4 = 0;
	int                 id_jump_fx5 = 0;
	int                 id_death_fx = 0;
	int                 id_attack_fx = 0;
};


class Enemy_Properties : public Properties
{
public:

	int               detection_ratio;
	float             path_interval_time;
};

class Enemy_Bat_Properties : public Enemy_Properties
{
public:
	//-----------Animations-----------------
	Animation		  bat_anim;
	Animation         smoke_anim;

	//-----------Textures-----------------
	SDL_Texture       *bat_tex;
	SDL_Texture       *smoke_tex;

	//------------Sfx----------------------
	int                id_bat_death_fx;


};

class Enemy_Skeleton_Properties : public Enemy_Properties
{
public:
	//-----------Animations-----------------
	Animation         skeleton_attack_anim;
	Animation         skeleton_dead_anim;
	Animation         skeleton_walking_anim;
	Animation         skeleton_idle_anim;

	//-----------Textures-----------------
	SDL_Texture      *skeleton_tex;

	//------------Sfx----------------------
	int               id_skeleton_death_fx = 0;

};

class Coin_Properties : public Properties
{
public:
	//------------Valeues-------------------
	//-----------Animations-----------------
	Animation		    anim_coin;
	//-----------Textures-------------------
	SDL_Texture*        tex_coin;
	//------------Sfx----------------------
	int                 pick_up_coin_fx;
};


class Entity
{
public:
	Entity(fPoint position, fPoint spawn_pos, Properties *properties);

	virtual ~Entity();
	// Virtual methods ================================

	virtual bool HandleInput() { return true; };

	virtual bool Update(float dt) { return true; };

	virtual bool Draw() { return true; };

	virtual bool OnCollision(Collider* c1, Collider* c2) { return true; };

	virtual bool Load(pugi::xml_node& node) { return true; };

	virtual bool Save(pugi::xml_node& node) const { return true; };

	virtual bool Reset() { return true; };

	virtual bool Active() { return true; };

	virtual bool Desactive() { return true; };

	// Common methods ==================================

	fPoint GetPosition();

	void  SetPosition(fPoint position);

	Collider * GetMainCollider();

protected:
	// Variables =======================================
	bool                  active = false;
	bool                  spawned = false;
	p2SString             name;
	int                   id = 0;
	fPoint                spawn_pos = {0.0f, 0.0f};
	Properties *          properties = nullptr;
	// Transform ======================================= 
	fPoint                position = { 0.0f, 0.0f };
	fPoint                velocity = { 0.0f, 0.0f };
	fPoint                acceleration = { 0.0f, 0.0f };
	// Colliders =======================================
	Collider *            main_collider = nullptr;

private:
	friend EntityManager;
};

#endif // __Entity_H__
