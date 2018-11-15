#include "j1App.h"
#include "EntityManager.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"

// Entities Headers =======================
#include "Entity.h"
#include "j1Player.h"
#include "Enemy_Test.h"
// ========================================

EntityManager::EntityManager()
{
	name.create("entities");
}

EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(pugi::xml_node& node)
{
	// Load properties =====================================================

	// Players ///////////////////////////////////////////
	pugi::xml_node enemy_node = node.child("players").child("player");
	
		Player_Properties*  player_properties = new Player_Properties();

		player_properties->name.create(enemy_node.attribute("name").as_string(""));

		pugi::xml_node collider_node = enemy_node.child("collider");
		player_properties->collider_rect = { 0 , 0 , collider_node.attribute("width").as_int(0) , collider_node.attribute("height").as_int(0) };

		pugi::xml_node spawn_margin = enemy_node.child("spawn_margin");
		player_properties->spawn_rect = { 0 , 0 , spawn_margin.attribute("width").as_int(0) , spawn_margin.attribute("height").as_int(0) };

		// Values ======================

		player_properties->gravity = enemy_node.child("physics").attribute("gravity").as_float(0);
		player_properties->speed_ground = enemy_node.child("physics").attribute("speed_ground").as_float(0);
		player_properties->speed_air = enemy_node.child("physics").attribute("speed_air").as_float(0);
		player_properties->speed_jump = enemy_node.child("physics").attribute("speed_jump").as_float(0);
		player_properties->speed_gummy_jump = enemy_node.child("physics").attribute("speed_gummy_jump").as_float(0);
		player_properties->speed_attack = enemy_node.child("physics").attribute("speed_attack").as_float(0);

		// Assets ======================

		//----------Textures---------------------------
		player_properties->path_tex_player.create(enemy_node.child("textures").child("jumping").attribute("path").as_string(""));
		player_properties->path_death_splash.create(enemy_node.child("textures").child("death").attribute("path").as_string(""));
		player_properties->path_attack_splash.create(enemy_node.child("textures").child("attack").attribute("path").as_string(""));
		//----------Animations-------------------------
		player_properties->path_jumping_anim.create(enemy_node.child("animations").child("jumping").attribute("path").as_string(""));
		player_properties->path_death_anim.create(enemy_node.child("animations").child("death").attribute("path").as_string(""));
		player_properties->path_attack_anim.create(enemy_node.child("animations").child("attack").attribute("path").as_string(""));

		//-----------------Sfx-----------------------
		player_properties->path_jump_fx1.create(enemy_node.child("sfx").child("jump1").attribute("path").as_string(""));
		player_properties->path_jump_fx2.create(enemy_node.child("sfx").child("jump2").attribute("path").as_string(""));
		player_properties->path_jump_fx3.create(enemy_node.child("sfx").child("jump3").attribute("path").as_string(""));
		player_properties->path_jump_fx4.create(enemy_node.child("sfx").child("jump4").attribute("path").as_string(""));
		player_properties->path_jump_fx5.create(enemy_node.child("sfx").child("jump5").attribute("path").as_string(""));
		player_properties->path_death_fx.create(enemy_node.child("sfx").child("death").attribute("path").as_string(""));
		player_properties->path_attack_fx.create(enemy_node.child("sfx").child("attack").attribute("path").as_string(""));

		//=================================================================

		properties_list.add(player_properties);
	
	// Enemies //////////////////////////////////////////////

	for (pugi::xml_node enemy_node = node.child("enemies").child("enemy") ; enemy_node; enemy_node = enemy_node.next_sibling("enemy"))
	{
		Properties* properties = new Properties();
		properties->name.create(enemy_node.attribute("name").as_string(""));

		pugi::xml_node collider_node = enemy_node.child("collider");
		properties->collider_rect = { 0 , 0 , collider_node.attribute("width").as_int(0) , collider_node.attribute("height").as_int(0)};

		pugi::xml_node spawn_margin = enemy_node.child("spawn_margin");
		properties->spawn_rect = { 0 , 0 , spawn_margin.attribute("width").as_int(0) , spawn_margin.attribute("height").as_int(0)};

		properties_list.add(properties);
	}
	// Objects ///////////////////////////////////////////////

	return true;
}

bool EntityManager::Start()
{
	LOG("Loading Entity Manager");

	return true;
}

bool EntityManager::CleanUp()
{
	LOG("Freeing all entities");

	// Remove all entities ======================================
	p2List_item<Entity*>* item = entities.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	entities.clear();

	// Remove all entities info =================================
	entities.clear();

	return true;
}

bool EntityManager::PreUpdate()
{
	SDL_Rect camera = App->render->camera;
	int scale = App->win->GetScale();

	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		// TODO: Despawn entities
	}

	for (p2List_item<Entity_Info> *item = entities_info.start ; item ; item = item->next ) 
	{
        // TODO: Spawn enttities 

		if (item->data.spawned)
		{
			continue;
		}

		SDL_Rect rect = item->data.properties->spawn_rect;

		if (((camera.x / scale < rect.x + rect.w) && (rect.x < (camera.x + camera.w) / scale)
			&& (camera.y / scale < rect.y + rect.h) && (rect.y < (camera.y + camera.h) / scale)))
		{
			item->data.spawned = true;
			CreateEntity(item->data);
		}
	}

	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		item->data->HandleInput();
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

bool EntityManager::PostUpdate(float dt)
{
	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		item->data->Draw();
	}

	return true;
}

bool EntityManager::LoadEntitiesInfo(pugi::xml_node& node)
{
	bool ret = true;
	
	for (pugi::xml_node object = node.child("object"); object; object = object.next_sibling("object"))
	{
		// Read information ------------------------------
		p2SString name(object.attribute("name").as_string());
		entities_info.add( Entity_Info( fPoint (object.attribute("x").as_int(0), object.attribute("y").as_int(0)), GetProperties(name) ) );
	}

	return ret;
}

bool EntityManager::CreateEntity(const Entity_Info& info)
{
	Entity* entity = nullptr;

	if (info.name == "test") 
	{
		entity = new Enemy_Test( info.position, info);
	}

	if (entity != nullptr) 
	{
		LOG("Entity created");
	}
	else 
	{
		LOG("Entity could't be created");
		return false;
	}
		
	entities.add(entity);

	return true;
}

bool EntityManager::CreatePlayer(fPoint spawn_pos)
{
	LOG("Enemy Spawned");
	Entity* entity = nullptr;

	if (player == nullptr)
	{
		Entity_Info info( spawn_pos, GetProperties(p2SString("player")) );
		entity = player = new j1Player( info.position, info);
		entities.add(entity);
	}
	
	return true;
}



bool EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		if (item->data->FindCollider(c1))
		{
			item->data->OnCollision(c1, c2);
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

Properties* EntityManager::GetProperties(p2SString name) const 
{
	for (p2List_item<Properties*> *item = properties_list.start; item && item->data; item = item->next)
	{
		if (item->data->name == name)
		{
			return item->data;
		}
	}

	return nullptr;
}

j1Player* EntityManager::GetPlayer()
{
	return player;
}