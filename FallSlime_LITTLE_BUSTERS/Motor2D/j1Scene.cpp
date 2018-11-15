#include "j1Scene.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "EntityManager.h"


bool j1Scene::LoadPhase(uint phase_number, bool spawn) 
{
	App->fade_to_black->FadeToBlack(0.5f);
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

	if (item == NULL)
	{
		LOG("Couldn't load phase %i", phase_number);
		return false;
	}

	App->map->CleanUp();
	ret = App->map->Load(item->data->map_path.GetString());

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

bool  j1Scene::UnloadPhase(uint phase_number)
{
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	bool ret = true;

	// Assigment keys =======================================

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) 
		ret = false;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		LoadPhase(1);

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->entity_manager->GetPlayer()->reset = true;
		App->render->reset = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		LoadPhase(2);

	return ret;
}

bool j1Scene::NextPhase()
{
	bool ret = true;
	ret = LoadPhase(++current_phase);
	if (!ret)
	{
		current_phase = 1;
		LoadPhase(current_phase);
	}
	return ret;
}

bool  j1Scene::Load(pugi::xml_node& node)
{
	current_phase =  node.child("phase").attribute("current_phase").as_uint(1u);
	LoadPhase(current_phase, false);
	return true;
}
bool  j1Scene::Save(pugi::xml_node& node) const
{
	pugi::xml_node phase_node = node.append_child("phase");
	phase_node.append_attribute("current_phase") = current_phase;
	return true;
}