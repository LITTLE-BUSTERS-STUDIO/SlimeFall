#ifndef __Object_H__
#define __Object_H__

#include "p2Point.h"
#include "Animation.h"
#include "p2List.h"
#include "j1Module.h"

struct SDL_Texture;
class Gui_Listener;

class Object
{

public:
	Object(iPoint position /*, j1Module *listener*/);
	
	virtual ~Object();

	// Virtual methods ================================
	virtual bool Draw() { return true; };

	bool DegubDraw();

	// Variables ======================================
	bool hover_on = false;

	iPoint                position;
	Animation			  animation;
	SDL_Rect			  section;
	SDL_Texture         * texture = nullptr;
	Gui_Listener        * listener = nullptr;
};

#endif // __Object_H__
