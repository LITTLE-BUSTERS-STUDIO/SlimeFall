#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "Label.h"


#include "SDL\include\SDL.h"
#include "SDL_TTF\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

Label::Label(iPoint position, p2SString text, _TTF_Font* font): Object(position)
{
	this->text = text;
	this->font = font;
}

// Destructor
Label::~Label()
{}

// Called before quitting
bool Label::CleanUp()
{
	return true;
}





