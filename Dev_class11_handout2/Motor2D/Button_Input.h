#ifndef _BUTTON_INPUT_H__
#define _BUTTON_INPUT_H__

#include "p2Point.h"
#include "Animation.h"
#include "Object.h"

class Button_Input : public Object
{

public:

	Button_Input(iPoint position, Animation animation, SDL_Texture * texture , Gui_Listener * listener);

	virtual ~Button_Input();

	bool Draw();

private:

	SDL_Texture * texture = nullptr;
};

#endif // _BUTTON_INPUT_H__

