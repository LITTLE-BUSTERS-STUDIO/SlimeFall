#include "j1Scene.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Player.h"
#include "p2Log.h"



bool j1Scene::LoadPhase(uint phase_number) 
{
	p2List_item<Phase*>* item = nullptr;
	bool ret = true;
	if (phase_number <= 0)
	{
		return true;
	}


	for (item = phases.start; item; item = item->next)
	{
		if (item->data->id == phase_number )
		{
			break;
		}

	}

	App->map->CleanUp();
	ret = App->map->Load(item->data->map_path.GetString());

	if (ret)
	{
		current_phase = phase_number;
		App->player->position = App->map->data.initial_position;
	}

	return ret;
}

bool  j1Scene::UnloadPhase(uint phase_number)
{

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

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		LoadPhase(1);

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		LoadPhase(2);

	return ret;
}