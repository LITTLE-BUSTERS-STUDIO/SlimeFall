#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Window.h"
#include "Enemies.h"
#include "j1Textures.h"
#include "Enemy.h"
// Enemies headers ---------------------
#include "Enemy_Test.h"

Enemies::Enemies()
{
	name.create("enemies");
}

Enemies::~Enemies()
{

}

bool Enemies::Awake(pugi::xml_node& node)
{
	for (pugi::xml_node enemy_node = node.child("enemy") ; enemy_node; enemy_node = enemy_node.next_sibling("enemy"))
	{
		Enemy_Properties properties;
		properties.name.create(enemy_node.attribute("name").as_string(""));

		pugi::xml_node collider_node = enemy_node.child("collider");
		properties.collider_rect = { 0 , 0 , collider_node.attribute("width").as_int(0) , collider_node.attribute("height").as_int(0)};

		pugi::xml_node spawn_margin = enemy_node.child("spawn_margin");
		properties.spawn_rect = { 0 , 0 , spawn_margin.attribute("width").as_int(0) , spawn_margin.attribute("height").as_int(0)};

		enemies_properties.add(properties);
	}
	return true;
}

bool Enemies::Start()
{
	LOG("Loading Enemies");
	sprites = App->tex->Load("textures/enemies.png");
	return true;
}

bool Enemies::PreUpdate()
{
	SDL_Rect camera = App->render->camera;
	int scale = App->win->GetScale();

	for (p2List_item<Enemy*> *item = current_enemies.start; item; item = item->next)
	{
		// TODO: Despawn enemies 
	}

	for (p2List_item<Enemy_Info> *item = enemies_info.start ; item ; item = item->next ) 
	{
        // TODO: Spawn enemies when collide with camera
		int x = item->data.position.x;
		int y = item->data.position.y;
		int w = item->data.spawn_rect.w;
		int h = item->data.spawn_rect.h;

		if (item->data.spawned)
		{
			continue;
		}
		else if (((camera.x / scale < x + w) && (x < (camera.x + camera.w) / scale)
			&& (camera.y / scale < y + h) && (y < (camera.y + camera.h) / scale)))
		{
			item->data.spawned = true;
			SpawnEnemy(item->data);
		}

	}
	
	return true;
}

bool Enemies::Update(float dt)
{
	for (p2List_item<Enemy*> *item = current_enemies.start; item; item = item->next)
	{
		item->data->Move(dt);
	}

	return true;
}

bool Enemies::PostUpdate()
{
	for (p2List_item<Enemy*> *item = current_enemies.start; item; item = item->next)
	{
		item->data->Draw(sprites);
	}

	return true;
}

bool Enemies::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(sprites);

	// Remove all enemies
	p2List_item<Enemy*>* item = current_enemies.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	current_enemies.clear();

	// Remove all enemies info
	current_enemies.clear();

	return true;
}

bool Enemies::LoadEnemiesInfo(pugi::xml_node& node)
{
	bool ret = true;
	
	for (pugi::xml_node object = node.child("object"); object; object = object.next_sibling("object"))
	{
		// Read information ------------------------------
		p2SString name(object.attribute("name").as_string());
		Enemy_Properties properties = GetProperties(name);
		enemies_info.add(Enemy_Info( fPoint (object.attribute("x").as_int(0), object.attribute("y").as_int(0)), properties));
	}
	return ret;
}

bool Enemies::SpawnEnemy(const Enemy_Info& info)
{
	LOG("Enemy Spawned");
	Enemy* enemy = nullptr;

	if (info.name == "test") 
	{
		enemy = new Enemy_Test( info.position, info.collider_rect);
	}
	
	current_enemies.add(enemy);

	return true;
}

bool Enemies::OnCollision(Collider* c1, Collider* c2)
{
	for (p2List_item<Enemy*> *item = current_enemies.start; item; item = item->next)
	{
		if (item->data->GetCollider() == c1)
		{
			item->data->OnCollision(c2);
		}
	}

	return true;
}

bool Enemies::Load(pugi::xml_node& node)
{
	return true;
}

bool Enemies::Save(pugi::xml_node& node) const
{
	return true;
}

bool Enemies::Reset(fPoint pos)
{
	return true;
}

Enemy_Properties Enemies::GetProperties(p2SString name) const 
{
	for (p2List_item<Enemy_Properties> *item = enemies_properties.start; item; item = item->next)
	{
		if (item->data.name == name)
		{
			return item->data;
		}
	}

	return Enemy_Properties();
}