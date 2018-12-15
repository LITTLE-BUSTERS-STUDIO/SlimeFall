#include "p2Defs.h"
#include "p2Log.h"
#include "Text_Panel.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Textures.h"

TextPanel::TextPanel(const iPoint position, TextPanel_Definition definition, Gui_Listener * listener): Object(position, listener)
{
	section.x = section.y = 0;
	this->definition = definition;
	SetText(definition.text);
}

TextPanel::~TextPanel()
{
	if (text_texture != nullptr)
	{
		App->tex->UnLoad(text_texture);
	}
}

void TextPanel::SetText(p2SString text)
{
	if (text_texture != nullptr)
	{
		App->tex->UnLoad(text_texture);
	}

	lines = 0;
	int last_line_x = 0;
	int first_line_letter = 0;

	int w = 0, h = 0;
	int length = text.Length();
	const char * char_text = text.GetString();
	
	App->font->CalcSize(text.GetString(), w, h, definition.font);

	for (uint i = 0; i < length ; ++i)
	{
		if (char_text[i + 1] == ' ')
		{
		
			p2SString sub_string;
			text.SubString(first_line_letter, i, sub_string);
			App->font->CalcSize(sub_string.GetString(), w, h, definition.font);

			if (w > definition.width)
			{
				first_line_letter = i + 2;
				i = i + 2;
				+lines;
				last_line_x = last_line_x + w + 1;
				rects.PushBack({ last_line_x, 0 , w, h});
			}
		}
	}

	section.h = lines * (h + definition.line_spacing);
	section.w = definition.width;

	text_texture = App->font->Print(text.GetString(), definition.color, definition.font);
}

bool TextPanel::Draw()
{


	return false;
}
