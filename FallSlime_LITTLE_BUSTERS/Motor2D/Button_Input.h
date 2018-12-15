#ifndef _BUTTON_INPUT_H__
#define _BUTTON_INPUT_H__

#include "p2Point.h"
#include "Animation.h"
#include "Object.h"
#include "j1Gui.h"

#define LABEL_PRESSED_OFFSET 4

class Label;
struct Button_Definition;

struct Button_Definition
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

class Button_Input : public Object, public Gui_Listener
{

public:

	Button_Input(const iPoint position, const Button_Definition definition, SDL_Texture * texture , Gui_Listener * listener);

	virtual ~Button_Input();

	bool Draw();

	bool SetLabel(const iPoint position, const p2SString text, _TTF_Font* font = nullptr, const SDL_Color color = {255,255,255,255});

	void SetDefinition(Button_Definition definition);

private:

	bool PreUpdate();

	bool Update(float dt);

private:
	Button_Definition	definition;

public:
	// Components =================================
	Label* label = nullptr;

private:
	friend j1Gui;
};

#endif // _BUTTON_INPUT_H__

