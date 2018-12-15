#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "j1Module.h"
#include "j1Scene.h"
#include "j1Gui.h"

#define CAMERA_OFFSET 640
struct SDL_Texture;
enum class MainMenu_States
{
	main_menu,
	settings,
	credits
};

class MainMenu : public j1Scene , public Gui_Listener
{
public:

	MainMenu();

	virtual bool Start();

	bool Update(float dt);

	bool PostUpdate();

	//Scene Load & Unload ===============

	bool LoadScene(pugi::xml_node& node);

	bool UnloadScene();

	bool OutClick(Object* object);

private:

	// GUI ============================================
	Object*             menu = nullptr;
	_TTF_Font*          karma_font = nullptr;

	// ---------- Menu -------------------------
	Image*              logo = nullptr;

	Button_Input*       button_play = nullptr;
	Button_Input*       button_continue = nullptr;
	Button_Input*       button_exit = nullptr;

	Button_Input*       button_credits = nullptr;
	Button_Input*       button_settings = nullptr;

	// ---------- Settings ----------------------
	Image*              settings_panel = nullptr;

	Button_Input*       button_return_settings = nullptr;

	Label*              music_volume_label = nullptr;
	Label*              sfx_volume_label = nullptr;
	Label*              limitate_fps_label = nullptr;
	Label*              mute_label = nullptr;

	Slider*             slider_music_volume = nullptr;
	Slider*             slider_sfx_volume = nullptr;

	Checkbox*           checkbox_mute = nullptr;
	Checkbox*           checkbox_limitate_fps = nullptr;


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
	fPoint				camera_position;
	MainMenu_States		current_state = MainMenu_States::main_menu;
};

#endif // __MAINMENU_H__

