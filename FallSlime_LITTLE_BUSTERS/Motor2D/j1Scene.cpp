#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
	
	
}

// Destructor
j1Scene::~j1Scene()
{}


// Called before render is available
bool j1Scene:: Awake(pugi::xml_node& node )
{
	LOG("Loading Scene");
	bool ret = true;
	music_path = node.child("music").attribute("path").as_string("");
	background_path = node.child("background").attribute("path").as_string("");

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load("Level1_2.tmx");
	App->audio->PlayMusic(music_path.GetString());
	background_parallax = App->tex->Load(background_path.GetString());

	//Parallax
	for (uint i = 0; i < 11; i++)
	{
		parallax[i].rect_parallax.x = 928*i;
		parallax[i].rect_parallax.y = 0;
		parallax[i].rect_parallax.w = 928;
		parallax[i].rect_parallax.h = 793;
	}
	

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) //Close Window
		ret = false;

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();


	return ret;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates(x/App->map->data.tile_width, y/ App->map->data.tile_height);
	iPoint map_coordinates_pixel(x  , y );

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d, %d Pixel: %d, %d Camera: %d, %d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y,
					map_coordinates_pixel.x + App->render->camera.x, map_coordinates_pixel.y + App->render->camera.y,
					App->render->camera.x, App->render->camera.y);

	App->win->SetTitle(title.GetString());

	// Blit background
	for (uint i = 0; i < 11; i++)
	{
		App->render->Blit(background_parallax, 0, -300, &parallax[i].rect_parallax, true, 0.4f); // -300 equals to start position.y
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
	App->map->Draw();
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->tex->UnLoad(background_parallax);

	return true;
}
