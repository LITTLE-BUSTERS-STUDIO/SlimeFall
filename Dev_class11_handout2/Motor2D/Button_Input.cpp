#include "Button_Input.h"
#include "j1Render.h"
#include "j1Gui.h"
#include  "p2Log.h"
#include "j1Input.h"


Button_Input::Button_Input(iPoint position, Button_Animation button_animation, SDL_Texture * texture, Gui_Listener* listener) : Object(position, listener)
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

	switch (current_state)
	{
	case Button_State::normal:
		anim_rect = button_animation.idle_rect;
		if(hover_on)
			anim_rect = button_animation.hover_rect;
		break;
	case Button_State::pushed:
		anim_rect = button_animation.pushed_rect;
		if (!hover_on) //TO DO: Change this hardcoded!!!
			current_state = Button_State::normal;
		break;
	default:
		break;
	}

	section.w = anim_rect.w;
	section.h = anim_rect.h;
	
	App->render->Blit(texture, position.x - section.w / 2, position.y - section.h / 2, &anim_rect, 0.0f);
	DegubDraw();
	return true;
}

bool Button_Input::ChangeState()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		current_state = Button_State::pushed;
	}
	else  
		current_state =  Button_State::normal;
	
	return false;
}

