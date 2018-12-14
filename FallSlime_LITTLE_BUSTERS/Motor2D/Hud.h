#ifndef __Hud_H__
#define __Hud_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Gui.h"

struct SDL_Texture;
class Image;
class Object;

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

	bool HideHud();

	bool ShowHud();

private:
	//--------------GUI-----------------------
	Image *				hud_object = nullptr;
	Image *				lives_1 = nullptr;
	Image *				lives_2 = nullptr;
	Image *				lives_3 = nullptr;
	Image *				lives_4 = nullptr;
	Image *				score_1 = nullptr;
	Image *				score_2 = nullptr;
	Image *				score_3 = nullptr;
	Image *				wood_panel = nullptr;
	SDL_Texture*		tex_cursor;
	uint				hp_counter;

public: 
	uint				SubstractLife();
};



#endif // !__Hud_H__

