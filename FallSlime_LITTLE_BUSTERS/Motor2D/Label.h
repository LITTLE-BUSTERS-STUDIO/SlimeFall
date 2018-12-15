#ifndef __LABEL_H__
#define __LABEL_H__

#include "j1Module.h"
#include "Object.h"
#include "p2Point.h"
#include "SDL_ttf/include/SDL_ttf.h"

class Button_Input;

class Label : public Object
{
public:

	Label(const iPoint position, const p2SString text, _TTF_Font* font, const SDL_Color color, Gui_Listener* listener);

	// Destructor
	~Label();

	// Set the text
	void SetText(p2SString text);

	// Draw label
	bool Draw();

private:
	SDL_Texture*        label_texture = nullptr;
	_TTF_Font*			font = nullptr;
	p2SString		    text;
	SDL_Color           color = { 255,255,255,255 };

private:
	friend Button_Input;
};


#endif // __LABEL_H__