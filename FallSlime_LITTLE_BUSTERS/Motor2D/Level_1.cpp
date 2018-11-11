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
#include "Enemies.h"

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
	LOG("Loading Level 1");
	bool ret = true;
	music_path = config.child("music").attribute("path").as_string("");
	background_path = config.child("background").attribute("path").as_string("");

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
	backgorund_startpos = config.child("backgorund_startpos").attribute("value").as_uint(0u);
	parallax_speed_1 = config.child("parallax_speed").attribute("low").as_float(0.0f);
	parallax_speed_2 = config.child("parallax_speed").attribute("medium").as_float(0.0f);
	parallax_speed_3 = config.child("parallax_speed").attribute("high").as_float(0.0f);
	return ret;
}

// Called before the first frame
bool Level_1::Start()
{
	LoadPhase(1);
	App->audio->PlayMusic(music_path.GetString());
	background_parallax = App->tex->Load(background_path.GetString());
	//Parallax
	for (uint i = 0; i < max_background_layers; i++)
	{
		parallax[i].rect_parallax.x = background_width * i;
		parallax[i].rect_parallax.y = 0;
		parallax[i].rect_parallax.w = background_width;
		parallax[i].rect_parallax.h = background_high;
	}

	App->enemies->SpawnEnemy(Enemy_Info(10, 10, 30, 30 , Enemy_Type ::Test)); //TODO: Delete TEST
	return true;
}

// Called each loop iteration
bool Level_1::Update(float dt)
{
	int x, y;

	App->input->GetMousePosition(x, y);
	iPoint map_coordinates(x / App->map->data.tile_width, y / App->map->data.tile_height);
	iPoint map_coordinates_pixel(x, y);


	//Title=======================================================================================DO NOT DELETE THIS
	/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d, %d Pixel: %d, %d Camera: %d, %d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y,
		map_coordinates_pixel.x + App->render->camera.x, map_coordinates_pixel.y + App->render->camera.y,
		App->render->camera.x, App->render->camera.y);

	App->win->SetTitle(title.GetString());*/

	return true;
}

// Called each loop iteration
bool Level_1::PostUpdate()
{
	bool ret = true;

	// Blit background
	for (uint i = 0; i < max_background_layers; i++)
	{
		if (i < 4) App->render->Blit(background_parallax, 0, backgorund_startpos, &parallax[i].rect_parallax, false, parallax_speed_1);
		else if (i == 4) App->render->Blit(background_parallax, 0, backgorund_startpos, &parallax[i].rect_parallax, false, parallax_speed_3);
		else if (i > 4) App->render->Blit(background_parallax, 0, backgorund_startpos, &parallax[i].rect_parallax, false, parallax_speed_2);
	}

	App->map->Draw();
	return ret;
}

// Called before quitting
bool Level_1::CleanUp()
{
	LOG("Freeing scene");
	App->tex->UnLoad(background_parallax);

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
