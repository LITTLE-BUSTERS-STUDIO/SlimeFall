#ifndef __Hud_H__
#define __Hud_H__

#include "j1Module.h"
#include "Animation.h"

struct SDL_Texture;

class Hud : public j1Module, public Gui_Listener
{
public:
	Hud();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

private:
	//--------------GUI-----------------------
	Image *				lives = nullptr;
	Image *				score = nullptr;
	Image *				wood_panel = nullptr;
	uint				hp;
	SDL_Texture*		tex_cursor;
};


#endif // !__Hud_H__

