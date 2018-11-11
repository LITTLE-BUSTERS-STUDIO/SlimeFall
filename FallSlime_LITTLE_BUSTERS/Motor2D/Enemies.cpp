#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Render.h"
#include "Enemies.h"
#include "j1Textures.h"
#include "Enemy.h"

Enemies::Enemies()
{

}

// Destructor
Enemies::~Enemies()
{

}

bool Enemies::Awake(pugi::xml_node&)
{
	return true;
}

bool Enemies::Start()
{
	LOG("Loading Map");
	sprites = App->tex->Load("rtype/enemies.png");
	return true;
}

bool Enemies::PreUpdate()
{

	for (p2List_item<Enemy*> *item = current_enemies.start; item; item = item->next)
	{
		// TODO: Despawn enemies 
	}

	for (p2List_item<Enemy_Info> *item = enemies_info.start ; item ; item = item->next ) 
	{
        // TODO: Spawn enemies when collide with camera
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
		// Read type -------------------------------------
		p2SString type_str(object.attribute("name").as_string());
		Enemy_Type type;

		if (type_str == "test")
		{
			type = Enemy_Type::Test;
		}
		else if (type_str == "test")
		{

		}

		enemies_info.add(Enemy_Info(0, 0, 30, 30, type));
	}
	return ret;
}

void Enemies::SpawnEnemy(const Enemy_Info& info)
{
	LOG("Enemy Spawned");
	Enemy* enemy = nullptr;

	switch (info.type)
	{
	case Enemy_Type::Test:
		enemy = new Enemy();
		break;

	}
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

bool  Enemies::Load(pugi::xml_node& node)
{
	return true;
}

bool  Enemies::Save(pugi::xml_node& node) const
{
	return true;
}

bool  Enemies::Reset(fPoint pos)
{
	return true;
}