#include "Image.h"
#include "j1Render.h"
#include "j1Gui.h"

Image::Image(iPoint position, Animation animation): Object(position)
{
	this->animation = animation;
}

Image::~Image()
{

}

bool Image::Draw()
{
	App->render->Blit( App->gui->GetAtlas() ,  position.x, position.y , &animation.GetCurrentFrame());
}

