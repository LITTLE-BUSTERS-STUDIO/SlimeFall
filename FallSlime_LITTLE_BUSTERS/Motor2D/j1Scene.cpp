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

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load("Level1.tmx");
	App->audio->PlayMusic(music_path.GetString());
	background_parallax = App->tex->Load("textures/Parallax.png");

	//Parallax
	parallax_1.x = 0;
	parallax_1.y = 0;
	parallax_1.w = 928;
	parallax_1.h = 793;

	parallax_2.x = 928;
	parallax_2.y = 0;
	parallax_2.w = 928;
	parallax_2.h = 793;

	parallax_3.x = 928*2;
	parallax_3.y = 0;
	parallax_3.w = 928;
	parallax_3.h = 793;

	parallax_4.x = 928*3;
	parallax_4.y = 0;
	parallax_4.w = 928;
	parallax_4.h = 793;

	parallax_5.x = 928*4;
	parallax_5.y = 0;
	parallax_5.w = 928;
	parallax_5.h = 793;

	parallax_6.x = 928*5;
	parallax_6.y = 0;
	parallax_6.w = 928;
	parallax_6.h = 793;

	parallax_7.x = 928*6;
	parallax_7.y = 0;
	parallax_7.w = 928;
	parallax_7.h = 793;

	parallax_8.x = 928*7;
	parallax_8.y = 0;
	parallax_8.w = 928;
	parallax_8.h = 793;

	parallax_9.x = 928*8;
	parallax_9.y = 0;
	parallax_9.w = 928;
	parallax_9.h = 793;

	parallax_10.x = 928 * 9;
	parallax_10.y = 0;
	parallax_10.w = 928;
	parallax_10.h = 793;

	parallax_11.x = 928 * 10;
	parallax_11.y = 0;
	parallax_11.w = 928;
	parallax_11.h = 793;


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) //Close Window
		ret = false;

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


	App->render->Blit(background_parallax, 0, -400, &parallax_1);	
	App->render->Blit(background_parallax, 0, -400, &parallax_2);
	App->render->Blit(background_parallax, 0, -400, &parallax_3);
	App->render->Blit(background_parallax, 0, -400, &parallax_4);
	App->render->Blit(background_parallax, 0, -400, &parallax_5);
	App->render->Blit(background_parallax, 0, -400, &parallax_6);
	App->render->Blit(background_parallax, 0, -400, &parallax_7);
	App->render->Blit(background_parallax, 0, -400, &parallax_8);
	App->render->Blit(background_parallax, 0, -400, &parallax_9);
	App->render->Blit(background_parallax, 0, -400, &parallax_10);
	App->render->Blit(background_parallax, 0, -400, &parallax_11);


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
