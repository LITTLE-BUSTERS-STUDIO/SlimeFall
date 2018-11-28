#include "p2Defs.h"
#include "p2Log.h"
#include "Label.h"

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





