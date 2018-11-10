#ifndef __Enemies_H__
#define __Enemies_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

class Enemy;
struct Enemy_Info;

enum class Enemy_Type
{
	None
};

class Enemies : public j1Module
{
public:

	Enemies();

	virtual ~Enemies();

	void Init();

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

	bool LoadEnemiesInfo(int x, int y, Enemy_Type type);

private:

	void SpawnEnemy(const Enemy_Info& info);

private:

	p2List<Enemy_Info>   enemies_info;
	p2List<Enemy*>       current_enemies;
	SDL_Texture*         sprites = nullptr;
	
	int despawn_margin;
};


struct Enemy_Info
{
	iPoint spawn_pos;
	bool spawned = false;
	Enemy_Type type = Enemy_Type::None;
	SDL_Rect spawn_rect;

	Enemy_Info(int x, int y, int w, int h, Enemy_Type type)
	{
		spawn_pos = { x , y };
		spawn_rect = { x - w / 2, y - h / 2 ,w ,h };
		this->type = type;
	}
};

#endif // __Enemies_H__
