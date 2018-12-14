#include "Checkbox.h"
#include "j1Render.h"
#include  "p2Log.h"
#include "j1Input.h"
#include "Button_Input.h"


Checkbox::Checkbox(iPoint position, Checkbox_Definition definition, SDL_Texture * texture, Gui_Listener* listener) : Object(position, listener)
{
	this->definition = definition;
	this->texture = texture;

	Button_Definition def;

	if (definition.default_value)
	{
		def = definition.check_on_button;
	}
	else
	{
		def = definition.check_off_button;
	}

	button = App->gui->CreateButton(position, def, this);
	button->SetAnchor(this);


}

Checkbox::~Checkbox()
{

}

void Checkbox::SetValue(const bool value)
{
	this->value = value;
}

bool Checkbox::GetValue()
{
	return value;
}

bool Checkbox::OutClick(Object * object)
{
	if (object == button)
	{
		value = !value;

		if (value)
		{
			button->SetDefinition(definition.check_on_button);
		}
		else
		{
			button->SetDefinition(definition.check_off_button);
		}
	}
	return true;
}

