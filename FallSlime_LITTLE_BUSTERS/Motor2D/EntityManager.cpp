#include "j1App.h"
#include "EntityManager.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "Brofiler/Brofiler.h"

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
	BROFILER_CATEGORY("EntityManager Awake", Profiler::Color::GreenYellow);

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

	player_properties->path_tex_skeleton.create(enemy_node.child("textures").child("enemy_skeleton").attribute("path").as_string(""));
	//----------Animations-------------------------
	player_properties->jumping_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("jumping").attribute("path").as_string("")), "pink_slime");
	player_properties->death_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("death").attribute("path").as_string("")), "pink_splash");
	player_properties->attack_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("attack").attribute("path").as_string("")), "pink_attack");

	
	player_properties->skeleton_attack_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("enemy_skeleton").attribute("path").as_string("")), "skeleton_attack");
	player_properties->skeleton_dead_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("enemy_skeleton").attribute("path").as_string("")), "skeleton_dead");
	player_properties->skeleton_walking_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("enemy_skeleton").attribute("path").as_string("")), "skeleton_walking");

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

	for (pugi::xml_node enemy_node = node.child("enemies").child("enemy"); enemy_node; enemy_node = enemy_node.next_sibling("enemy"))
	{

		Enemy_Properties*  enemy_properties = new Enemy_Properties();
		enemy_properties->name.create(enemy_node.attribute("name").as_string(""));

		// Assets ======================

		//----------Textures---------------------------
		enemy_properties->path_tex_bat.create(enemy_node.child("textures").child("enemy_bat").attribute("path").as_string(""));
		enemy_properties->path_tex_smoke.create(enemy_node.child("textures").child("smoke").attribute("path").as_string(""));

		//----------Animations-------------------------
		enemy_properties->bat_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("enemy_bat").attribute("path").as_string("")), "enemy_bat");
		enemy_properties->smoke_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("smoke").attribute("path").as_string("")), "smoke");

		pugi::xml_node collider_node = enemy_node.child("collider");
		enemy_properties->collider_rect = { 0 , 0 , collider_node.attribute("width").as_int(0) , collider_node.attribute("height").as_int(0) };

		pugi::xml_node spawn_margin = enemy_node.child("spawn_margin");
		enemy_properties->spawn_rect = { 0 , 0 , spawn_margin.attribute("width").as_int(0) , spawn_margin.attribute("height").as_int(0) };

		properties_list.add(enemy_properties);
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
	BROFILER_CATEGORY("EntityManager CleanUp", Profiler::Color::HoneyDew);

	LOG("Freeing all entities info");
	// Remove all entities info =================================
	entities_info.clear();

	LOG("Freeing all Properties");
	// Remove all properties  ==================================
	p2List_item<Properties*>* item = properties_list.start;
	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	properties_list.clear();

	// Remove all entities =====================================
	LOG("Freeing all entities");

	p2List_item<Entity*>* item_2 = entities.start;

	while (item_2 != NULL)
	{
		RELEASE(item_2->data);
		item_2 = item_2->next;
	}
	
	entities.clear();

	return true;
}

bool EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("EntityManager PreUpdate", Profiler::Color::HotPink);

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
	BROFILER_CATEGORY("EntityManager Update", Profiler::Color::Lavender);

	for (p2List_item<Entity*> *item = entities.start; item ; item = item->next)
	{
		item->data->Update(dt);
	}

	return true;
}

bool EntityManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("EntityManager PostUpdate", Profiler::Color::LavenderBlush);

	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		item->data->Draw();
	}

	return true;
}

bool EntityManager::LoadEntitiesInfo(pugi::xml_node& node)
{
	BROFILER_CATEGORY("EntityManager LoadEntitiesInfo", Profiler::Color::LawnGreen);

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
	BROFILER_CATEGORY("EntityManager CreateEntity", Profiler::Color::LemonChiffon);

	Entity* entity = nullptr;

	if (info.name == "test") 
	{
		entity = new Enemy_Bat( info.position, info);
	}

	if (entity != nullptr) 
	{
		LOG("Entity %s created", info.name.GetString());
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
	BROFILER_CATEGORY("EntityManager CreatePlayer", Profiler::Color::LightBlue);

    LOG("Entity Player created");

	Entity* entity = nullptr;

	if (player == nullptr)
	{
		Entity_Info info( spawn_pos, GetProperties(p2SString("player")) );
		entity = player = new j1Player( info.position, info);
		entities.add(entity);
	}
	
	return true;
}

bool EntityManager::CreateEnemy(fPoint spawn_pos)
{
	BROFILER_CATEGORY("EntityManager CreateEnemy", Profiler::Color::LightBlue);

	LOG("Entity Enemy created");

	Entity* entity = nullptr;

	if (player == nullptr)
	{
		Entity_Info info(spawn_pos, GetProperties(p2SString("enemy")));
		entity = player = new j1Player(info.position, info);
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
	pugi::xml_node node_entity;

	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		node_entity = node.child(item->data->name.GetString());
		item->data->Load(node_entity);
	}
	return true;
}

bool EntityManager::Save(pugi::xml_node& node) const
{
	pugi::xml_node node_entity;

	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		node_entity = node.append_child(item->data->name.GetString());
		item->data->Save(node_entity);
	}
	return true;
}

bool EntityManager::ResetAll( )
{
	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		item->data->Reset();
	}
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

	LOG("Property %s not found", name.GetString());
	return nullptr;
}

j1Player* EntityManager::GetPlayer()
{
	return player;
}

