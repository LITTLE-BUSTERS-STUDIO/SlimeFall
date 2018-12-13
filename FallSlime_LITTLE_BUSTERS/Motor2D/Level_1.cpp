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
#include "j1Gui.h"

Level_1::Level_1() : j1Scene()
{
	name.create("level_1");
}


bool Level_1::LoadScene(pugi::xml_node& node)
{
	BROFILER_CATEGORY("Level_1 Load", Profiler::Color::Maroon);

	LOG("Loading Level 1");

	music_path = node.child("music").attribute("path").as_string("");
	App->audio->PlayMusic(music_path.GetString());

	paralax_tex_1 = App->tex->Load(node.child("background1").attribute("path").as_string(""));
	paralax_tex_2 = App->tex->Load(node.child("background2").attribute("path").as_string(""));
	paralax_tex_3 = App->tex->Load(node.child("background3").attribute("path").as_string(""));

	background_width = node.child("background_dimension").attribute("width").as_uint(0u);
	background_high = node.child("background_dimension").attribute("high").as_uint(0u);
	max_background_layers = node.child("max_background_layers").attribute("value").as_uint(0u);
	background_startpos = node.child("background_startpos").attribute("value").as_uint(0u);
	parallax_speed_1 = node.child("parallax_speed").attribute("low").as_float(0.0f);
	parallax_speed_2 = node.child("parallax_speed").attribute("medium").as_float(0.0f);
	parallax_speed_3 = node.child("parallax_speed").attribute("high").as_float(0.0f);

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


bool Level_1::Update(float dt)
{
	BROFILER_CATEGORY("Level_1 Update", Profiler::Color::MediumBlue);


	return true;
}

bool Level_1::PostUpdate()
{
	BROFILER_CATEGORY("Level_1 PostUpdate", Profiler::Color::MediumOrchid);

	bool ret = true;

	// Blit background--------------------------------------
	for (uint i = 0; i < max_background_layers; i++)
		App->render->Blit(paralax_tex_1, 0, background_startpos, &parallax1[i].rect_parallax, false, parallax_speed_1 /** dt*/);


	for (uint i = 0; i < max_background_layers; i++)
	{

		if (i == 0)
			App->render->Blit(paralax_tex_2, 0, background_startpos, &parallax2[i].rect_parallax, false, parallax_speed_3 /** dt*/);
		else if (i > 0)
			App->render->Blit(paralax_tex_2, 0, background_startpos, &parallax2[i].rect_parallax, false, parallax_speed_2/** dt*/);
	}

	for (uint i = 0; i < max_background_layers; i++)
		App->render->Blit(paralax_tex_3, 0, background_startpos, &parallax3[i].rect_parallax, false, parallax_speed_2 /** dt*/);


	App->map->Draw();

	return ret;
}

// Called before quitting
bool Level_1::UnloadScene()
{
	BROFILER_CATEGORY("Level_1 Unload", Profiler::Color::MediumPurple);

	LOG("Freeing level_1");
	App->tex->UnLoad(paralax_tex_1);
	App->tex->UnLoad(paralax_tex_2);
	App->tex->UnLoad(paralax_tex_3);

	return true;
}
