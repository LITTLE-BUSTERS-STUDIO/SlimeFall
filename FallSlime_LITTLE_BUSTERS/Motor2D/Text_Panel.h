#ifndef __TEXT_PANEL_H__
#define __TEXT_PANEL_H__

#include "j1Module.h"
#include "Object.h"
#include "p2Point.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "p2DynArray.h"


struct TextPanel_Definition
{
	p2SString		      text;
	_TTF_Font*			  font = nullptr;
	int                   line_spacing = 10;
	int                   width = 0;
	SDL_Color             color = { 255,255,255,255 };
};

class TextPanel : public Object
{
public:

	TextPanel(const iPoint position, TextPanel_Definition definition, Gui_Listener* listener);

	// Destructor
	~TextPanel();

	// Set the text
	void SetText(p2SString text);

	// Draw label
	bool Draw();

private:

	SDL_Texture*           text_texture = nullptr;
	TextPanel_Definition   definition;
	p2DynArray<SDL_Rect>   rects;
	uint                   lines = 0;
	int                    font_height = 0;
	int                    font_width = 0;
};


#endif // __TEXT_PANEL_H__
