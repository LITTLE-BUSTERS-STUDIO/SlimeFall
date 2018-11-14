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

	bool Reset(fPoint pos);

	//bool ResetAll();

	//bool ResetEntity (Entity * entity);

	// Player =========================================


	// Enemies ========================================

	bool LoadEnemiesInfo(pugi::xml_node& node);

private:

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

#endif // __Enemies_H__
