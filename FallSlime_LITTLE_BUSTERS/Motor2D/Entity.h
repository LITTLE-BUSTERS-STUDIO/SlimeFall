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
	~Properties() {}

	p2SString            name;
	SDL_Rect             collider_rect;
	SDL_Rect             spawn_rect;
};

class Player_Properties: public Properties
{
public:

	//------------Valeues-------------------
	float			  gravity;
	float			  speed_ground;
	float			  speed_air;
	float			  speed_jump;
	float			  speed_gummy_jump;
	float             speed_attack;
	//-----------Animations-----------------
	p2SString         path_jumping_anim;
	p2SString         path_death_anim;
	p2SString         path_attack_anim;;

	//-----------Textures-------------------
	p2SString         path_tex_player;
	p2SString         path_death_splash;
	p2SString         path_attack_splash;

	//------------Sfx----------------------
	p2SString         path_jump_fx1;
	p2SString         path_jump_fx2;
	p2SString         path_jump_fx3;
	p2SString         path_jump_fx4;
	p2SString         path_jump_fx5;
	p2SString         path_death_fx;
	p2SString         path_attack_fx;

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


enum class EntityType 
{
	Player,
	Enemy,
	Object,
	Unknown
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

	// Save & Load =====================================
	virtual bool Load(pugi::xml_node&) { return true; };

	virtual bool Save(pugi::xml_node&) const { return true; };

	// Common methods ==================================

	bool FindCollider(Collider *collider) const;

	p2SString             name;
	fPoint                position;
	p2List<Collider*>     colliders;
	EntityType            type = EntityType::Unknown;
};

#endif // __Entity_H__
