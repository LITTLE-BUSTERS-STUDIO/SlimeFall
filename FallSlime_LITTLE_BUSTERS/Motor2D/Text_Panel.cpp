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

	p2SString txt(text.GetString());

	int last_line_x = 0;
	unsigned int first_line_letter = 0;
	int width = 0;
	int height = 0;
	int length = txt.Length();
	const char * char_text = txt.GetString();

	lines = 0;
	App->font->CalcSize(txt.GetString(), width, height, definition.font);

	

	for (uint i = 0; i < length ; ++i)
	{
		if (char_text[i + 1] == ' ' || char_text[i + 1] == '\0')
		{
			p2SString str(txt);
			str.Cut(i + 1, length);
			if (first_line_letter != 0)
			{
				str.Cut(0, first_line_letter -1);
			}
		

			LOG("%s", str.GetString());
			App->font->CalcSize(str.GetString(), width, height, definition.font);

			if (width > definition.width)
			{
				first_line_letter = i + 2;
				i = i + 2;
				+lines;
				rects.PushBack({ last_line_x, 0 , width, height });
				last_line_x = last_line_x + width + 1;
			}
		}
	}

	section.w = definition.width;
	LOG("%i", section.w);
	section.h = lines * (height + definition.line_spacing);
	LOG("%i", section.h);

	text_texture = App->font->Print(text.GetString(), definition.color, definition.font);
}

bool TextPanel::Draw()
{


	return false;
}
