#include "j1App.h"
#include "EntityManager.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "Brofiler/Brofiler.h"

// Entities Headers =======================
#include "Entity.h"
#include "j1Player.h"
#include "Enemy_Bat.h"
#include "Enemy_Skeleton.h"
// ========================================

EntityManager::EntityManager()
{
	name.create("entity_manager");
}

EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(pugi::xml_node& node)
{
	BROFILER_CATEGORY("EntityManager Awake", Profiler::Color::GreenYellow);
	
	

	return true;
}

bool EntityManager::Start()
{
	BROFILER_CATEGORY("EntityManager Start", Profiler::Color::GreenYellow);

	LOG("Loading Entity Manager");

	pugi::xml_document doc;
	doc.load_file("data/entities.xml");
	pugi::xml_node node = doc.child("entities");

	// ===========================================================================================
	// -------------------------------------- Player ---------------------------------------------
	// ===========================================================================================


	Player_Properties*  player_properties = new Player_Properties();

	pugi::xml_node player_node = node.child("player");
	player_properties->name.create(player_node.attribute("name").as_string(""));

	pugi::xml_node collider_node = player_node.child("collider");
	player_properties->collider_rect = { 0 , 0 , collider_node.attribute("width").as_int(0) , collider_node.attribute("height").as_int(0) };

	pugi::xml_node spawn_margin_node = player_node.child("spawn_margin");
	player_properties->spawn_rect = { 0 , 0 , spawn_margin_node.attribute("width").as_int(0) , spawn_margin_node.attribute("height").as_int(0) };

	//============== Variables ===================
	pugi::xml_node variables_node = player_node.child("variables");

	player_properties->gravity = variables_node.attribute("gravity").as_float(0);
	player_properties->speed_ground = variables_node.attribute("speed_ground").as_float(0);
	player_properties->speed_air = variables_node.attribute("speed_air").as_float(0);
	player_properties->speed_jump = variables_node.attribute("speed_jump").as_float(0);
	player_properties->speed_gummy_jump = variables_node.attribute("speed_gummy_jump").as_float(0);
	player_properties->speed_attack = variables_node.attribute("speed_attack").as_float(0);

	//============== Textures ===================
	pugi::xml_node textures_node = player_node.child("textures");

	player_properties->player_tex = App->tex->Load(textures_node.child("jumping").attribute("path").as_string(""));
	player_properties->death_tex = App->tex->Load(textures_node.child("death").attribute("path").as_string(""));
	player_properties->attack_tex = App->tex->Load(textures_node.child("attack").attribute("path").as_string(""));

	//============== Animations =================
	pugi::xml_node animations_node = player_node.child("animations");

	player_properties->jumping_anim.LoadAnimation(p2SString(animations_node.child("jumping").attribute("path").as_string("")), "pink_slime");
	player_properties->death_anim.LoadAnimation(p2SString(animations_node.child("death").attribute("path").as_string("")), "pink_splash");
	player_properties->attack_anim.LoadAnimation(p2SString(animations_node.child("attack").attribute("path").as_string("")), "pink_attack");

	//=============== Sfx ======================
	pugi::xml_node sfx_node = player_node.child("sfx");

	player_properties->id_jump_fx1 = App->audio->LoadFx(sfx_node.child("jump1").attribute("path").as_string(""));
	player_properties->id_jump_fx2 = App->audio->LoadFx(sfx_node.child("jump2").attribute("path").as_string(""));
	player_properties->id_jump_fx3 = App->audio->LoadFx(sfx_node.child("jump3").attribute("path").as_string(""));
	player_properties->id_jump_fx4 = App->audio->LoadFx(sfx_node.child("jump4").attribute("path").as_string(""));
	player_properties->id_jump_fx5 = App->audio->LoadFx(sfx_node.child("jump5").attribute("path").as_string(""));
	player_properties->id_death_fx = App->audio->LoadFx(sfx_node.child("death").attribute("path").as_string(""));
	player_properties->id_attack_fx = App->audio->LoadFx(sfx_node.child("attack").attribute("path").as_string(""));

	properties_list.add(player_properties);

	// ===========================================================================================
	// -------------------------------------- Enemies ---------------------------------------------
	// ===========================================================================================

	for (pugi::xml_node enemy_node = node.child("enemy"); enemy_node; enemy_node = enemy_node.next_sibling("enemy"))
	{
		Enemy_Properties*  enemy_properties = nullptr;

		p2SString name(enemy_node.attribute("name").as_string(""));

		collider_node = enemy_node.child("collider");
		spawn_margin_node = enemy_node.child("spawn_margin");
		variables_node = enemy_node.child("variables");
		textures_node = enemy_node.child("textures");
		animations_node = enemy_node.child("animations");
		sfx_node = enemy_node.child("sfx");

		if (name == "bat")
		{
			enemy_properties = new Enemy_Bat_Properties();
			Enemy_Bat_Properties * properties = (Enemy_Bat_Properties*)enemy_properties;

			//============== Variables ===================

			//============== Textures ===================
			properties->bat_tex = App->tex->Load(textures_node.child("enemy_bat").attribute("path").as_string(""));
			properties->smoke_tex = App->tex->Load(textures_node.child("smoke").attribute("path").as_string(""));

			//============== Animations =================
			properties->bat_anim.LoadAnimation(p2SString(animations_node.child("enemy_bat").attribute("path").as_string("")), "enemy_bat");
			properties->smoke_anim.LoadAnimation(p2SString(animations_node.child("smoke").attribute("path").as_string("")), "smoke");

			//=============== Sfx ======================
			properties->id_bat_death_fx = App->audio->LoadFx(sfx_node.child("death").attribute("path").as_string(""));
		}

		else if (name == "skeleton")
		{
			enemy_properties = new Enemy_Skeleton_Properties();
			Enemy_Skeleton_Properties * properties = (Enemy_Skeleton_Properties*)enemy_properties;

			//============== Variables ===================

			//============== Textures ===================
			properties->skeleton_tex = App->tex->Load(textures_node.child("enemy_skeleton").attribute("path").as_string(""));
			//============== Animations =================
			properties->skeleton_attack_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("enemy_skeleton").attribute("path").as_string("")), "skeleton_attack");
			properties->skeleton_dead_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("enemy_skeleton").attribute("path").as_string("")), "skeleton_dead");
			properties->skeleton_walking_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("enemy_skeleton").attribute("path").as_string("")), "skeleton_walking");
			properties->skeleton_idle_anim.LoadAnimation(p2SString(enemy_node.child("animations").child("enemy_skeleton").attribute("path").as_string("")), "skeleton_idle");
			//=============== Sfx ======================
			properties->id_skeleton_death_fx = App->audio->LoadFx(sfx_node.child("death").attribute("path").as_string(""));

		}

		if (enemy_properties == nullptr)
		{
			LOG("Enemie %s couldn't be loaded", name.GetString());
			return false;
		}

		//================================= Common atributes ==========================================
		enemy_properties->name = name;
		enemy_properties->collider_rect = { 0 , 0 , collider_node.attribute("width").as_int(0) , collider_node.attribute("height").as_int(0) };
		enemy_properties->spawn_rect = { 0 , 0 , spawn_margin_node.attribute("width").as_int(0) , collider_node.attribute("height").as_int(0) };
		//============== Variables ===================
		enemy_properties->detection_ratio = variables_node.attribute("detection_ratio").as_int(0);
		enemy_properties->path_interval_time = variables_node.attribute("path_interval_time").as_uint(0);

		properties_list.add(enemy_properties);
	}

	// ===========================================================================================
	// -------------------------------------- Coin ---------------------------------------------
	// ===========================================================================================

	Coin_Properties*  coin_properties = new Coin_Properties();

	pugi::xml_node coin_node = node.child("coin");
	coin_properties->name.create(coin_node.attribute("name").as_string(""));

	pugi::xml_node collider_node = coin_node.child("collider");
	coin_properties->collider_rect = { 0 , 0 , collider_node.attribute("width").as_int(0) , collider_node.attribute("height").as_int(0) };

	pugi::xml_node spawn_margin_node = coin_node.child("spawn_margin");
	coin_properties->spawn_rect = { 0 , 0 , spawn_margin_node.attribute("width").as_int(0) , spawn_margin_node.attribute("height").as_int(0) };

	//============== Variables ===================
	//============== Textures ===================
	pugi::xml_node textures_node = coin_node.child("textures");

	coin_properties->tex_coin = App->tex->Load(textures_node.child("tex_coin").attribute("path").as_string(""));


	//============== Animations =================
	pugi::xml_node animations_node = coin_node.child("animations");

	coin_properties->anim_coin.LoadAnimation(p2SString(animations_node.child("anim_coin").attribute("path").as_string("")), "pink_slime");


	//=============== Sfx ======================
	pugi::xml_node sfx_node = coin_node.child("sfx");

	coin_properties->pick_up_coin_fx = App->audio->LoadFx(sfx_node.child("pick_up").attribute("path").as_string(""));

	properties_list.add(coin_properties);
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

	LOG("Freeing all entities");
	// Remove all entities =====================================
	p2List_item<Entity*>* entities_item = entities.start;
	
	while (entities_item != NULL)
	{
		entities_item->data->colliders.clear();
		RELEASE(entities_item->data);
		entities_item = entities_item->next;
	}
	entities.clear();

	return true;
}

bool EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("EntityManager PreUpdate", Profiler::Color::HotPink);

	SDL_Rect camera = App->render->camera;
	int scale = App->win->GetScale();

	// Spawn entities ===============================

	for (p2List_item<Entity_Info> *item = entities_info.start ; item; item = item->next )
	{
		if (item->data.entity == nullptr || item->data.entity->active || item->data.spawned)
		{
			continue;
		}

		fPoint pos = item->data.position;

		if (((camera.x / scale < pos.x) && (pos.x < (camera.x + camera.w) / scale)
			&& (camera.y / scale < pos.y ) && (pos.y < (camera.y + camera.h) / scale)))
		{
			item->data.spawned = true;
			item->data.entity->Active();
		}
	}


	for (p2List_item<Entity*> *item = entities.start;  item; item = item->next)
	{
		if (!item->data->active)
		{
			continue;
		}
		item->data->HandleInput();
	}


	return true;
}

bool EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntityManager Update", Profiler::Color::Lavender);

	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		if (!item->data->active)
		{
			continue;
		}
		item->data->Update(dt);
	}

	return true;
}

bool EntityManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("EntityManager PostUpdate", Profiler::Color::LavenderBlush);

	for (p2List_item<Entity*> *item = entities.start;  item ; item = item->next)
	{
		if (!item->data->active)
		{
			continue;
		}
		item->data->Draw();
	}

	return true;
}

bool EntityManager::LoadEntities(pugi::xml_node& node)
{
	BROFILER_CATEGORY("EntityManager LoadEntitiesInfo", Profiler::Color::LawnGreen);

	bool ret = true;
	
	for (pugi::xml_node object = node.child("object"); object; object = object.next_sibling("object"))
	{
		// Read information ------------------------------
		p2SString name(object.attribute("name").as_string());
		Entity_Info info(fPoint(object.attribute("x").as_int(0), object.attribute("y").as_int(0)), GetProperties(name));
		info.entity = CreateEntity(info);
		entities_info.add(info);
	}

	return ret;
}

bool  EntityManager::UnloadEntities()
{
	// Remove all entities info =================================
	
	p2List_item<Entity*>* item = entities.start;

	while (item != nullptr)
	{
		if (item->data->name == "player")
		{
			item = item->next;
			continue;
		}

		for (p2List_item<Collider*>* colliders = item->data->colliders.start; colliders; colliders = colliders->next)
		{
			colliders->data->to_delete = true;
		}
		
		item->data->colliders.clear();

		p2List_item<Entity*>* iterator = item->next;

		RELEASE(item->data);
		entities.del(item);
		item = iterator;
	}

	entities_info.clear();

	return true;
}

Entity* EntityManager::CreateEntity(Entity_Info& info)
{
	BROFILER_CATEGORY("EntityManager CreateEntity", Profiler::Color::LemonChiffon);

	Entity* entity = nullptr;

	if (info.name == "bat") 
	{
		entity = new Enemy_Bat( info.position, info);
	}
	else if (info.name == "skeleton") 
	{
		entity = new Enemy_Skeleton( info.position, info);
	}

	if (entity != nullptr) 
	{
		LOG("Entity %s created at Position  x: %.1f  y: %.1f", info.name.GetString() , info.position.x, info.position.y);
		entities.add(entity);
	}
	else 
	{
		LOG("Entity could't be created");
	}

	return entity;
}

bool EntityManager::CreatePlayer(fPoint spawn_pos)
{
	BROFILER_CATEGORY("EntityManager CreatePlayer", Profiler::Color::LightBlue);

	Entity* entity = nullptr;

	if (player == nullptr)
	{
		Entity_Info info( spawn_pos, GetProperties(p2SString("player")) );
		LOG("Player created at Position x: %.1f  y: %.1f", info.position.x, info.position.y);
		entity = player = new j1Player( info.position, info);
		entity->active = true;
		entities.add(entity);
	}
	
	return true;
}


bool EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (p2List_item<Entity*> *item = entities.start; item ; item = item->next)
	{
		if (!item->data->active)
		{
			continue;
		}

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

	pugi::xml_node node_info;

	for (p2List_item<Entity_Info> *item = entities_info.start; item; item = item->next)
	{
		node_info = node.append_child("Enemy_Info");

		node_info.append_attribute("name") = item->data.name.GetString();
		node_info.append_attribute("id") = item->data.id;
		node_info.append_attribute("x") = item->data.position.x;
		node_info.append_attribute("x") = item->data.position.y;
		node_info.append_attribute("spawned") = item->data.spawned;
	}


	return true;
}

bool EntityManager::ResetAll()
{
	for (p2List_item<Entity_Info> *item = entities_info.start; item; item = item->next)
	{
		Entity* entity = item->data.entity;
		if (entity == nullptr || entity->name == "player")
		{
			continue;
		}
		item->data.spawned = false;
		entity->Reset(item->data);
		entity->position = item->data.position;
		entity->active = false;
	}
	return true;
}

Properties* EntityManager::GetProperties(p2SString name) const 
{
	for (p2List_item<Properties*> *item = properties_list.start; item; item = item->next)
	{
		if (item->data == nullptr)
		{
			continue;
		}
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

