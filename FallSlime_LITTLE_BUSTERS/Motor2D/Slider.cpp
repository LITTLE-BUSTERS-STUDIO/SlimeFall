#include "Slider.h"
#include "j1Render.h"
#include  "p2Log.h"
#include "j1Input.h"


Slider::Slider(iPoint position, Slider_Definition definition, SDL_Texture * texture, Gui_Listener* listener) : Object(position, listener)
{
	this->definition = definition;
	this->texture = texture;
}


Slider::~Slider()
{

}

bool Slider::Draw()
{
	App->render->Blit(texture, position.x - section.w / 2, position.y - section.h / 2, &definition.draw_rect, false, 0.0f);

	return true;
}

float Slider::GetValue()
{
	return 0.0f;
}

void Slider::SetValue(float value)
{
}

bool Slider::OnClick(Object * object)
{
	return false;
}

bool Slider::RepeatClcik(Object * object)
{

	return true;
}

bool Slider::OutClick(Object * object)
{
	return false;
}
