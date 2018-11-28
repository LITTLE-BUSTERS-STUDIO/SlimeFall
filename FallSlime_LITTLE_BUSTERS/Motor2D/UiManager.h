#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "j1Module.h"

class Object;

enum class Object_Type
{
	Test
};

class UiManager : public j1Module
{
public:
	UiManager();

	~UiManager();

	bool Awake(pugi::xml_node&);

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

private:

	bool UpdateMouseIcon();

	bool CheckClick();

	bool DeleteObject();

public:

	p2List<Object*> objects_list;
	Object* selected = nullptr;
	bool show_cursor = false;

};

#endif