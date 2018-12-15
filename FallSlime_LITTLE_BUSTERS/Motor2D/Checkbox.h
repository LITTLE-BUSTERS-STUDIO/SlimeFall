#ifndef _CHECKBOX_H__
#define _CHECKBOX_H__

#include "j1Gui.h"
#include "p2Point.h"
#include "Animation.h"
#include "Object.h"
#include "Button_Input.h"

class Gui_Listener;

struct Checkbox_Definition
{
	bool                default_value = false;
	Button_Definition   check_on_button;
	Button_Definition   check_off_button;
};

class Checkbox : public Object, public Gui_Listener
{
public:

	Checkbox(const iPoint position, const Checkbox_Definition definition, SDL_Texture * texture, Gui_Listener * listener);

	virtual ~Checkbox();

	void SetValue(const bool value);

	bool GetValue();

private:

	bool OutClick(Object* object);

private:

	Checkbox_Definition     definition;
	bool                    value = false;

	// Components =================================

	Button_Input* button = nullptr;   

private:
	friend j1Gui;
};

#endif // _CHECKBOX_H__
