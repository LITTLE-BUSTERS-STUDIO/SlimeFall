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
#include "Checkbox.h"

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

	for (p2List_item<Object*> * item = objects_list.start; item != nullptr ; item = item->next)
	{
		if (item->data->state != ObjectState::visible)
		{
			continue;
		}

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

		item->data->PreUpdate();
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
			clicked_object->UpdateRelativePosition();
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
			clicked_object->listener->RepeatClick(clicked_object);
			break;
		case ClickState::Out:
			if (clicked_object->hover_state != HoverState::None)
			{
				clicked_object->listener->OutClick(clicked_object);
			}
			break;
		}
	}

	// Hover Callbacks =============================================

	for (p2List_item<Object*> * item = objects_list.start; item; item = item->next)
	{
		if (item->data->listener == nullptr)
		{
			if (item->data != screen)
			{
				LOG("Object callback failed, listener was nullptr");
			}
			
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

	UpdateGuiPositions(screen, { 0,0 });

	// Update objects ==============================================
	for (p2List_item<Object*> * item = objects_list.start; item; item = item->next)
	{
		item->data->Update(dt);
	}

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

 ClickState j1Gui::GetClickState() const
 {
	 return click_state;
 }

// Creation methods =================================================================

 Object * j1Gui::CreateObject(iPoint position, Gui_Listener * listener)
 {
	 Object* object = new Object(position, listener);
	 object->SetAnchor(screen);
	 objects_list.add(object);
	 return object;
 }

 Label* j1Gui::CreateLabel(iPoint position, p2SString text, _TTF_Font* font, Gui_Listener* listener, SDL_Color color )
{
	Label* object = new Label(position, text, font, color, listener);
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

Button_Input* j1Gui::CreateButton(iPoint position, Button_Definition definition, Gui_Listener* listener)
{
	Button_Input* object = new Button_Input(position, definition, atlas, listener);
	object->SetAnchor(screen);
	objects_list.add(object);
	return object;
}

Slider * j1Gui::CreateSlider(iPoint position, Slider_Definition definition, Gui_Listener * listener)
{
	Slider* object = new Slider(position, definition, atlas, listener);
	object->SetAnchor(screen);
	objects_list.add(object);
	return object;
}

Checkbox * j1Gui::CreateCheckbox(iPoint position, Checkbox_Definition definition, Gui_Listener * listener)
{
	Checkbox* object = new Checkbox(position, definition, atlas, listener);
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
		LOG("Object not deleted: Pointer nullptr");
		return false;
	}
	p2List_item<Object*>* object_to_delete = nullptr;

	for (p2List_item<Object*> * item = objects_list.start; item != nullptr; item = item->next)
	{
		if (object == item->data)
		{
			object_to_delete = item;
		}
	}

	if (object_to_delete == nullptr)
	{
		LOG("Object not deleted: Not found");
		return false;
	}

	if (object_to_delete->data->anchor_parent != nullptr)
	{
		p2List<Object*> *list = object_to_delete->data->anchor_parent->GetAnchorSons();
		p2List_item<Object*> *item = list->start;

		while (item != nullptr)
		{
			if (object == item->data)
			{
				p2List_item<Object*>* iterator = item->next;
				list->del(item);
				item = iterator;
			}
			else
			{
				item = item->next;
			}
		}
	}

	RELEASE(object_to_delete->data);
	objects_list.del(object_to_delete);

	return true;
}

void j1Gui::SetStateToBranch(const ObjectState state, Object * branch_root)
{
	if (branch_root == nullptr)
	{
		return;
	}

	branch_root->state = state;

	for (p2List_item<Object*> *item = branch_root->anchor_sons.start; item != nullptr; item = item->next)
	{
		SetStateToBranch(state, item->data);
	}

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
		if (item->data->hover_state != HoverState::None  &&  item->data->state == ObjectState::visible && item->data->is_interactive == true)
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
	if (object == nullptr)
	{
		return;
	}

	if (object->state != ObjectState::hidden)
	{
		object->Draw();
	}
	
	if (App->gui->debug && object->state != ObjectState::hidden)
	{
		SDL_Rect rect;
		rect.x = object->position.x - object->section.w / 2;
		rect.y = object->position.y - object->section.h / 2;
		rect.w = object->section.w;
		rect.h = object->section.h;

		if (object->hover_state != HoverState::None )
		{
			App->render->DrawQuad(rect, 255, 0, 0, 100, true, false);
		}
		else
		{
			App->render->DrawQuad(rect, 255, 100, 40, 100, true, false);
		}
	}

	for (p2List_item<Object*> *item = object->anchor_sons.start;  item != nullptr; item = item->next)
	{
		DrawGui(item->data);
	}
}

void j1Gui::UpdateGuiPositions(Object * object, iPoint cumulated_position)
{
	if (object == nullptr)
	{
		return;
	}

	cumulated_position += object->relative_position;
	object->position = cumulated_position;

	for (p2List_item<Object*> *item = object->anchor_sons.start; item != nullptr; item = item->next)
	{
		UpdateGuiPositions(item->data, cumulated_position);
	}
}
