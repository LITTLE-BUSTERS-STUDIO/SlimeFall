#include "j1App.h"
#include "EntityManager.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"

// Entities headers ========================
#include "Entity.h"
#include "Enemy_Test.h"

EntityManager::EntityManager()
{
	name.create("entities");
}

EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(pugi::xml_node& node)
{

	for (pugi::xml_node enemy_node = node.child("enemies").child("enemy") ; enemy_node; enemy_node = enemy_node.next_sibling("enemy"))
	{
		Properties properties;
		properties.name.create(enemy_node.attribute("name").as_string(""));

		pugi::xml_node collider_node = enemy_node.child("collider");
		properties.collider_rect = { 0 , 0 , collider_node.attribute("width").as_int(0) , collider_node.attribute("height").as_int(0)};

		pugi::xml_node spawn_margin = enemy_node.child("spawn_margin");
		properties.spawn_rect = { 0 , 0 , spawn_margin.attribute("width").as_int(0) , spawn_margin.attribute("height").as_int(0)};

		enemies_properties.add(properties);
	}
	return true;
}

bool EntityManager::Start()
{
	LOG("Loading Entity Manager");
	/*sprites = App->tex->Load("textures/enemies.png");*/
	return true;
}

bool EntityManager::CleanUp()
{
	LOG("Freeing all entities");

	/*App->tex->UnLoad(sprites);*/

	// Remove all enemies
	p2List_item<Entity*>* item = entities.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	entities.clear();

	// Remove all enemies info
	entities.clear();

	return true;
}

bool EntityManager::PreUpdate()
{
	SDL_Rect camera = App->render->camera;
	int scale = App->win->GetScale();

	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		// TODO: Despawn enemies
	}

	for (p2List_item<Entity_Info> *item = entities_info.start ; item ; item = item->next ) 
	{
        // TODO: Spawn enemies when collide with camera
		if (item->data.spawned)
		{
			continue;
		}

		int x = item->data.position.x;
		int y = item->data.position.y;
		int w = item->data.spawn_rect.w;
		int h = item->data.spawn_rect.h;

		if (((camera.x / scale < x + w) && (x < (camera.x + camera.w) / scale)
			&& (camera.y / scale < y + h) && (y < (camera.y + camera.h) / scale)))
		{
			item->data.spawned = true;
			SpawnEnemy(item->data);
		}
	}
	return true;
}

bool EntityManager::Update(float dt)
{
	for (p2List_item<Entity*> *item = entities.start; item ; item = item->next)
	{
		item->data->Update(dt);
	}

	return true;
}

bool EntityManager::PostUpdate()
{
	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		item->data->Draw();
	}

	return true;
}

bool EntityManager::LoadEnemiesInfo(pugi::xml_node& node)
{
	bool ret = true;
	
	for (pugi::xml_node object = node.child("object"); object; object = object.next_sibling("object"))
	{
		// Read information ------------------------------
		p2SString name(object.attribute("name").as_string());
		Properties properties = GetProperties(name);
		entities_info.add(Entity_Info( iPoint (object.attribute("x").as_int(0), object.attribute("y").as_int(0)), properties));
	}

	return ret;
}

bool EntityManager::SpawnEnemy(const Entity_Info& info)
{
	LOG("Enemy Spawned");
	Entity* enemy = nullptr;

	if (info.name == "test") 
	{
		/*enemy = new Enemy_Test( info.position, info.collider_rect);*/
	}
	
	entities.add(enemy);

	return true;
}

bool EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		if (item->data->GetCollider() == c1)
		{
			item->data->OnCollision(c2);
		}
	}

	return true;
}

bool EntityManager::Load(pugi::xml_node& node)
{
	return true;
}

bool EntityManager::Save(pugi::xml_node& node) const
{
	return true;
}

bool EntityManager::Reset(fPoint pos)
{
	return true;
}

Properties EntityManager::GetProperties(p2SString name) const 
{
	for (p2List_item<Properties> *item = enemies_properties.start; item; item = item->next)
	{
		if (item->data.name == name)
		{
			return item->data;
		}
	}

	return Properties();
}