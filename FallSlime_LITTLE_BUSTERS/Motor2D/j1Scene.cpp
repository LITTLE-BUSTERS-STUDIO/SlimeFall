#include "j1Scene.h"
#include "j1Map.h"
#include "j1App.h"



bool j1Scene::LoadPhase(uint phase_number) 
{

	p2List_item<Phase*>* item = nullptr;

	if (phase_number <= 0)
	{
		return true;
	}

	for (item = phases.start; item; item = item->next)
	{
		if (item->data->id == phase_number - 1)
		{
			break;
		}
	}
	App->map->Load(item->data->map_path.GetString());

	return true;
}
bool  j1Scene::UnloadPhase(uint phase_number)
{

	return true;
}