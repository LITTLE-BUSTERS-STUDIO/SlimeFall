#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "Animation.h"

#define CURSOR_WIDTH 2


// TODO 1: Create your structure of classes

// ---------------------------------------------------
class Object;
class Label;
class Image;
class _TTF_Font;

enum class Object_Type
{
	image,
	label
};


class j1Gui : public j1Module
{
public:

	j1Gui();

	virtual ~j1Gui();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

	// Creation functions ---------------------------------------------------------

	Label* CreateLabel(iPoint position, p2SString text, _TTF_Font* font );

	Image* CreateImage(iPoint position, Animation animation);

	// ----------------------------------------------------------------------------

	const SDL_Texture* GetAtlas() const;

private:

	// ---------------------------------------
	SDL_Texture* atlas;
	p2SString atlas_file_name;
	// ---------------------------------------
	p2List<Object*> objects_list;
	Object* selected = nullptr;
	bool show_cursor = false;
};

//private:
//
//	bool UpdateMouseIcon();
//
//	bool CheckClick();
//
//	bool DeleteObject();


#endif // __j1GUI_H__