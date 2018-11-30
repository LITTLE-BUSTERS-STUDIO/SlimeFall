#include "Image.h"
#include "j1Render.h"
#include "j1Gui.h"

Image::Image(iPoint position, Animation animation, SDL_Texture * texture): Object(position)
{
	this->animation = animation;
	this->texture = texture;
}

Image::~Image()
{

}

bool Image::Draw()
{
	SDL_Rect anim_rect = animation.GetCurrentFrame();
	App->render->Blit( texture ,  position.x - anim_rect.w/2 , position.y - anim_rect.h / 2, &anim_rect);
	return true;
}

