#ifndef _IMAGE_H__
#define _IMAGE_H__

#include "p2Point.h"
#include "Animation.h"
#include "Object.h"

class Image: public Object
{

public:

	Image (iPoint position, Animation animation, SDL_Texture * texture = nullptr);

	virtual ~Image();

	bool Draw();

private:

	SDL_Texture* texture = nullptr;
};

#endif // _IMAGE_H__

