#ifndef __LABEL_H__
#define __LABEL_H__

#include "j1Module.h"
#include "Object.h"
#include "p2Point.h"
#include "SDL_ttf/include/SDL_ttf.h"

class Label : public Object
{
public:

	Label(iPoint position, p2SString text, _TTF_Font* font, Gui_Listener* listener);

	// Destructor
	virtual ~Label();

	// Set the text
	void SetText(p2SString text);

	// Draw label
	bool Draw();

public:
	_TTF_Font*			font = nullptr;
	p2SString		    text;

};


#endif // __j1FONTS_H__