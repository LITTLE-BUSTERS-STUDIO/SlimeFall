#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "j1Module.h"
#include "j1Scene.h"
#include "j1Gui.h"

class Image;
struct SDL_Texture;

class MainMenu : public j1Scene , public Gui_Listener
{
public:

	MainMenu();

	bool Update(float dt);

	bool PostUpdate();

	//Scene Load & Unload ===============

	bool LoadScene(pugi::xml_node& node);

	bool UnloadScene();

private:

	// GUI ============================================
	Image*              logo = nullptr;
	Button_Input*       button_start = nullptr;
	// Scene ==========================================
	p2SString			music_path;
	SDL_Texture         *paralax_tex_1 = nullptr;
	SDL_Texture         *paralax_tex_2 = nullptr;
	SDL_Texture         *paralax_tex_3 = nullptr;
	Parallax            parallax1[4];
	Parallax            parallax2[4];
	Parallax            parallax3[3];
	float	           	parallax_speed_1;
	float		        parallax_speed_2;
	float				parallax_speed_3;
	uint				background_width;
	uint				background_high;
	uint				max_background_layers;
	uint				background_startpos;
};

#endif // __MAINMENU_H__

