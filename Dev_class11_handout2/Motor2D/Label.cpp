#include "p2Defs.h"
#include "p2Log.h"
#include "Label.h"
#include "j1Fonts.h"

Label::Label(iPoint position, p2SString text, _TTF_Font* font): Object(position)
{
	this->text = text;
	this->font = font;
}

// Destructor
Label::~Label()
{

}

void Label::SetText()
{
}

bool Label::Draw()
{
	SDL_Color color = { 255, 255, 255 };
	App->font->Print(text.GetString(), color, font);
	return false;
}

// Called before quitting
bool Label::CleanUp()
{
	return true;
}





