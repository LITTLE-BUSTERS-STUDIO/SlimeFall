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

	// Virtual methods ================================
	virtual bool Draw() { return true; };

};

#endif // __Entity_H__
