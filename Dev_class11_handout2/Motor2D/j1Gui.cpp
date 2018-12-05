#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "Object.h"
#include "Label.h"
#include "Image.h"
#include "Button_Input.h"
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
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug = !debug;

	// Detect hover on ============================================
	SDL_Rect rect;
	int x, y;
	App->input->GetMousePosition(x, y);

	LOG("MOUSE X: %i Y: %i", x, y);

	for (p2List_item<Object*> * item = objects_list.start; item; item = item->next)
	{
		rect.x = item->data->position.x - item->data->section.w / 2;
		rect.y = item->data->position.y - item->data->section.h / 2;
		rect.w = item->data->section.w;
		rect.h = item->data->section.h;

		if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
		{
			item->data->hover_on = true;
		}
		else
		{
			item->data->hover_on = false;
		}
	}
	// Detect click ===============================================

	return true;
}

bool j1Gui::Update(float dt)
{
	for (p2List_item<Object*> * item = objects_list.start; item; item = item->next)
	{
		if (item->data->hover_on)
		{
			if (item->data->listener)
			{
				item->data->listener->OnHover(item->data);
			}
			else
				LOG("OnHover failed, listener was nullptr");
			
		}
	}

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

Label* j1Gui::CreateLabel(iPoint position, p2SString text, _TTF_Font* font, Gui_Listener* listener)
{
	Label* object = new Label(position, text, font, listener);
	objects_list.add(object);
	return object;

}

Image* j1Gui::CreateImage(iPoint position, Animation animation, SDL_Texture* texture, Gui_Listener* listener)
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

	Image* object = new Image(position, animation, tex, listener);
	objects_list.add(object);
	return object;
}

Button_Input* j1Gui::CreateButton(iPoint position, Animation animation, SDL_Texture * texture, Gui_Listener* listener)
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

	Button_Input* object = new Button_Input(position, animation, tex, listener);
	objects_list.add(object);

	return object;
}
