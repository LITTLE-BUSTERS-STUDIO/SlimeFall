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
#include "Coin.h"
// ========================================

EntityManager::EntityManager()
{
	name.create("entity_manager");
}

EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("EntityManager Awake", Profiler::Color::GreenYellow);
	
	document_path.create(config.child("document").attribute("path").as_string(""));
	return true;
}

bool EntityManager::Start()
{
	BROFILER_CATEGORY("EntityManager Start", Profiler::Color::GreenYellow);

	LOG("Loading Entity Manager");

	pugi::xml_document doc;
	doc.load_file(document_path.GetString());
	pugi::xml_node node = doc.child("entities");

	pugi::xml_node collider_node;
	pugi::xml_node spawn_margin_node;
	pugi::xml_node variables_node;
	pugi::xml_node textures_node;
	pugi::xml_node animations_node;
	pugi::xml_node sfx_node;

	// Load properties information from entities.xml =====================================================

	for (pugi::xml_node enemy_node = node.child("entity"); enemy_node; enemy_node = enemy_node.next_sibling("entity"))
	{
		Properties*  entity_properties = nullptr;

		p2SString name(enemy_node.attribute("name").as_string(""));

		collider_node = enemy_node.child("collider");
		spawn_margin_node = enemy_node.child("spawn_margin");
		variables_node = enemy_node.child("variables");
		textures_node = enemy_node.child("textures");
		animations_node = enemy_node.child("animations");
		sfx_node = enemy_node.child("sfx");

		if (name == "player")
		{
			entity_properties = new Player_Properties();
			Player_Properties * properties = (Player_Properties *)entity_properties;
			//============== Variables ===================
			properties->gravity = variables_node.attribute("gravity").as_float(0);
			properties->speed_ground = variables_node.attribute("speed_ground").as_float(0);
			properties->speed_air = variables_node.attribute("speed_air").as_float(0);
			properties->speed_jump = variables_node.attribute("speed_jump").as_float(0);
			properties->speed_gummy_jump = variables_node.attribute("speed_gummy_jump").as_float(0);
			properties->speed_attack = variables_node.attribute("speed_attack").as_float(0);

			//============== Textures ===================
			properties->player_tex = App->tex->Load(textures_node.child("jumping").attribute("path").as_string(""));
			properties->death_tex = App->tex->Load(textures_node.child("death").attribute("path").as_string(""));
			properties->attack_tex = App->tex->Load(textures_node.child("attack").attribute("path").as_string(""));

			//============== Animations =================
			properties->jumping_anim.LoadAnimation(p2SString(animations_node.child("jumping").attribute("path").as_string("")), "pink_slime");
			properties->death_anim.LoadAnimation(p2SString(animations_node.child("death").attribute("path").as_string("")), "pink_splash");
			properties->attack_anim.LoadAnimation(p2SString(animations_node.child("attack").attribute("path").as_string("")), "pink_attack");

			//=============== Sfx ======================
			properties->id_jump_fx1 = App->audio->LoadFx(sfx_node.child("jump1").attribute("path").as_string(""));
			properties->id_jump_fx2 = App->audio->LoadFx(sfx_node.child("jump2").attribute("path").as_string(""));
			properties->id_jump_fx3 = App->audio->LoadFx(sfx_node.child("jump3").attribute("path").as_string(""));
			properties->id_jump_fx4 = App->audio->LoadFx(sfx_node.child("jump4").attribute("path").as_string(""));
			properties->id_jump_fx5 = App->audio->LoadFx(sfx_node.child("jump5").attribute("path").as_string(""));
			properties->id_death_fx = App->audio->LoadFx(sfx_node.child("death").attribute("path").as_string(""));
			properties->id_attack_fx = App->audio->LoadFx(sfx_node.child("attack").attribute("path").as_string(""));
		}

		if (name == "bat")
		{
			entity_properties = new Enemy_Bat_Properties();
			Enemy_Bat_Properties * properties = (Enemy_Bat_Properties*)entity_properties;

			//============== Variables ===================
			properties->detection_ratio = variables_node.attribute("detection_ratio").as_int(0);
			properties->path_interval_time = variables_node.attribute("path_interval_time").as_uint(0);
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
			entity_properties = new Enemy_Skeleton_Properties();
			Enemy_Skeleton_Properties * properties = (Enemy_Skeleton_Properties*)entity_properties;

			//============== Variables ===================
			properties->detection_ratio = variables_node.attribute("detection_ratio").as_int(0);
			properties->path_interval_time = variables_node.attribute("path_interval_time").as_uint(0);
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
		else if (name == "coin")
		{
			entity_properties = new Coin_Properties();
			Coin_Properties * properties = (Coin_Properties*)entity_properties;

			//============== Variables ===================

			//============== Textures ===================
			properties->tex_coin = App->tex->Load(textures_node.child("tex_coin").attribute("path").as_string(""));
			//============== Animations =================
			properties->anim_coin.LoadAnimation(p2SString(enemy_node.child("animations").child("anim_coin").attribute("path").as_string("")), "coin");
			//=============== Sfx ======================
			properties->pick_up_coin_fx = App->audio->LoadFx(sfx_node.child("pick_up").attribute("path").as_string(""));
		}

		if (entity_properties == nullptr)
		{
			LOG("Enemie %s couldn't be loaded", name.GetString());
			return false;
		}

		//================================= Common atributes ==========================================
		entity_properties->name = name;
		entity_properties->collider_rect = { 0 , 0 , collider_node.attribute("width").as_int(0) , collider_node.attribute("height").as_int(0) };
		entity_properties->spawn_rect = { 0 , 0 , spawn_margin_node.attribute("width").as_int(0) , collider_node.attribute("height").as_int(0) };

		properties_list.add(entity_properties);
	}

	
	
	return true;
}

bool EntityManager::CleanUp()
{
	BROFILER_CATEGORY("EntityManager CleanUp", Profiler::Color::HoneyDew);

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
	
	while (entities_item != nullptr)
	{
		App->collision->DeleteCollider(entities_item->data->main_collider);
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

	for (p2List_item<Entity*> *item = entities.start ; item != nullptr; item = item->next )
	{
		if (item->data->spawned)
		{
			continue;
		}

		fPoint pos = item->data->position;

		if (((camera.x / scale < pos.x) && (pos.x < (camera.x + camera.w) / scale)
			&& (camera.y / scale < pos.y ) && (pos.y < (camera.y + camera.h) / scale)))
		{
			item->data->spawned = true;
			item->data->Active();
		}
	}


	for (p2List_item<Entity*> *item = entities.start;  item != nullptr; item = item->next)
	{
		if (item->data->active == false)
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

	for (p2List_item<Entity*> *item = entities.start; item != nullptr; item = item->next)
	{
		if (!item->data->active)
		{
			continue;
		}
		item->data->Update(dt);
	}

	return true;
}

bool EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntityManager PostUpdate", Profiler::Color::LavenderBlush);

	for (p2List_item<Entity*> *item = entities.start;  item != nullptr; item = item->next)
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
	entity_count_id = 0;
	
	for (pugi::xml_node object = node.child("object"); object; object = object.next_sibling("object"))
	{
		// Read information ------------------------------
		p2SString name(object.attribute("name").as_string());
		fPoint spawn_pos(object.attribute("x").as_float(0.0f), object.attribute("y").as_float(0.0f));
		Properties * properties = GetProperties(name);

		if (properties == nullptr)
		{
			LOG("Error: Enemy %s Properties not found spawn pos x : %f y: %f" ,name.GetString(), spawn_pos.x, spawn_pos.y);
			continue;
		}
		CreateEntity(name, spawn_pos, spawn_pos, properties);
	}

	return ret;
}

bool  EntityManager::UnloadEntities()
{
	// Remove all entities info =================================
	
	p2List_item<Entity*>* item = entities.start;

	while (item != nullptr)
	{
		++entity_deleted;

		App->collision->DeleteCollider(item->data->main_collider);
		
		p2List_item<Entity*>* iterator = item->next;

		if (item->data->name == "player")
		{
			RELEASE(item->data);
			player = nullptr;
		}
		else
		{
			RELEASE(item->data);
		}

		entities.del(item);
		item = iterator;
	}

	LOG("Entities deleted: %i  ||   Entities added: %i", entity_deleted, entity_count);
    entity_deleted = entity_count = 0;


	return true;
}

Entity* EntityManager::CreateEntity( p2SString name, fPoint position, fPoint spawn_pos , Properties* properties)
{
	BROFILER_CATEGORY("EntityManager CreateEntity", Profiler::Color::LemonChiffon);

	Entity* entity = nullptr;

	if (name == "player")
	{
		entity = new j1Player(position, spawn_pos, properties);
		entity->active = true;
		entity->spawned = true;
		player = (j1Player*) entity;
	}
	else if (name == "bat") 
	{
		entity = new Enemy_Bat( position, spawn_pos, properties);
	}
	else if (name == "skeleton") 
	{
	entity = new Enemy_Skeleton(position, spawn_pos, properties);
	}
	else if (name == "coin")
	{
		entity = new Coin(position, spawn_pos, properties);
	}

	if (entity != nullptr) 
	{
		LOG("Entity %s created at Position  x: %.1f  y: %.1f", name.GetString() , position.x, position.y);
		entities.add(entity);
		++entity_count;
		++entity_count_id;
		entity->id = entity_count_id;
	}
	else 
	{
		LOG("Entity could't be created");
	}

	return entity;
}

bool EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (p2List_item<Entity*> *item = entities.start; item != nullptr; item = item->next)
	{
     	if ( !item->data->active)
		{
			continue;
		}

		if (item->data->GetMainCollider() == c1)
		{
			item->data->OnCollision(c1, c2);
		}

	}

	return true;
}

bool EntityManager::Load(pugi::xml_node& node)
{
	
	for (pugi::xml_node node_entity = node.child("entity"); node_entity;  node_entity = node_entity.next_sibling("entity"))
	{
		int id = node_entity.attribute("id").as_int(0);

		Entity *entity_to_found = nullptr;

		for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
		{
			if (id == item->data->id)
			{
				entity_to_found = item->data;
				break;
			}
		}

		if (entity_to_found == nullptr)
		{
			LOG("Enemy with id : %i not found", id);
			continue;
		}

		entity_to_found->position = fPoint(node_entity.attribute("x").as_float(0.0f), node_entity.attribute("y").as_float(0.0f));
		entity_to_found->spawned = node_entity.attribute("spawned").as_bool(false);
		entity_to_found->active = node_entity.attribute("active").as_bool(false);

		if (entity_to_found->active == true)
		{
			entity_to_found->Active();
		}

		entity_to_found->Load(node_entity);
	}
	return true;
}

bool EntityManager::Save(pugi::xml_node& node) const
{

	pugi::xml_node node_entity;

	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		node_entity = node.append_child("entity");

		node_entity.append_attribute("name") = item->data->name.GetString();
		node_entity.append_attribute("id") = item->data->id;
		node_entity.append_attribute("x") = (int)item->data->position.x;
		node_entity.append_attribute("y") = (int)item->data->position.y;
		node_entity.append_attribute("spawned") = item->data->spawned;
		node_entity.append_attribute("active") = item->data->active;

		item->data->Save(node_entity);
	}


	return true;
}

bool EntityManager::ResetAll()
{
	for (p2List_item<Entity*> *item = entities.start; item; item = item->next)
	{
		if (item->data->name != "player")
		{
			item->data->active = false;
			item->data->spawned = false;
		}
		item->data->position = item->data->spawn_pos;
		item->data->Reset();
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

