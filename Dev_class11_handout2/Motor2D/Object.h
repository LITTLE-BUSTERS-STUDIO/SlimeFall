#ifndef __Object_H__
#define __Object_H__

#include "p2Point.h"
#include "Animation.h"
#include "p2List.h"

struct SDL_Texture;


class Object
{

public:
	Object(iPoint position);
	
	virtual ~Object();

	iPoint                position;
	Animation			  animation;
	SDL_Rect			  section;
	SDL_Texture         * texture = nullptr;

	// Virtual methods ================================
	virtual bool Draw() { return true; };

	bool DegubDraw();
};

#endif // __Object_H__
