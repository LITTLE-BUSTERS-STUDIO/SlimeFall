#ifndef _SLIDER_H__
#define _SLIDER_H__

#include "p2Point.h"
#include "Animation.h"
#include "Object.h"
#include "j1Gui.h"

struct Slider_Definition
{
	int       point_A = -1;
	int       point_B = -1;
	SDL_Rect  draw_rect = {0,0,0,0};
	float     default_value = 0;
};

class Slider : public Object, public Gui_Listener
{

public:

	Slider(iPoint position, Slider_Definition definition, SDL_Texture * texture, Gui_Listener * listener);

	virtual ~Slider();

	bool Draw();

	float GetValue();

	void SetValue(float value);

private:

	bool OnClick(Object* object);
	bool RepeatClcik(Object* object);
	bool OutClick(Object* object);

private:
	Button_Input*       button = nullptr;
	Slider_Definition   definition;
	float               value = 0;

private:
	friend j1Gui;
};

#endif // _SLIDER_H__
