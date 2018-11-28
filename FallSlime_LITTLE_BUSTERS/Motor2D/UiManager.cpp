#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "UiManager.h"
#include "j1Input.h"
#include "SDL/include/SDL.h"

UiManager::UiManager()
{
	name.create("ui_manager");
}

UiManager::~UiManager()
{

}

bool UiManager::Awake(pugi::xml_node&)
{
	return true;
}

bool UiManager::PreUpdate()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		CheckClick();
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		selected
	}

	return true;
}

bool UiManager::PostUpdate()
{
	return true;
}

bool UiManager::CleanUp()
{
	return true;
}

bool UiManager::UpdateMouseIcon()
{
	return true;
}

bool UiManager::CheckClick()
{
	return true;
}


bool UiManager::DeleteObject()
{
	return true;
}

