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

	bool Update(float dt);

	bool PostUpdate();

	//Scene Load & Unload ===============

	bool LoadScene(pugi::xml_node& node);

	bool UnloadScene();

	bool OnClick(Object* object);

	bool OutClick(Object* object);

private:
	// Values =========================================
	bool                exit = false; 
	// GUI ============================================
	Object*             menu = nullptr;
	_TTF_Font*          karma_font_settings = nullptr;
	_TTF_Font*          karma_font_buttons = nullptr;

	// ---------- Menu -------------------------
	Image*              logo = nullptr;

	Button_Input*       button_new_game = nullptr;
	Button_Input*       button_continue = nullptr;
	Button_Input*       button_exit = nullptr;

	Button_Input*       button_credits = nullptr;
	Button_Input*       button_settings = nullptr;
	Button_Input*       button_web = nullptr;

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

	iPoint				settings_panel_pos;

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
	int			     	background_width;
	int				    background_high;
	int			    	max_background_layers;
	int				    background_startpos;
	fPoint				camera_position;
	MainMenu_States		current_state = MainMenu_States::main_menu;
};

#endif // __MAINMENU_H__

