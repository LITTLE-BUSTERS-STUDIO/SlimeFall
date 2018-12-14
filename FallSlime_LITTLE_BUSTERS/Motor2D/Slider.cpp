#include "Slider.h"
#include "j1Render.h"
#include  "p2Log.h"
#include "j1Input.h"
#include "j1Gui.h"

#include"Button_Input.h"

Slider::Slider(iPoint position, Slider_Definition definition, SDL_Texture * texture, Gui_Listener* listener) : Object(position, listener)
{
	this->definition = definition;
	this->texture = texture;

	section.w = definition.rail_draw_rect.w;
	section.h = definition.rail_draw_rect.h;

	button = App->gui->CreateButton(position, definition.button_definition, this);
	button->SetAnchor(this);
	button->IsDraggable(true);

	point_A = position.x - definition.ditance * 0.5f;
	point_B = position.x + definition.ditance * 0.5f;
	current_value = definition.default_value;
	SetValue(current_value);
}


Slider::~Slider()
{

}

bool Slider::Draw()
{
	App->render->Blit(texture, position.x - definition.rail_draw_rect.w * 0.5f , position.y - definition.rail_draw_rect.h * 0.5f, &definition.rail_draw_rect, false, 0.0f);
	return true;
}

int Slider::GetValue()
{
	return current_value;
}

void Slider::SetValue(int value)
{
	if (value > definition.max_value)
	{
		current_value = definition.max_value;
	}
	else
	{
		current_value = value;
	}

	current_value = value;
	int init_button_x = point_A + (current_value* definition.ditance) / 100;
	button->SetPosition(iPoint(init_button_x, position.y));
}

bool Slider::Update(float dt)
{
	point_A = position.x - definition.ditance * 0.5f;
	point_B = position.x + definition.ditance * 0.5f;

	iPoint pos = button->GetPosition();

	if (pos.x < point_A)
	{
		button->SetPosition(iPoint(point_A, position.y));
	}
	else if (pos.x > point_B)
	{
		button->SetPosition(iPoint(point_B, position.y));
	}
	else
	{
		button->SetPosition(iPoint( button->GetPosition().x, position.y));
	}

	current_value = ((pos.x - point_A) * definition.max_value) / definition.ditance;

	return true;
}



