#ifndef __LEVEL_1_H__
#define __LEVEL_1_H__

#include "j1Module.h"
#include "j1Scene.h"
#include "j1Gui.h"

struct SDL_Texture;

class Level_1 : public j1Scene, public Gui_Listener
{
public:

	Level_1();

	 bool Update(float dt);

	 bool PostUpdate();

	//Scene Load & Unload ===============

	 bool LoadScene(pugi::xml_node& node);

	 bool UnloadScene();

	 bool OnClick(Object* object);

	 bool OutClick(Object* object);

	 bool PauseScene();

	 bool ResumeScene();

private:

	//-------------VARS----------------------
	p2SString			music_path;
	SDL_Texture         *paralax_tex_1 = nullptr;
	SDL_Texture         *paralax_tex_2 = nullptr;
	SDL_Texture         *paralax_tex_3 = nullptr;
	Parallax            parallax1[4];
	Parallax            parallax2[4];
	Parallax            parallax3[3];
	float	           	parallax_speed_11;
	float		        parallax_speed_22;
	float				parallax_speed_33;
	uint				background_width;
	uint				background_high;
	uint				max_background_layers;
	uint				background_startpos;
	
	//------------Paused Menu----------------
	Object*             paused_menu = nullptr;
	_TTF_Font*          karma_font_buttons = nullptr;
	Button_Input*       button_back = nullptr;
	Button_Input*       button_save = nullptr;
	Button_Input*       button_load = nullptr;
	Button_Input*       button_exit_to_menu = nullptr;
};

#endif // __LEVEL_1_H__
