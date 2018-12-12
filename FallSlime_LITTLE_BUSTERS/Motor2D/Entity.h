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
	SDL_Rect             collider_rect;
	SDL_Rect             spawn_rect;
};

class Player_Properties : public Properties
{
public:
	//------------Valeues-------------------
	float			    gravity;
	float			    speed_ground;
	float			    speed_air;
	float			    speed_jump;
	float			    speed_gummy_jump;
	float               speed_attack;

	//-----------Animations-----------------
	Animation           jumping_anim;
	Animation		    death_anim;
	Animation		    attack_anim;

	//-----------Textures-------------------
	SDL_Texture*        player_tex;
	SDL_Texture*        death_tex;
	SDL_Texture*        attack_tex;
	
	//------------Sfx----------------------
	int                 id_jump_fx1;
	int                 id_jump_fx2;
	int                 id_jump_fx3;
	int                 id_jump_fx4;
	int                 id_jump_fx5;
	int                 id_death_fx;
	int                 id_attack_fx;
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
	int               id_skeleton_death_fx;

};

class Entity;

class Entity_Info
{
public:
	p2SString            name;
	int                  id;
	fPoint               position;
	Properties*          properties = nullptr;
	SDL_Rect             spawn_rect;
	Entity              *entity = nullptr;
	bool                 spawned = false;

	Entity_Info() {}

	Entity_Info(fPoint spawn_position, Properties * properties)
	{
		position = spawn_position;
		name = properties->name;
		this->properties = properties;

		spawn_rect = properties->spawn_rect;
	    spawn_rect.x = spawn_position.x - properties->spawn_rect.w / 2;
		spawn_rect.y = spawn_position.y - properties->spawn_rect.h / 2;
	}
};

class Entity
{
public:
	Entity(fPoint position, Entity_Info info);

	virtual ~Entity();
	// Virtual methods ================================

	virtual bool HandleInput() { return true; };

	virtual bool Update(float dt) { return true; };

	virtual bool Draw() { return true; };

	virtual bool OnCollision(Collider* c1, Collider* c2) { return true; };

	virtual bool Load(pugi::xml_node& node) { return true; };

	virtual bool Save(pugi::xml_node& node) const { return true; };

	virtual bool Reset(Entity_Info  info) { return true; };

	virtual bool Active() { return true; };

	virtual bool Desactive() { return true; };

	// Common methods ==================================

	bool FindCollider(Collider *collider) const;

	// Variables ======================================
	p2SString             name;
	int                   id;
	bool                  active = false;

	fPoint                position;
	fPoint                velocity;
	fPoint                acceleration;

	p2List<Collider*>     colliders;
};

#endif // __Entity_H__
