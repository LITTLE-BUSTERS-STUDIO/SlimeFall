#include "Slider.h"
#include "j1Render.h"
#include  "p2Log.h"
#include "j1Input.h"


Slider::Slider(iPoint position, Slider_Definition definition, SDL_Texture * texture, Gui_Listener* listener) : Object(position, listener)
{
	this->definition = definition;
	this->texture = texture;

	points_distance = definition.point_B - definition.point_A;
	current_value = definition.default_value;

	App->gui->CreateImage(position, definition.image_draw_rect, this);
}


Slider::~Slider()
{

}

bool Slider::Draw()
{
	App->render->Blit(texture, position.x - section.w / 2, position.y - section.h / 2, &definition.image_draw_rect, false, 0.0f);

	return true;
}

float Slider::GetValue()
{
	return 0.0f;
}

void Slider::SetValue(float current_value)
{
}

bool Slider::Update(float dt)
{


	return true;
}

//bool Slider::OnClick(Object * object)
//{
//	button->IsDraggable(true);
//	return true;
//}
//
//bool Slider::RepeatClcik(Object * object)
//{
//	
//
//	return true;
//}
//
//bool Slider::OutClick(Object * object)
//{
//	button->IsDraggable(false);
//	return true;
//}
