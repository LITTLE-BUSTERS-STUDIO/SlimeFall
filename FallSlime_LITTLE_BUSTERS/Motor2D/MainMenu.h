#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "j1Module.h"
#include "j1Scene.h"
#include "j1Gui.h"

enum class MenuSection : int {
	min = -1,
	credits = 0,
	main_menu = 1,
	settings = 2,
	max = 3
};

//enum class Direction
//{
//	left,
//	right,
//	none
//};

struct SDL_Texture;

class MainMenu : public j1Scene , public Gui_Listener
{
public:

	MainMenu();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	//Scene Load & Unload ===============

	bool LoadScene(pugi::xml_node& node);

	bool UnloadScene();

	bool OnClick(Object* object);

	bool OutClick(Object* object);

private:
	bool MoveToSection(MenuSection menu_section);

private:
	// Values =========================================
	bool                exit = false; 
	fPoint              move_to_point[(int)MenuSection::max];
	fPoint              camera_pos = { 0.0f, 0.0f };
	float               camera_speed = 0.0f;
	fPoint              camera_velocity = { 0.0f , 0.0f };
	MenuSection         current_section = MenuSection::main_menu;

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
	Image*              credits_panel = nullptr;
	Image*              settings_image = nullptr;

	Button_Input*       button_return_settings = nullptr;
	Button_Input*       button_return_credits = nullptr;

	Label*              settings_label = nullptr;
	Label*              music_volume_label = nullptr;
	Label*              sfx_volume_label = nullptr;
	Label*              limitate_fps_label = nullptr;
	Label*              mute_label = nullptr;

	Slider*             slider_music_volume = nullptr;
	Slider*             slider_sfx_volume = nullptr;

	Checkbox*           checkbox_mute = nullptr;
	Checkbox*           checkbox_limitate_fps = nullptr;

	iPoint				settings_panel_pos;

	// --------- Credits -----------------------

	TextPanel*          license_text_panel = nullptr;



	// Scene ==========================================
	p2SString			music_path;
	p2SString			credits_music_path;
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

};

#endif // __MAINMENU_H__

