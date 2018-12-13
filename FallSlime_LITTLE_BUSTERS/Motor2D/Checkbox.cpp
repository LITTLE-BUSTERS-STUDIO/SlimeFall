#include "Checkbox.h"
#include "j1Render.h"
#include "j1Gui.h"
#include  "p2Log.h"
#include "j1Input.h"


Checkbox::Checkbox(iPoint position, Button_Definition definition, SDL_Texture * texture, Gui_Listener* listener) : Object(position, listener)
{
	this->definition = definition;
	this->texture = texture;
}


Checkbox::~Checkbox()
{

}

bool Checkbox::Draw()
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

	section.w = anim_rect.w;
	section.h = anim_rect.h;

	App->render->Blit(texture, position.x - section.w / 2, position.y - section.h / 2, &anim_rect, false, 0.0f);

	return true;
}


