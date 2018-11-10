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
	// Check camera position to decide what to spawn
	for (p2List_item<Enemy_Info> *item = enemies_info.start ; item ; item = item->next ) 
	{
        // TODO: Spawn enemies when collide with camera
	}
	
	return true;
}

// Called before render is available
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

	for (p2List_item<Enemy*> *item = current_enemies.start; item; item = item->next)
	{
		// TODO: Despawn enemies 
	}

	return true;
}

// Called before quitting
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

bool Enemies::LoadEnemiesInfo(int x, int y, Enemy_Type type)
{
	bool ret = false;
	//enemies_info.add(Enemy_Info());
	return ret;
}

void Enemies::SpawnEnemy(const Enemy_Info& info)
{
	LOG("Enemy Spawned");

	switch (info.type)
	{
	case Enemy_Type::None:
		/*enemies[i] = new Enemy_RedBird(info.x, info.y);*/
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

bool  Enemies::Load(pugi::xml_node&)
{
	return true;
}

bool  Enemies::Save(pugi::xml_node&) const
{
	return true;
}

bool  Enemies::Reset(fPoint pos)
{
	return true;
}