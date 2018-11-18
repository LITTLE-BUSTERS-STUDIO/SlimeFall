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

};

class Enemy_Skeleton_Properties : public Enemy_Properties
{
public:
	//-----------Animations-----------------
	Animation         skeleton_attack_anim;
	Animation         skeleton_dead_anim;
	Animation         skeleton_walking_anim;
	//-----------Textures-----------------
	SDL_Texture*      skeleton_tex;
};















class Entity_Info
{
public:
	p2SString            name;
	fPoint               position;
	Properties*          properties = nullptr;
	SDL_Rect             spawn_rect;
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

	virtual bool Load(pugi::xml_node&) { return true; };

	virtual bool Save(pugi::xml_node&) const { return true; };

	virtual bool Reset() { return true; };

	// Common methods ==================================

	bool FindCollider(Collider *collider) const;

	// Variables ======================================
	p2SString             name;
	int                   id;
	bool                  active = true;
	Entity_Info          *info;

	fPoint                position;
	fPoint                velocity;
	fPoint                acceleration;

	p2List<Collider*>     colliders;
};

#endif // __Entity_H__
