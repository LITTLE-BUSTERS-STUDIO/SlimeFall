#include "j1App.h"
#include "j1Render.h"
#include "Object.h"
#include "j1Gui.h"

Object::Object(iPoint position, Gui_Listener *listener)
{
	this->position = position;
	this->listener = listener;
}

Object::~Object()
{
}

bool Object::DegubDraw()
{
	if (App->gui->debug)
	{
		SDL_Rect rect;

		rect.x = position.x - section.w / 2;
		rect.y = position.y - section.h / 2;
		rect.w = section.w;
		rect.h = section.h;

		if (hover_on)
		{
			App->render->DrawQuad(rect, 255, 0, 0, 100, true, false);
		}
		else
		{
			App->render->DrawQuad(rect, 255, 100, 40, 100, true, false);
		}
	
	}
	return true;
}
