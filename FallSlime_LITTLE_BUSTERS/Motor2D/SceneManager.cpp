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
	bool ret = true;


	return ret;
}

bool SceneManager::Start()
{
	BROFILER_CATEGORY("Scene Manager Start", Profiler::Color::MediumAquaMarine);

	return true;
}

bool SceneManager::Update(float dt)
{
	BROFILER_CATEGORY("Scene Manager Update", Profiler::Color::MediumBlue);

	return true;
}

bool SceneManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Scene Manager PostUpdate", Profiler::Color::MediumOrchid);

	bool ret = true;

	App->map->Draw();

	return ret;
}

bool SceneManager::CleanUp()
{
	BROFILER_CATEGORY("Scene Manager CleanUp", Profiler::Color::MediumPurple);

	LOG("Freeing Scene Manager");


	return true;
}
