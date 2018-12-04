#include "Button_Input.h"
#include "j1Render.h"
#include "j1Gui.h"
#include  "p2Log.h"


Button_Input::Button_Input(iPoint position, Animation animation, SDL_Texture * texture, Gui_Listener* listener) : Object(position, listener)
{
	this->animation = animation;
	this->texture = texture;
}


Button_Input::~Button_Input()
{

}

bool Button_Input::Draw()
{
	SDL_Rect anim_rect = animation.GetCurrentFrame();

	section.w = anim_rect.w;
	section.h = anim_rect.h;
	if (hover_on)
		LOG("Hi");
	else
		App->render->Blit(texture, position.x - section.w / 2, position.y - section.h / 2, &anim_rect, 0.0f);
	DegubDraw();
	return true;
}

