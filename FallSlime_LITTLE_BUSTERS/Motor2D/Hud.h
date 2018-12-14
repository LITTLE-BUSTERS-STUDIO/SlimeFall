#ifndef __Hud_H__
#define __Hud_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Gui.h"

#define MAX_LIVES 5
#define MAX_COINS 3

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
	// Gui ===================================
	Object *		    hud_object = nullptr;
	p2List<Image*>		lives_list;
	p2List<Image*>		coins_list;

	Image *				wood_panel = nullptr;

	// Vars ===================================
	int 				lives_counter;
	int				    coin_counter;
	bool				game_over;

public: 
	int				SubstractLife();
	int				AddCoin();
};



#endif // !__Hud_H__

