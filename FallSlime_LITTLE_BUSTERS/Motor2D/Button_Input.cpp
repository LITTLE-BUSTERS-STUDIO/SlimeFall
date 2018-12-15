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
	SDL_Rect anim_rect;

	if (App->gui->GetClickedObject() == this)
	{
		anim_rect = definition.pushed_rect;
	}
	else if (hover_state == HoverState::Repeat || hover_state == HoverState::On)
	{
		anim_rect = definition.hover_rect;
	}
	else
	{
		anim_rect = definition.idle_rect;
	}
	if (hover_state == HoverState::On)
		App->audio->PlayFx(App->gui->fx_button_hovered);

	section.w = anim_rect.w;
	section.h = anim_rect.h;
	
	App->render->Blit(texture, position.x - section.w/2 , position.y - section.h/2, &anim_rect, false, 0.0f);
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
			label->SetPosition(iPoint(label->GetPosition().x, label->GetPosition().y + 3));
			break;
		case ClickState::Out:
			label->SetPosition(iPoint(label->GetPosition().x, label->GetPosition().y - 3));
			break;
		}
	}

	return true;
}


