#include "p2Defs.h"
#include "p2Log.h"
#include "SceneManager.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "EntityManager.h"
#include "j1Scene.h"

#include "Level_1.h"
#include "MainMenu.h"
#include "Brofiler/Brofiler.h"

SceneManager::SceneManager() : j1Module()
{
	name.create("scene_manager");
}

SceneManager::~SceneManager()
{}

bool SceneManager::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("Scene Manager Awake", Profiler::Color::Maroon);

	LOG("Loading Scene Manager "); 

	scenes_doc.load_file(config.child("document").attribute("path").as_string(""));
	pugi::xml_node node = scenes_doc.child("scenes");
	default_scene_str.create(scenes_doc.child("scenes").child("default_scene").attribute("name").as_string(""));

	return true;
}

bool SceneManager::Start()
{
	BROFILER_CATEGORY("Scene Manager Start", Profiler::Color::MediumAquaMarine);
	return true;
}

bool SceneManager::PreUpdate()
{
	BROFILER_CATEGORY("Scene PreUpdate", Profiler::Color::Linen);

	if (default_scene_loaded == false)
	{
		LoadScene(default_scene_str);
		default_scene_loaded = true;
	}

	if (current_scene == nullptr)
	{
		return true;
	}
	else
	{
		current_scene->PreUpdate();
	}

	// Debug keys =======================================

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return false;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		LoadPhase(1);

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->entity_manager->ResetAll();
		App->entity_manager->GetPlayer()->reset = true;
		App->render->reset = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		LoadPhase(2);

	return true;
}

bool SceneManager::Update(float dt)
{
	BROFILER_CATEGORY("Scene Manager Update", Profiler::Color::MediumBlue);

	if (current_scene == nullptr)
	{
		return true;
	}
	else
	{
		current_scene->Update(dt);
	}

	return true;
}

bool SceneManager::PostUpdate()
{
	BROFILER_CATEGORY("Scene Manager PostUpdate", Profiler::Color::MediumOrchid);

	bool ret = true;

	if (current_scene == nullptr)
	{
		return true;
	}
	else
	{
		current_scene->PostUpdate();
	}

	return ret;
}

bool SceneManager::CleanUp()
{
	BROFILER_CATEGORY("Scene Manager CleanUp", Profiler::Color::MediumPurple);

	LOG("Freeing Scene Manager");

	UnloadScene();

	return true;
}

j1Scene* SceneManager::GetCurrentScene()
{
	return current_scene;
}

bool  SceneManager::Load(pugi::xml_node& node)
{
	current_phase = node.child("phase").attribute("current_phase").as_uint(1u);
	LoadPhase(current_phase, false);
	return true;
}

bool  SceneManager::Save(pugi::xml_node& node) const
{
	pugi::xml_node phase_node = node.append_child("phase");
	phase_node.append_attribute("current_phase") = current_phase;
	return true;
}

bool SceneManager::LoadScene(p2SString name)
{
	UnloadScene();

	j1Scene* scene_to_load = nullptr;

	pugi::xml_node node_to_send;

	// Search scene name on document scenes.xml =============

	p2SString scene_name;

	for (pugi::xml_node node = scenes_doc.child("scenes").child("scene"); node; node = node.next_sibling("scene"))
	{
		scene_name.create(node.attribute("name").as_string(""));

		if (scene_name == name.GetString())
		{
			node_to_send = node;
		}
	}

	if (node_to_send == NULL)
	{
		LOG("Error Loading scene with name: %s not found on scenes.xml", name.GetString());
		return false;
	}

	// Create scene ========================================   // Add here all scene names 

	if (name == "level_1")
	{
		scene_to_load = new Level_1();
	}
	else if (name == "main_menu")
	{
		scene_to_load = new MainMenu();
	}
	
	if (scene_to_load == nullptr)
	{
		LOG("Scene %s class not found. Add scene in LoadScene");
		return false;
	}

	// Load phases ==========================================

	scene_to_load->default_phase = node_to_send.attribute("default_phase").as_uint(0u);  // Default scene

	for (pugi::xml_node node = node_to_send.child("phase"); node; node = node.next_sibling("phase"))
	{
		Phase* item = new Phase;
		item->id = node.attribute("id").as_uint(0u);
		item->x_limit = node.attribute("x_limit").as_uint(0u);
		item->y_limit = node.attribute("y_limit").as_uint(0u);
		item->map_path.create(node.attribute("map_path").as_string(""));
		scene_to_load->phases.add(item);

		LOG("Added phase id: %u with map path : %s", item->id, item->map_path.GetString());
	}

	// Current scene =======================================
	current_scene = scene_to_load;
	current_scene->LoadScene(node_to_send);
	LoadPhase(scene_to_load->default_phase);

	return true;
}

bool SceneManager::UnloadScene()
{
	if (!current_scene)
	{
		LOG("Could not unload scene: Current scene is nullptr");
		return false;
	}
	// Unload scene ==========================
	current_scene->UnloadScene();

	// Unload phases ==========================
	p2List_item<Phase*>* item;
	item = current_scene->phases.start;
	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	current_scene->phases.clear();

	// Delete scene ==========================
	delete current_scene;

	return true;
}

bool SceneManager::LoadPhase(uint phase_number, bool spawn)
{
	BROFILER_CATEGORY("Scene LoadPhase", Profiler::Color::LimeGreen);

	p2List_item<Phase*>* item = nullptr;
	bool ret = true;
	if (phase_number <= 0)
	{
		return true;
	}

	for (item = current_scene->phases.start; item; item = item->next)
	{
		if (item->data->id == phase_number)
		{
			break;
		}

	}

	if (item == NULL)
	{
		LOG("Couldn't load phase %i", phase_number);
		return false;
	}

	// Unload old map ===================================
	App->entity_manager->UnloadEntities();
	App->map->CleanUp();

	// Load new map  ====================================
	ret = App->map->Load(item->data->map_path.GetString() );

	// Hardcode =========================================
	if (ret)
	{
		current_phase = phase_number;
		App->render->SetCameraLimits(item->data->x_limit, item->data->y_limit);

		if (spawn)
		{
			if (App->entity_manager->GetPlayer())
			{
				App->entity_manager->GetPlayer()->reset = true;
				App->render->reset = true;
			}
			else
				App->entity_manager->CreatePlayer(App->map->data.initial_position);

		}
	}

	return ret;
}

bool SceneManager::NextPhase()
{
	BROFILER_CATEGORY("Scene NextPhase", Profiler::Color::Magenta);

	bool ret = true;
	ret = LoadPhase(++current_phase);
	if (!ret)
	{
		current_phase = 1;
		LoadPhase(current_phase);
	}
	return ret;
}


