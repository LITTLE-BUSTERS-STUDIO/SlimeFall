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

	point_A = position.x - definition.ditance * 0.5f;
	point_B = position.x + definition.ditance * 0.5f;
	current_value = definition.default_value;

	button = App->gui->CreateButton(position, definition.button_definition, this);
	int init_button_x = point_A + (current_value* definition.ditance) / 100;

	button->SetPosition(iPoint(init_button_x, position.y));
}


Slider::~Slider()
{

}

bool Slider::Draw()
{
	//App->render->Blit(texture, position.x - section.w / 2, position.y - section.h / 2, &definition.rail_draw_rect, false, 0.0f);
	SDL_Rect rect = { point_A - definition.ditance* 0.5f , position.y , definition.ditance , 20 };
	App->render->DrawQuad(rect, 255 ,255, 255, 255, true, false);
	return true;
}

int Slider::GetValue()
{
	return current_value;
}

void Slider::SetValue(int value)
{
}

bool Slider::Update(float dt)
{
	
	return true;
}

