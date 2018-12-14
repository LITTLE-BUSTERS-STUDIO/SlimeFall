#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "Animation.h"
#include "p2Point.h"

#define CURSOR_WIDTH 2

class Object;
enum class ObjectState;

class Gui_Listener;
class Image;
class Button_Input;
class Slider;
class Label;
class Checkbox;

struct Button_Definition;
struct Slider_Definition;
struct Checkbox_Definition;

enum class ClickState
{
	On,
	Out,
	Repeat,
	None
};

class j1Gui : public j1Module
{
public:

	j1Gui();

	virtual ~j1Gui();

	bool Awake(pugi::xml_node& node);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	SDL_Texture* GetAtlas() const;

	// Creation functions ---------------------------------------------------------
	Object* CreateObject(iPoint position, Gui_Listener* listener = nullptr);

	Label* CreateLabel(iPoint position, p2SString text, _TTF_Font* font, Gui_Listener* listener = nullptr ,SDL_Color color = {255,255,255,255});

	Image* CreateImage(iPoint position, Animation animation, Gui_Listener* listener = nullptr);

	Button_Input* CreateButton(iPoint position, Button_Definition definition, Gui_Listener* listener = nullptr);

	Slider* CreateSlider(iPoint position, Slider_Definition definition, Gui_Listener* listener = nullptr);

	Checkbox* CreateCheckbox(iPoint position, Checkbox_Definition definition, Gui_Listener* listener = nullptr);


	// Object functions ----------------------------------------------------------
	Object*  GetClickedObject();

	Object* GetScreen();

	bool DeleteObject(Object* object);

	void SetStateToBranch(const ObjectState state, Object* branch_root);

	// Cursor functions ----------------------------------------------------------

	iPoint GetCursorOffset() const;

	void SetCursorOffset(const iPoint offset);

	//bool UpdateMouseIcon();

private:

	bool SelectClickedObject();

	void DrawGui(Object* object); // Recursive function to draw gui as a tree

	void UpdateGuiPositions(Object* object, iPoint cumulated_position);

private:

	bool debug = false;

	// Atlas Texture ---------------------------------------
	SDL_Texture* atlas;

	p2SString atlas_file_name;

	// Objects ---------------------------------------------
	p2List<Object*> objects_list;

	Object* screen = nullptr;

	Object* clicked_object = nullptr;

	ClickState click_state = ClickState::None;

	// Cursor ----------------------------------------------
	iPoint cursor_position;

	iPoint cursor_offset;
	
	bool show_cursor = false; 
};


class Gui_Listener
{
public:
	Gui_Listener()
	{}

	virtual bool OnHover(Object* object) { return true; }
	virtual bool RepeatHover(Object* object) { return true; }
	virtual bool OutHover(Object* object) { return true; }

	virtual bool OnClick(Object* object) { return true; }
	virtual bool RepeatClick(Object* object) { return true; }
	virtual bool OutClick(Object* object) { return true; }
};



#endif // __j1GUI_H__