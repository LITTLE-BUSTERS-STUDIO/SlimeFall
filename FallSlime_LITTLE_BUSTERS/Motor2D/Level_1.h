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
};

#endif // __LEVEL_1_H__
