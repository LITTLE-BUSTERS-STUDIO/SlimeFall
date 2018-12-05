#ifndef _BUTTON_INPUT_H__
#define _BUTTON_INPUT_H__

#include "p2Point.h"
#include "Animation.h"
#include "Object.h"

class Button_Animation;

enum class Button_State
{
	pushed,
	normal
};

class Button_Input : public Object
{

public:

	Button_Input(iPoint position, Button_Animation animation, SDL_Texture * texture , Gui_Listener * listener);

	virtual ~Button_Input();

	bool Draw();

	bool ChangeState();

private:

	SDL_Texture * texture = nullptr;
	Button_State current_state = Button_State::normal;
	Button_Animation button_animation;
};

#endif // _BUTTON_INPUT_H__

