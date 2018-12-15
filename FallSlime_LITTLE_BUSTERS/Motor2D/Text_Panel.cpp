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


	//int last_line_x = 0;
	//uint first_line_letter = 0u;
	//int width = 0;
	//int height = 0;
	//int length = text.Length();
	//const char * char_text = text.GetString();

	//lines = 0;
	//App->font->CalcSize(text.GetString(), width, height, definition.font);

	//p2SString sub_string("");

	//for (uint i = 0; i < length ; ++i)
	//{
	//	if (char_text[i + 1] == ' ' || char_text[i + 1] == '\0')
	//	{
	//		text.SubString(first_line_letter, i, sub_string);
	//		App->font->CalcSize(sub_string.GetString(), width, height, definition.font);

	//		if (width > definition.width)
	//		{
	//			first_line_letter = i + 2;
	//			i = i + 2;
	//			+lines;
	//			last_line_x = last_line_x + width + 1;
	//			rects.PushBack({ last_line_x, 0 , width, height });
	//		}
	//	}
	//}

	//section.w = definition.width;
	//LOG("%i", section.w);
	//section.h = lines * (height + definition.line_spacing);
	//LOG("%i", section.h);

	text_texture = App->font->Print(text.GetString(), definition.color, definition.font);
}

bool TextPanel::Draw()
{


	return false;
}
