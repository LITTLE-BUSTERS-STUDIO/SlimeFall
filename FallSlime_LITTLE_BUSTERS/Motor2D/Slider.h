#ifndef _SLIDER_H__
#define _SLIDER_H__

#include "p2Point.h"
#include "Animation.h"
#include "Object.h"
#include "j1Gui.h"

struct Slider_Definition
{
	int       point_A = 0;  // Left point
	int       point_B = 0;  // Right point
	int       fixed_y = 0;
	SDL_Rect  rail_draw_rect = { 0,0,0,0 };
	SDL_Rect  image_draw_rect = {0,0,0,0};
	int     default_value = 0;
	int     max_value = 100;
};

class Slider : public Object, public Gui_Listener
{

public:

	Slider(iPoint position, Slider_Definition definition, SDL_Texture * texture, Gui_Listener * listener);

	virtual ~Slider();

	bool Draw();

	float GetValue();

	void SetValue(float current_value);

private:

	bool Update(float dt);

private:
	Button_Input*       button = nullptr;
	Slider_Definition   definition;
	float               current_value = 0;
	float               points_distance = 0;

private:
	friend j1Gui;
};

#endif // _SLIDER_H__
