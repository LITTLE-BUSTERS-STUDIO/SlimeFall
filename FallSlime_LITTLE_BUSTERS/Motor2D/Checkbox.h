#ifndef _CHECKBOX_H__
#define _CHECKBOX_H__

#include "p2Point.h"
#include "Animation.h"
#include "Object.h"
#include "Button_Input.h"

class Checkbox : public Object
{
public:

	Checkbox(iPoint position, Button_Definition definition, SDL_Texture * texture, Gui_Listener * listener);

	virtual ~Checkbox();

	bool Draw();

	bool SetLabel(/*Position position ,*/p2SString text/* , Color color ,*/);

private:

	SDL_Texture * texture = nullptr;
	Button_Definition definition;

	// Components =================================
	//Label* label = nullptr;
};

#endif // _CHECKBOX_H__
