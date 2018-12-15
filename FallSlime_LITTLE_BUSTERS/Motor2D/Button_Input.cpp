#include "Button_Input.h"
#include "j1Render.h"
#include "j1Gui.h"
#include  "p2Log.h"
#include "j1Input.h"
#include "Label.h"
#include "j1Audio.h"


Button_Input::Button_Input(const iPoint position, const Button_Definition definition, SDL_Texture * texture, Gui_Listener* listener) : Object(position, listener)
{
	this->definition = definition;
	this->texture = texture;
}


Button_Input::~Button_Input()
{
	if (label != nullptr)
	{
		App->gui->DeleteObject(label);
		label = nullptr;
	}
}

bool Button_Input::Draw()
{
	ClickState state = App->gui->GetClickState();
	SDL_Rect current_frame;

	if (App->gui->GetClickedObject() == this && state != ClickState::None &&  state != ClickState::Out)
	{
		current_frame = definition.pushed_rect;
	}
	else if (hover_state != HoverState::None)
	{
		current_frame = definition.hover_rect;
	}
	else
	{
		current_frame = definition.idle_rect;
	}

	section.w = current_frame.w;
	section.h = current_frame.h;
	
	App->render->Blit(texture, position.x - section.w*0.5f, position.y - section.h*0.5f, &current_frame, false, 0.0f);

	return true;
}

bool Button_Input::SetLabel(const iPoint position, const p2SString text, _TTF_Font* font, const SDL_Color color)
{
	if (label != nullptr)
	{
		App->gui->DeleteObject(label);
		label = nullptr;
	}

	label = App->gui->CreateLabel(position, text, font, this, color);
	label->SetAnchor(this);

	return true;
}

void Button_Input::SetDefinition(Button_Definition definition)
{
	this->definition = definition;
}

bool Button_Input::PreUpdate()
{
	if (hover_state == HoverState::On)
	{
		App->audio->PlayFx(App->gui->fx_button_hovered);
	}
	
	return true;
}

bool Button_Input::Update(float dt)
{
	if (label == nullptr)
	{
		return true;
	}

	if (this == App->gui->GetClickedObject())
	{
		ClickState state = App->gui->GetClickState();
		
		switch (state)
		{
		case ClickState::On:
			label->SetPosition(iPoint(label->position.x, label->position.y + LABEL_PRESSED_OFFSET));
			break;
		case ClickState::Out:
			label->SetPosition(iPoint(label->position.x, label->position.y - LABEL_PRESSED_OFFSET));
			break;
		}
	}

	return true;
}


