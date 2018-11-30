#include "Button_Input.h"
#include "j1Render.h"
#include "j1Gui.h"


Button_Input::Button_Input(iPoint position, Animation animation, SDL_Texture * texture) : Object(position)
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
	App->render->Blit(texture, position.x - anim_rect.w / 2, position.y - anim_rect.h / 2, &anim_rect);
	return true;
}

