#include "p2Defs.h"
#include "p2Log.h"
#include "Label.h"
#include "j1Render.h"
#include "j1Fonts.h"


Label::Label(iPoint position, p2SString text, _TTF_Font* font): Object(position)
{
	this->text = text;
	this->font = font;
}

Label::~Label()
{

}

void Label::SetText(p2SString text)
{
	text.create(text.GetString());
}

bool Label::Draw()
{
	SDL_Rect rect;
	App->font->CalcSize(text.GetString(), rect.w, rect.h, font);
	rect.x = rect.y = 0;

	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Texture * texture = App->font->Print(text.GetString(), color, font);
	App->render->Blit(texture, position.x- rect.w/2, position.y - rect.h / 2, &rect);
	return false;
}






