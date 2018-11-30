#include "p2Defs.h"
#include "p2Log.h"
#include "Label.h"
#include "j1Render.h"
#include "j1Fonts.h"


Label::Label(iPoint position, p2SString text, _TTF_Font* font): Object(position)
{
	SetText(text);
	this->font = font;
}

Label::~Label()
{

}

void Label::SetText(p2SString text)
{
	text.create(text.GetString());

	App->font->CalcSize(text.GetString(), section.w, section.h, font);
	section.x = section.y = 0;
	
	SDL_Color color = { 255, 255, 255, 255 };
	texture = App->font->Print(text.GetString(), color, font);

}

bool Label::Draw()
{
	App->render->Blit(texture, position.x- section.w/2, position.y - section.h / 2, &section, 0.0f);
	DegubDraw();
	return false;
}






