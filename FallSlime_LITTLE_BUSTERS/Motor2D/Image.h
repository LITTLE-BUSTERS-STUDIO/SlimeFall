#ifndef _IMAGE_H__
#define _IMAGE_H__

#include "p2Point.h"
#include "Animation.h"
#include "Object.h"

class Image: public Object
{

public:
	Image::Image(iPoint position, SDL_Rect draw_rect, SDL_Texture * texture, Gui_Listener* listener);

	Image (iPoint position, Animation animation, SDL_Texture * texture,  Gui_Listener* listener);

	virtual ~Image();

	bool Draw();

private:

	Animation animation;
};

#endif // _IMAGE_H__

