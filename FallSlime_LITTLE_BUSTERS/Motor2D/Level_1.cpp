#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "Level_1.h"
#include "EntityManager.h"
#include "Brofiler/Brofiler.h"

Level_1::Level_1() : j1Scene()
{
	name.create("level_1");
}

// Destructor
Level_1::~Level_1()
{}


// Called before render is available
bool Level_1::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("Level1 Awake", Profiler::Color::Maroon);

	LOG("Loading Level 1");
	bool ret = true;
	music_path = config.child("music").attribute("path").as_string("");
	background_path1 = config.child("background1").attribute("path").as_string("");
	background_path2 = config.child("background2").attribute("path").as_string("");
	background_path3 = config.child("background3").attribute("path").as_string("");

	for (pugi::xml_node node = config.child("phase"); node; node = node.next_sibling("phase"))
	{
		Phase* item = new Phase;
		item->id = node.attribute("id").as_uint(0u);
		item->map_path.create(node.attribute("map_path").as_string(""));
		phases.add(item);
	}
	current_phase = config.attribute("current_phase").as_uint(0u);

	background_width = config.child("background_dimension").attribute("width").as_uint(0u);
	background_high = config.child("background_dimension").attribute("high").as_uint(0u);
	max_background_layers = config.child("max_background_layers").attribute("value").as_uint(0u);
	background_startpos = config.child("background_startpos").attribute("value").as_uint(0u);
	parallax_speed_1 = config.child("parallax_speed").attribute("low").as_float(0.0f);
	parallax_speed_2 = config.child("parallax_speed").attribute("medium").as_float(0.0f);
	parallax_speed_3 = config.child("parallax_speed").attribute("high").as_float(0.0f);
	return ret;
}

// Called before the first frame
bool Level_1::Start()
{
	BROFILER_CATEGORY("Level1 Start", Profiler::Color::MediumAquaMarine);

	LoadPhase(1);
	App->audio->PlayMusic(music_path.GetString());
	background_parallax1 = App->tex->Load(background_path1.GetString());
	background_parallax2 = App->tex->Load(background_path2.GetString());
	background_parallax3 = App->tex->Load(background_path3.GetString());

	//Parallax
	for (uint i = 0; i < 4; i++)
	{
		parallax1[i].rect_parallax.x = background_width * i;
		parallax1[i].rect_parallax.y = 0;
		parallax1[i].rect_parallax.w = background_width;
		parallax1[i].rect_parallax.h = background_high;

		parallax2[i].rect_parallax.x = background_width * i;
		parallax2[i].rect_parallax.y = 0;
		parallax2[i].rect_parallax.w = background_width;
		parallax2[i].rect_parallax.h = background_high;

		parallax3[i].rect_parallax.x = background_width * i;
		parallax3[i].rect_parallax.y = 0;
		parallax3[i].rect_parallax.w = background_width;
		parallax3[i].rect_parallax.h = background_high;
	}

	return true;
}

// Called each loop iteration
bool Level_1::Update(float dt)
{
	BROFILER_CATEGORY("Level1 Update", Profiler::Color::MediumBlue);

	int x, y;

	App->input->GetMousePosition(x, y);
	iPoint map_coordinates(x / App->map->data.tile_width, y / App->map->data.tile_height);
	iPoint map_coordinates_pixel(x, y);

	//Title=======================================================================================
	//p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d, %d Pixel: %d, %d Camera: %d, %d",
	//	App->map->data.width, App->map->data.height,
	//	App->map->data.tile_width, App->map->data.tile_height,
	//	App->map->data.tilesets.count(),
	//	map_coordinates.x, map_coordinates.y,
	//	map_coordinates_pixel.x + App->render->camera.x, map_coordinates_pixel.y + App->render->camera.y,
	//	App->render->camera.x, App->render->camera.y);

	//App->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Level_1::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Level1 PostUpdate", Profiler::Color::MediumOrchid);

	bool ret = true;

	// Blit background--------------------------------------
	for (uint i = 0; i < max_background_layers; i++)
		App->render->Blit(background_parallax1, 0, background_startpos, &parallax1[i].rect_parallax, false, parallax_speed_1 /** dt*/);


	for (uint i = 0; i < max_background_layers; i++)
	{

		if (i == 0)
			App->render->Blit(background_parallax2, 0, background_startpos, &parallax2[i].rect_parallax, false, parallax_speed_3 /** dt*/);
		else if (i > 0)
			App->render->Blit(background_parallax2, 0, background_startpos, &parallax2[i].rect_parallax, false, parallax_speed_2/** dt*/);
	}

	for (uint i = 0; i < max_background_layers; i++)
		App->render->Blit(background_parallax3, 0, background_startpos, &parallax3[i].rect_parallax, false, parallax_speed_2 /** dt*/);


	App->map->Draw();

	return ret;
}

// Called before quitting
bool Level_1::CleanUp()
{
	BROFILER_CATEGORY("Level1 CleanUp", Profiler::Color::MediumPurple);

	LOG("Freeing scene");
	App->tex->UnLoad(background_parallax1);
	App->tex->UnLoad(background_parallax2);
	App->tex->UnLoad(background_parallax3);

	// Remove all tilesets
	p2List_item<Phase*>* item;
	item = phases.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	phases.clear();

	return true;
}
