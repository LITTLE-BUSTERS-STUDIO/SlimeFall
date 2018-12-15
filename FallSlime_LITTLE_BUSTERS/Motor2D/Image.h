#ifndef _IMAGE_H__
#define _IMAGE_H__

#include "p2Point.h"
#include "Animation.h"
#include "Object.h"

class Image: public Object
{

public:
	Image(const iPoint position, const SDL_Rect draw_rect, SDL_Texture * texture, Gui_Listener* listener);

	Image (const iPoint position, const Animation animation, SDL_Texture * texture,  Gui_Listener* listener);

	virtual ~Image();

	bool Draw();

	void SetAnimationFrame( int frame);

private:

	Animation animation;
};

#endif // _IMAGE_H__

