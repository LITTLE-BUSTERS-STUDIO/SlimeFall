#ifndef __Enemies_H__
#define __Enemies_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "SDL/include/SDL.h"


class  Entity;
class  Entity_Info;
class  Properties;

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

	bool PostUpdate(float dt);

	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	bool OnCollision(Collider*, Collider*);

	bool Reset(fPoint pos);

	bool CreatePlayer(fPoint spawn_pos);
	bool CreateEnemy(fPoint spawn_pos);
	//bool ResetAll();

	//bool ResetEntity (Entity * entity);

	// Player =========================================

	j1Player* GetPlayer();

	// Enemies ========================================

	bool LoadEntitiesInfo(pugi::xml_node& node);

private:

	bool CreateEntity(const Entity_Info& info);

	Properties* GetProperties(const p2SString name) const;

private:
	// Entities ===========================================
	p2List<Entity_Info>         entities_info;          // Entities to spawn added in LoadMap (objectgroup entities)
	p2List<Entity*>				entities;				// Active entities
	j1Player*                   player = nullptr;
	//Enemy_Bat*                  enemy_bat = nullptr;

	//// Entities Properties ================================
	p2List<Properties *>        properties_list; 
};

#endif // __Enemies_H__
