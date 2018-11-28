#ifndef __LABEL_H__
#define __LABEL_H__

#include "j1Module.h"
#include "Object.h"
#include "j1Gui.h"
#include "p2Point.h"
#include "SDL\include\SDL_pixels.h"


struct SDL_Texture;
struct _TTF_Font;


class Label : public Object
{
public:

	Label(iPoint position, p2SString text, _TTF_Font* font);

	// Destructor
	virtual ~Label();

	// Set the text
	void SetText();

	// Draw label
	bool Draw();

public:
	_TTF_Font*			font;
	p2SString		    text;

};


#endif // __j1FONTS_H__