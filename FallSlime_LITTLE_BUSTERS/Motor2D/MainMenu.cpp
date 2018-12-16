#include "p2Defs.h"
#include "p2Log.h"
#include "MainMenu.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "SceneManager.h"
#include "Hud.h"
#include "EntityManager.h"
#include "Brofiler/Brofiler.h"


#include "Label.h"
#include "Checkbox.h"
#include "Image.h"
#include "Button_Input.h"
#include "Slider.h"
#include "Text_Panel.h"


MainMenu::MainMenu() : j1Scene()
{
	name.create("main_menu");
	is_pausable = false;
	App->gui->show_cursor = true;
}

bool MainMenu::PreUpdate()
{

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		MoveToSection(MenuSection::main_menu);
	}

	return true;
}

bool MainMenu::Update(float dt)
{
	BROFILER_CATEGORY("MainMenu Update", Profiler::Color::MediumBlue);

	if (camera_velocity.x < 0.0f)
	{
		if (camera_pos.x > move_to_point[(int)current_section].x)
		{
			camera_pos.x += camera_velocity.x *dt;
		}

		if (camera_pos.x < move_to_point[(int)current_section].x)
		{
			camera_pos.x = move_to_point[(int)current_section].x;
			camera_velocity.x = 0.0f;
		}

	}
	else if (camera_velocity.x >  0.0f)
	{
		if (camera_pos.x < move_to_point[(int)current_section].x)
		{
			camera_pos.x += camera_velocity.x *dt;
		}

		if (camera_pos.x > move_to_point[(int)current_section].x)
		{
			camera_pos.x = move_to_point[(int)current_section].x;
			camera_velocity.x = 0.0f;
		}
	}
	else
	{
		camera_pos.x = move_to_point[(int)current_section].x;
	}


	App->render->camera.x = camera_pos.x;
	App->render->camera.y = camera_pos.y;


	menu->SetPosition(iPoint(-(App->render->camera.x / (int)App->win->GetScale()), 0));

	return true;
}

bool MainMenu::PostUpdate()
{
	BROFILER_CATEGORY("MainMenu PostUpdate", Profiler::Color::MediumBlue);

	// Blit background--------------------------------------
	int speed = 0;

	for (uint i = 0; i < max_background_layers; i++)
		App->render->Blit(paralax_tex_1, 0, background_startpos, &parallax1[i].rect_parallax, false, 0.0f);

	for (uint i = 0; i < max_background_layers; i++)
	{
		if (i == 0)
			App->render->Blit(paralax_tex_2, 0, background_startpos, &parallax2[i].rect_parallax, false, 0.0f);
		else if (i > 0)
			App->render->Blit(paralax_tex_2, 0, background_startpos, &parallax2[i].rect_parallax, false, 0.0f);
	}

	for (uint i = 0; i < max_background_layers; i++)
		App->render->Blit(paralax_tex_3, 0, background_startpos, &parallax3[i].rect_parallax, false, 0.0f);


	App->map->Draw();

	return true;
}

bool MainMenu::LoadScene(pugi::xml_node & node)
{
	BROFILER_CATEGORY("MainMenu Load", Profiler::Color::Maroon);

	LOG("Loading MainMenu");
	
	App->hud->HideHud();
	App->pause_game = false;

	// Menu Secions ===================================================
 	move_to_point[(int)MenuSection::credits].create(0, 0);
	move_to_point[(int)MenuSection::main_menu].create(1280, 0);
	move_to_point[(int)MenuSection::settings].create(1280*2 , 0);

	App->render->camera.x = 1280;
	camera_pos.create(1280, 0);
	camera_speed = 1515.0f;

	// Music ========================================================
	music_path = node.child("music").attribute("path").as_string("");
	credits_music_path = node.child("credits").attribute("path").as_string("");
	App->audio->PlayMusic(music_path.GetString());

	// Paralax ========================================================
	paralax_tex_1 = App->tex->Load(node.child("background1").attribute("path").as_string(""));
	paralax_tex_2 = App->tex->Load(node.child("background2").attribute("path").as_string(""));
	paralax_tex_3 = App->tex->Load(node.child("background3").attribute("path").as_string(""));

	background_width = node.child("background_dimension").attribute("width").as_uint(0u);
	background_high = node.child("background_dimension").attribute("high").as_uint(0u);
	max_background_layers = node.child("max_background_layers").attribute("value").as_uint(0u);
	background_startpos = node.child("background_startpos").attribute("value").as_uint(0u);


	for (int i = 0; i < 4; i++)
	{
		parallax1[i].rect_parallax.x = background_width * i;
		parallax1[i].rect_parallax.y = 0;
		parallax1[i].rect_parallax.w = background_width;
		parallax1[i].rect_parallax.h = background_high;

		parallax2[i].rect_parallax.x = background_width * i;
		parallax2[i].rect_parallax.y = 0;
		parallax2[i].rect_parallax.w = background_width;
		parallax2[i].rect_parallax.h = background_high;

		parallax3[i].rect_parallax.x = background_width * i;
		parallax3[i].rect_parallax.y = 0;
		parallax3[i].rect_parallax.w = background_width;
		parallax3[i].rect_parallax.h = background_high;
	}

	parallax_speed_1 = node.child("parallax_speed").attribute("low").as_float(0.0f);
	parallax_speed_2 = node.child("parallax_speed").attribute("medium").as_float(0.0f);
	parallax_speed_3 = node.child("parallax_speed").attribute("high").as_float(0.0f);

	menu = App->gui->CreateObject(iPoint( 0,0), this);
	karma_font_settings = App->font->Load("fonts/KarmaSuture.ttf", 24);

	SDL_Color color = { 231,94,152,255 };

	// =============================================================
	// ====================   Menu   ===============================
	// =============================================================

	// Images ============================================
	Animation logo_anim;
	logo_anim.PushBack({ 0, 0, 219, 94 });
	logo = App->gui->CreateImage(iPoint(340 , 84), logo_anim, this);
	logo->SetAnchor(menu);

	// Buttons ============================================
	Button_Definition button_rectangle({ 219 , 0, 122, 36 }, { 219, 36, 122, 36 }, { 219, 72, 122, 36 });
	button_new_game = App->gui->CreateButton(iPoint(320 , 208), button_rectangle, this);
	button_new_game->SetLabel(iPoint(320 , 204), p2SString("NEW GAME"), karma_font_settings, color);
	button_new_game->SetAnchor(logo);

	button_continue = App->gui->CreateButton(iPoint(320, 250), button_rectangle, this);
	button_continue->SetLabel(iPoint(320 , 246), p2SString("CONTINUE"), karma_font_settings, color);
	button_continue->SetAnchor(logo);

	if (!App->save_doc_exist)
	{
		button_continue->SetState(ObjectState::locked);
	}

	button_exit = App->gui->CreateButton(iPoint(320, 292), button_rectangle, this);
	button_exit->SetLabel(iPoint(320 , 288), p2SString("EXIT"), karma_font_settings, color);
	button_exit->SetAnchor(logo);

	Button_Definition button_def_credits({ 866 ,0, 42,45 }, { 866 ,45, 42,45 }, { 866 ,90, 42,45 });
	button_credits = App->gui->CreateButton(iPoint(84 , 329), button_def_credits, this);
	button_credits->SetAnchor(logo);

	Button_Definition button_def_web({ 954 ,0, 42,45 }, { 954 ,45, 42,45 }, { 954 ,90, 42,45 });
	button_web = App->gui->CreateButton(iPoint(141 , 329), button_def_web, this);
	button_web->SetAnchor(logo);

	Button_Definition button_def_settings({ 910  ,0, 42,45 }, { 910 ,45, 42,45 }, { 910 ,90, 42,45 });
	button_settings = App->gui->CreateButton(iPoint(555 , 329), button_def_settings, this);
	button_settings->SetAnchor(logo);

	logo->SetPosition(iPoint(320 + 640, logo->GetPosition().y));

	// =============================================================
    // ==================   Settings   =============================
    // =============================================================

	// Images ============================================
	Animation panel_anim;
	panel_anim.PushBack({ 387, 0, 389, 293 });
	settings_panel = App->gui->CreateImage(iPoint(320 , 182), panel_anim, this);
	settings_panel->SetAnchor(menu);

	Animation settings_anim;
	settings_anim.PushBack({ 915, 5, 31, 30 });
	settings_image = App->gui->CreateImage(iPoint(363 , 78), settings_anim, this);
	settings_image->SetAnchor(settings_panel);

	// Buttons ============================================
	Button_Definition button_def_return_settings({ 778 ,0, 42,45 }, { 778 ,45, 42,45 }, { 778 ,90, 42,45 });
	button_return_settings = App->gui->CreateButton(iPoint(320, 318), button_def_return_settings, this);
	button_return_settings->SetAnchor(settings_panel);


	// Labels ============================================
	settings_label = App->gui->CreateLabel(iPoint(295 , 75), "Settings", karma_font_settings, this, color);
	settings_label->SetAnchor(settings_panel);
	music_volume_label = App->gui->CreateLabel(iPoint(229 , 130), "Music volume", karma_font_settings, this, color);
	music_volume_label->SetAnchor(settings_panel);
	sfx_volume_label = App->gui->CreateLabel(iPoint(218 , 173), "Sfx volume", karma_font_settings, this, color);
	sfx_volume_label->SetAnchor(settings_panel);
	mute_label = App->gui->CreateLabel(iPoint(185 , 216), "Mute", karma_font_settings, this, color);
	mute_label->SetAnchor(settings_panel);
	limitate_fps_label = App->gui->CreateLabel(iPoint(225 , 255), "Limitate FPS", karma_font_settings, this, color);
	limitate_fps_label->SetAnchor(settings_panel);

	// Sliders ============================================
	Slider_Definition slider_def;
	slider_def.distance = 120;
	slider_def.thumb_definition.idle_rect = { 302 ,111, 13,24 };
	slider_def.thumb_definition.hover_rect = { 315 ,111, 13,24 };
	slider_def.thumb_definition.pushed_rect = { 328 ,111, 13,24 };
	slider_def.rail_draw_rect = { 248 ,136, 136,7 };

	slider_def.default_value = App->audio->volume_music;
	slider_def.max_value = 60;

	slider_music_volume = App->gui->CreateSlider(iPoint(399 , 134), slider_def, this);
	slider_music_volume->SetAnchor(settings_panel);

	slider_def.default_value = App->audio->volume_sfx;
	slider_def.max_value = 60;

	slider_sfx_volume = App->gui->CreateSlider(iPoint(399 , 178), slider_def, this);
	slider_sfx_volume->SetAnchor(settings_panel);

	// Checkboxes ========================================
	Checkbox_Definition checkbox_def;
	checkbox_def.check_off_button.idle_rect = { 248, 144 , 19, 21};
	checkbox_def.check_off_button.hover_rect = { 267, 144 , 19, 21 };
	checkbox_def.check_off_button.pushed_rect = { 286, 144 , 19, 21 };

	checkbox_def.check_on_button.idle_rect = { 305, 144 , 19, 21 };
	checkbox_def.check_on_button.hover_rect = { 324, 144 , 19, 21 };
	checkbox_def.check_on_button.pushed_rect = { 343, 144 , 19, 21 };

	checkbox_mute = App->gui->CreateCheckbox(iPoint(398 , 219), checkbox_def, this);
	checkbox_mute->SetValue(App->audio->mute);
	checkbox_mute->SetAnchor(settings_panel);

	checkbox_limitate_fps = App->gui->CreateCheckbox(iPoint(398 , 257), checkbox_def, this);
	checkbox_limitate_fps->SetAnchor(settings_panel);
	checkbox_limitate_fps->SetValue(App->apply_cap_frames);

	settings_panel->SetPosition(iPoint(320 + 1280, 180));

	// =============================================================
    // ==================   Credits   ==============================
    // =============================================================

	Animation panel_anim2;
	panel_anim2.PushBack({ 387, 0, 389, 293 });
	credits_panel = App->gui->CreateImage(iPoint(320, 182), panel_anim2, this);
	credits_panel->SetAnchor(menu);

	karma_font_credits = App->font->Load("fonts/ProFontWindows.ttf", 12);

	TextPanel_Definition license_text_panel_def;
	license_text_panel_def.line_spacing = 2;
	license_text_panel_def.color = color;
	license_text_panel_def.font = karma_font_credits;
	license_text_panel_def.width = 360;
	license_text_panel_def.text.create(
		
		"MIT License"

		"Copyright(c) 2018 @alejandro61299 (Alejandro Gamarra Niño) and @optus23 (Marc Gálvez Llorens) "

		"Permission is hereby granted, free of charge, to any person obtaining a copy"
		"of this software and associated documentation files(the Software), to deal"
		"in the Software without restriction, including without limitation the rights"
		"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell"
		"copies of the Software, and to permit persons to whom the Software is"
		"furnished to do so, subject to the following conditions :"

	    "The above copyright notice and this permission notice shall be included in all"
		"copies or substantial portions of the Software."

		"THE SOFTWARE IS PROVIDED  AS IS , WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
		"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
		"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE"
		"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
		"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
		"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE"
		"SOFTWARE"  );

	license_text_panel = App->gui->CreateTextPanel(iPoint(320, 180), license_text_panel_def, this);
	license_text_panel->SetAnchor(menu);


	Button_Definition button_def_return_credits({ 778 ,136, 42,45 }, { 778 ,181, 42,45 }, { 778 ,230, 42,45 });
	button_return_credits = App->gui->CreateButton(iPoint(320, 318), button_def_return_credits, this);
	button_return_credits->SetAnchor(credits_panel);

	Button_Definition button_def_github({ 822 ,0, 42,45 }, { 822 ,45, 42,45 }, { 822 ,90, 42,50 });
	button_github_1 = App->gui->CreateButton(iPoint(50, 329), button_def_github, this);
	button_github_1->SetAnchor(credits_panel);

	button_github_2 = App->gui->CreateButton(iPoint(80, 329), button_def_github, this);
	button_github_2->SetAnchor(credits_panel);


	menu->SetPosition(iPoint(-(App->render->camera.x / (int)App->win->GetScale()), 0));


	return true;
}

bool MainMenu::UnloadScene()
{
	BROFILER_CATEGORY("MainMenu Unload", Profiler::Color::Maroon);
	App->font->Unload(karma_font_settings);

	App->gui->DeleteObject(menu);

	// ---------- Menu -------------------------

	App->gui->DeleteObject(logo);

	App->gui->DeleteObject(button_new_game);
	App->gui->DeleteObject(button_continue);
	App->gui->DeleteObject(button_exit);

	App->gui->DeleteObject(button_credits);
	App->gui->DeleteObject(button_settings);
	App->gui->DeleteObject(button_web);
	App->gui->DeleteObject(button_github_1);
	App->gui->DeleteObject(button_github_2);

	// ---------- Settings ----------------------

	App->gui->DeleteObject(settings_label);
	App->gui->DeleteObject(music_volume_label);
	App->gui->DeleteObject(sfx_volume_label);
	App->gui->DeleteObject(limitate_fps_label);
	App->gui->DeleteObject(mute_label);

	App->gui->DeleteObject(slider_music_volume);
	App->gui->DeleteObject(slider_sfx_volume);

	App->gui->DeleteObject(checkbox_mute);
	App->gui->DeleteObject(checkbox_limitate_fps);

	App->gui->DeleteObject(settings_panel);
	App->gui->DeleteObject(credits_panel);
	App->gui->DeleteObject(button_return_settings);
	App->gui->DeleteObject(button_return_credits);


	// --------- Credits -----------------------

	App->gui->DeleteObject(license_text_panel);

	App->tex->UnLoad(paralax_tex_1);
	App->tex->UnLoad(paralax_tex_2);
	App->tex->UnLoad(paralax_tex_3);
	return true;
}

bool MainMenu::RepeatClick(Object * object)
{
	if (object == slider_music_volume)
	{
		App->audio->volume_music = slider_music_volume->GetValue();
	}
	if (object == slider_sfx_volume)
	{
		App->audio->volume_sfx = slider_sfx_volume->GetValue();
	}

	return false;
}


bool MainMenu::OutClick(Object * object)
{
	if (object == button_new_game)
	{
		App->hud->Reset();
		App->scene_manager->ChangeScene("level_1", 1);	
	}
	else if (object == button_settings)
	{
		MoveToSection(MenuSection::settings);
	}
	else if (object == button_return_settings)
	{
		MoveToSection(MenuSection::main_menu);
	}
	else if (object == button_exit)
	{
		App->scene_manager->Exit();
	}
	else if (object == button_web)
	{
		ShellExecuteA(NULL, "open", "https://little-busters-studio.github.io/SlimeFall/", NULL, NULL, SW_SHOWNORMAL);
	}
	else if (object == button_github_1)
	{
		ShellExecuteA(NULL, "open", "https://github.com/optus23", NULL, NULL, SW_SHOWNORMAL);
	}
	else if (object == button_github_2)
	{
		ShellExecuteA(NULL, "open", "https://github.com/alejandro61299", NULL, NULL, SW_SHOWNORMAL);
	}
	else if (object == button_credits)
	{
		App->audio->PlayMusic(credits_music_path.GetString()); 
		MoveToSection(MenuSection::credits);
	}
	else if (object == button_return_credits)
	{
		App->audio->PlayMusic(music_path.GetString());
		MoveToSection(MenuSection::main_menu);
	}
	else if (object == checkbox_mute)
	{
		App->audio->mute = checkbox_mute->GetValue();
	}
	else if (object == checkbox_limitate_fps)
	{
		App->apply_cap_frames = checkbox_limitate_fps->GetValue();
	}
	else if (object == button_continue)
	{
		App->LoadGame();
	}


	return true;
}

bool MainMenu::MoveToSection(MenuSection menu_section)
{
	current_section = menu_section;

	if (App->render->camera.x > move_to_point[(int)menu_section].x )
	{
		camera_velocity.x = - camera_speed;
	}
	else if (App->render->camera.x < move_to_point[(int)menu_section].x)
	{
		camera_velocity.x = camera_speed;
	}
	else 
	{
		camera_velocity.x = 0.0f;
	}

	return false;
}



