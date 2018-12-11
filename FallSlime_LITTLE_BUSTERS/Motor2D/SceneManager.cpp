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

	LOG("Loading Scene Manager "); // All scenes information

	pugi::xml_document doc;
	doc.load_file("data/scenes.xml");
	pugi::xml_node node = doc.child("scenes");

	current_scene = new Level_1;
	level = (Level_1*)current_scene;
	level->Awake(node.child(level->name.GetString()));


	return true;
}

bool SceneManager::Start()
{
	BROFILER_CATEGORY("Scene Manager Start", Profiler::Color::MediumAquaMarine);
	level->Start();

	return true;
}

bool SceneManager::PreUpdate()
{
	BROFILER_CATEGORY("Scene PreUpdate", Profiler::Color::Linen);

	if (current_scene == nullptr)
	{
		return true;
	}
	else
	{
		level->PreUpdate();
	}

	if (default_phase_loaded == false)
	{
		LoadPhase(current_scene->default_phase);
		default_phase_loaded = true;
	}

	// Assigment keys =======================================

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
		level->Update(dt);
	}
	return true;
}

bool SceneManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Scene Manager PostUpdate", Profiler::Color::MediumOrchid);

	bool ret = true;
	if (current_scene == nullptr)
	{
		return true;
	}
	else
	{
		level->PostUpdate(dt);
	}
	return ret;
}

bool SceneManager::CleanUp()
{
	BROFILER_CATEGORY("Scene Manager CleanUp", Profiler::Color::MediumPurple);

	LOG("Freeing Scene Manager");


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

	current_scene = new Level_1;

	return false;
}

bool SceneManager::UnloadScene()
{
	if (current_scene)
	{
		current_scene->CleanUp();
		delete current_scene;
	}

	return false;
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

	App->entity_manager->UnloadEntities();
	App->map->CleanUp();
	ret = App->map->Load(item->data->map_path.GetString() );

	if (ret)
	{
		current_phase = phase_number;

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


