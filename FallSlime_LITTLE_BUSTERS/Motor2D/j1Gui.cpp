#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

#include "Object.h"
#include "Label.h"
#include "Image.h"
#include "Button_Input.h"
#include "Slider.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
	screen = new Object({ 0,0 }, nullptr);
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& config)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = config.child("atlas").attribute("path").as_string("");

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

	 App->input->GetMousePosition(cursor_position.x, cursor_position.y);

	// Hover States ============================================
	SDL_Rect rect;

	for (p2List_item<Object*> * item = objects_list.start; item; item = item->next)
	{
		rect.x = item->data->position.x - item->data->section.w / 2;
		rect.y = item->data->position.y - item->data->section.h / 2;
		rect.w = item->data->section.w;
		rect.h = item->data->section.h;

		if (cursor_position.x > rect.x && cursor_position.x < rect.x + rect.w && cursor_position.y > rect.y && cursor_position.y < rect.y + rect.h)
		{
			if (item->data->hover_state == HoverState::None)
				item->data->hover_state = HoverState::On;
			else
				item->data->hover_state = HoverState::Repeat;
		}
		else
		{
			if (item->data->hover_state == HoverState::On || item->data->hover_state == HoverState::Repeat)
				item->data->hover_state = HoverState::Out;
			else
				item->data->hover_state = HoverState::None;
		}
	}

	// Click States ============================================
	
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		SelectClickedObject();
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && clicked_object)
	{
		click_state = ClickState::Repeat;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && clicked_object)
	{
		click_state = ClickState::Out;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_IDLE && clicked_object)
	{
		click_state = ClickState::None;
		clicked_object = nullptr;
	}
	return true;
}

bool j1Gui::Update(float dt)
{
	// Draggable ================================================

	if (clicked_object && clicked_object->is_draggable)
	{
		switch (click_state)
		{
		case ClickState::On:
			App->gui->SetCursorOffset(cursor_position - clicked_object->GetPosition());
			break;
		case ClickState::Repeat:
			clicked_object->SetPosition(cursor_position - App->gui->GetCursorOffset());
			clicked_object->SetRelativePosition(clicked_object->GetPosition() - clicked_object->GetAnchorParent()->GetPosition());
			break;
		case ClickState::Out:
			App->gui->SetCursorOffset({0,0});
			break;
		}
	}
	// Click Callbacks =============================================

	if (clicked_object && clicked_object->listener)
	{
		switch (click_state)
		{
		case ClickState::On:
			clicked_object->listener->OnClick(clicked_object);
			break;
		case ClickState::Repeat:
			
			break;
		case ClickState::Out:
		
			break;
		}
	}

	// Hover Callbacks =============================================

	for (p2List_item<Object*> * item = objects_list.start; item; item = item->next)
	{
		if (item->data->listener == nullptr)
		{
			LOG("Object callback failed, listener was nullptr");
			continue;
		}

		switch (item->data->hover_state)
		{
		case HoverState::On:
			item->data->listener->OnHover(item->data);
			break;
		case HoverState::Out:
			item->data->listener->OutHover(item->data);
			break;
		case HoverState::Repeat:
			item->data->listener->OnHover(item->data);
			break;
		}
	}

	// Update objects ==============================================
	for (p2List_item<Object*> * item = objects_list.start; item; item = item->next)
	{
		item->data->Update(dt);
	}

	UpdateGuiPositions(screen, { 0,0 });

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	DrawGui(screen);
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
 SDL_Texture* j1Gui::GetAtlas() const 
{
	return atlas;
}

// Creation methods =================================================================

Label* j1Gui::CreateLabel(iPoint position, p2SString text, _TTF_Font* font, Gui_Listener* listener)
{
	Label* object = new Label(position, text, font, listener);
	object->SetAnchor(screen);
	objects_list.add(object);
	return object;

}

Image* j1Gui::CreateImage(iPoint position, Animation animation, Gui_Listener* listener)
{
	Image* object = new Image(position, animation, atlas, listener);
	object->SetAnchor(screen);
	objects_list.add(object);
	return object;
}

Button_Input* j1Gui::CreateButton(iPoint position, Button_Definition animation, Gui_Listener* listener)
{
	Button_Input* object = new Button_Input(position, animation, atlas, listener);
	object->SetAnchor(screen);
	objects_list.add(object);
	return object;
}


// ====================================================================================

Object * j1Gui::GetClickedObject()
{
	return clicked_object;
}

Object * j1Gui::GetScreen()
{
	return screen;
}

bool j1Gui::DeleteObject(Object * object)
{
	if (object == nullptr)
	{
		return false;
	}

	int index = objects_list.find(object);

	if (index == -1)
	{
		return false;
	}
	p2List_item<Object*> * item = objects_list.At(index);

	if (item->data->anchor_parent != nullptr)
	{
		p2List<Object*> *list = item->data->anchor_parent->GetAnchorSons();
		int son_index = list->find(object);
		if (son_index != -1)
		{
			p2List_item<Object*>* item_son = list->At(son_index);
			list->del(item_son);
		}
	}

	RELEASE(item->data);
	item->data = nullptr;
	objects_list.del(item);

	return true;
}

iPoint j1Gui::GetCursorOffset() const
{
	return cursor_offset;
}

void j1Gui::SetCursorOffset(const iPoint offset)
{
	cursor_offset = offset;
}

bool j1Gui::SelectClickedObject()
{
	p2List<Object*> clicked_objects;

	for (p2List_item<Object*> * item = objects_list.start; item; item = item->next)
	{
		if (item->data->hover_state == HoverState::On || item->data->hover_state == HoverState::Repeat)
		{
			clicked_objects.add(item->data);
		}
	}

	// Select nearest object -------------------------------
	if (clicked_objects.count() > 0)
	{
		Object* nearest_object = nullptr;
		int nearest_object_position = -1;

		for (p2List_item<Object*> * item = clicked_objects.start; item; item = item->next)
		{
			int count = 0;
			for (Object* iterator = item->data; iterator; iterator = iterator->anchor_parent)
			{
				++count;
			}

			if (count > nearest_object_position)
			{
				nearest_object_position = count;
				nearest_object = item->data;
			}
		}
		clicked_object = nearest_object;
		click_state = ClickState::On;
	}

	return true;
}

void j1Gui::DrawGui(Object * object)
{
	
	object->Draw();

	if (App->gui->debug)
	{
		SDL_Rect rect;
		rect.x = object->position.x - object->section.w / 2;
		rect.y = object->position.y - object->section.h / 2;
		rect.w = object->section.w;
		rect.h = object->section.h;

		if (object->hover_state == HoverState::On || object->hover_state == HoverState::Repeat)
		{
			App->render->DrawQuad(rect, 255, 0, 0, 100, true, false);
		}
		else
		{
			App->render->DrawQuad(rect, 255, 100, 40, 100, true, false);
		}
	}

	for (p2List_item<Object*> *item = object->anchor_sons.start; item; item = item->next)
	{
		DrawGui(item->data);
	}
}

void j1Gui::UpdateGuiPositions(Object * object, iPoint cumulated_position)
{
	cumulated_position += object->relative_position;
	object->position = cumulated_position;

	for (p2List_item<Object*> *item = object->anchor_sons.start; item; item = item->next)
	{
		UpdateGuiPositions(item->data, cumulated_position);
	}
}
