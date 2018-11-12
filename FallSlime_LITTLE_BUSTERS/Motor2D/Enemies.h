#ifndef __Enemies_H__
#define __Enemies_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

class Enemy;
struct Enemy_Info;
struct Enemy_Properties;

class Enemies : public j1Module
{
public:

	Enemies();

	virtual ~Enemies();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	bool Reset(fPoint pos);

	bool OnCollision(Collider*, Collider*);

	bool LoadEnemiesInfo(pugi::xml_node& node);

//private:

	bool SpawnEnemy(const Enemy_Info& info);
	Enemy_Properties GetProperties(const p2SString name) const;


private:
	p2List<Enemy_Properties>   enemies_properties;    // Added in Awake (config.xml)
	p2List<Enemy_Info>         enemies_info;          // Added in LoadMap (objectgroup enemies)
	p2List<Enemy*>             current_enemies;       // Active enemies
	SDL_Texture*               sprites = nullptr;
	
	int despawn_margin;
};
struct Enemy_Properties
{
	p2SString            name;
	SDL_Rect             collider_rect;
	SDL_Rect             spawn_rect;
};

struct Enemy_Info
{
	fPoint               position;
	p2SString            name;
	SDL_Rect             collider_rect;
	SDL_Rect             spawn_rect;
	bool                 spawned = false;

	Enemy_Info() {}
	Enemy_Info(fPoint spawn_position , Enemy_Properties properties)
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

#endif // __Enemies_H__
