#ifndef __LABEL_H__
#define __LABEL_H__

#include "j1Module.h"
#include "Object.h"
#include "p2Point.h"
#include "SDL_ttf/include/SDL_ttf.h"

class Label : public Object
{
public:

	Label(iPoint position, p2SString text, _TTF_Font* font, SDL_Color color, Gui_Listener* listener);

	// Destructor
	~Label();

	// Set the text
	void SetText(p2SString text);

	// Draw label
	bool Draw();

private:
	_TTF_Font*			font = nullptr;
	p2SString		    text;
	SDL_Color           color = { 255,255,255,255 };
};


#endif // __j1FONTS_H__