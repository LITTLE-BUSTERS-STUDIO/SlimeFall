#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "Object.h"
#include "Label.h"
#include "Image.h"
#include "SDL_ttf/include/SDL_ttf.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<Object*>* object;
	object = objects_list.start;

	while (object != NULL)
	{
		RELEASE(object->data);
		object = object->next;
	}
	objects_list.clear();

	return true;
}

// const getter for atlas
 SDL_Texture* j1Gui::GetAtlas() 
{
	return atlas;
}

// class Gui ---------------------------------------------------

Label* j1Gui::CreateLabel(iPoint position, p2SString text, _TTF_Font* font)
{
	Label* object = new Label(position, text, font);
	objects_list.add(object);
	return object;

}

Image* j1Gui::CreateImage(iPoint position, Animation animation, SDL_Texture* texture)
{
	SDL_Texture* tex = nullptr;

	if (texture == nullptr)
	{
		tex = atlas;
	}
	else
	{
		tex = texture;
	}

	Image* object = new Image(position, animation, tex);
	objects_list.add(object);
	return object;
}