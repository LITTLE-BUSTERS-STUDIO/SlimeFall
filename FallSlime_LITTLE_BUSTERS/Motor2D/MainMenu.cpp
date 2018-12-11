#include "p2Defs.h"
#include "p2Log.h"
#include "MainMenu.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "Brofiler/Brofiler.h"

MainMenu::MainMenu() : j1Scene()
{
	name.create("main_menu");
}

MainMenu::~MainMenu()
{}


bool MainMenu::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("MainMenu Awake", Profiler::Color::Maroon);

	LOG("Loading MainMenu ");
	bool ret = true;
	music_path = config.child("music").attribute("path").as_string("");

	return ret;
}

bool MainMenu::Start()
{
	BROFILER_CATEGORY("MainMenu Start", Profiler::Color::MediumAquaMarine);

	App->audio->PlayMusic(music_path.GetString());

	return true;
}

bool MainMenu::Update(float dt)
{
	BROFILER_CATEGORY("MainMenu Update", Profiler::Color::MediumBlue);

	return true;
}

bool MainMenu::PostUpdate(float dt)
{
	BROFILER_CATEGORY("MainMenu PostUpdate", Profiler::Color::MediumOrchid);

	bool ret = true;

	App->map->Draw();

	return ret;
}

bool MainMenu::CleanUp()
{
	BROFILER_CATEGORY("Level1 CleanUp", Profiler::Color::MediumPurple);

	LOG("Freeing scene MainMenu");


	return true;
}
