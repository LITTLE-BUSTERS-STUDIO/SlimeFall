#ifndef __Enemies_H__
#define __Enemies_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "SDL/include/SDL.h"

// Enemies Headers =======================
class  Entity;
class  Entity_Info;

class Properties
{
public:
	Properties() {}
	~Properties() {}

	p2SString            name;
	SDL_Rect             collider_rect;
	SDL_Rect             spawn_rect;
};

class Properties;

class  EntityManager : public j1Module
{
public:
	// Manager =======================================
	EntityManager();

	virtual ~EntityManager();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	bool OnCollision(Collider*, Collider*);

	// Player =========================================


	// Enemies ========================================

	bool LoadEnemiesInfo(pugi::xml_node& node);

	bool Reset(fPoint pos);



//private:

	bool SpawnEnemy(const Entity_Info& info);

	Properties GetProperties(const p2SString name) const;

private:
	// Entities ===========================================
	p2List<Entity_Info>         entities_info;          // Entities to spawn added in LoadMap (objectgroup entities)
	p2List<Entity*>				entities;				// Active entities

	//// Entities Properties ================================
	p2List<Properties>          enemies_properties; 
	//// Player ----------------------------------------
	//Player_Properties           player_properties;
	//// Enemies ----------------------------------------
	//p2List<Enemy_Properties>    enemies_properties;    // Added in Awake (config.xml)
	//// Objects ----------------------------------------
	//p2List<Object_Properties>   objects_properties;

};

class Properties;

class Entity_Info
{
public:
	iPoint               position;
	p2SString            name;
	SDL_Rect             collider_rect;
	SDL_Rect             spawn_rect;
	bool                 spawned = false;

	Entity_Info() {}
	Entity_Info(iPoint spawn_position, Properties properties)
	{
		position = spawn_position;
		name = properties.name;
		spawn_rect = properties.spawn_rect;
		spawn_rect.x = spawn_position.x - spawn_rect.w / 2;
		spawn_rect.y = spawn_position.y - spawn_rect.h / 2;

		collider_rect = properties.collider_rect;
		collider_rect.x = spawn_position.x - collider_rect.w / 2;
		collider_rect.y = spawn_position.y - collider_rect.h / 2;
	}
};



//class Enemy_Properties : public Properties
//{
//
//};
//
//class Player_Properties : public Properties
//{
//
//};
//
//class Object_Properties : public Properties
//{
//
//};




#endif // __Enemies_H__
