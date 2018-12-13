#ifndef _BUTTON_INPUT_H__
#define _BUTTON_INPUT_H__

#include "p2Point.h"
#include "Animation.h"
#include "Object.h"

class Button_Definition;

class Button_Definition
{
public:
	Button_Definition() {}
	Button_Definition(SDL_Rect idle_rect, SDL_Rect hover_rect, SDL_Rect pushed_rect)
	{
		this->idle_rect = idle_rect;
		this->hover_rect = hover_rect;
		this->pushed_rect = pushed_rect;
	}

	SDL_Rect idle_rect;
	SDL_Rect pushed_rect;
	SDL_Rect hover_rect;
};

class Button_Input : public Object
{

public:

	Button_Input(iPoint position, Button_Definition definition, SDL_Texture * texture , Gui_Listener * listener);

	virtual ~Button_Input();

	bool Draw();

	bool SetLabel(/*Position position ,*/p2SString text/* , Color color ,*/);

private:

	SDL_Texture * texture = nullptr;
	Button_Definition definition;

	// Components =================================
	//Label* label = nullptr;
};

#endif // _BUTTON_INPUT_H__

