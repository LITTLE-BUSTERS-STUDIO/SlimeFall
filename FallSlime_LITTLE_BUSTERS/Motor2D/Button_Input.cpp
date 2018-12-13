#include "Button_Input.h"
#include "j1Render.h"
#include "j1Gui.h"
#include  "p2Log.h"
#include "j1Input.h"


Button_Input::Button_Input(iPoint position, Button_Definition button_animation, SDL_Texture * texture, Gui_Listener* listener) : Object(position, listener)
{
	this->button_animation = button_animation;
	this->texture = texture;
}


Button_Input::~Button_Input()
{

}

bool Button_Input::Draw()
{
	SDL_Rect anim_rect;

	if (App->gui->GetClickedObject() == this)
	{
		anim_rect = button_animation.pushed_rect;
	}
	else if (hover_state == HoverState::Repeat || hover_state == HoverState::On)
	{
		anim_rect = button_animation.hover_rect;
	}
	else
	{
		anim_rect = button_animation.idle_rect;
	}

	section.w = anim_rect.w;
	section.h = anim_rect.h;
	
	App->render->Blit(texture, position.x - section.w/2 , position.y - section.h/2, &anim_rect, false, 0.0f);

	return true;
}


