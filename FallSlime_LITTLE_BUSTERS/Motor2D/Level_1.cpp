#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "Level_1.h"
#include "EntityManager.h"
#include "Brofiler/Brofiler.h"
#include "j1Gui.h"
#include "Hud.h"

#include "Button_Input.h"
#include "Label.h"
#include "j1Fonts.h"



Level_1::Level_1() : j1Scene()
{
	name.create("level_1");
}


bool Level_1::LoadScene(pugi::xml_node& node)
{
	BROFILER_CATEGORY("Level_1 Load", Profiler::Color::Maroon);

	LOG("Loading Level 1");

	App->pause_game = false;

	music_path = node.child("music").attribute("path").as_string("");
	game_over_path = node.child("game_over").attribute("path").as_string("");
	App->audio->PlayMusic(music_path.GetString());

	paralax_tex_1 = App->tex->Load(node.child("background1").attribute("path").as_string(""));
	paralax_tex_2 = App->tex->Load(node.child("background2").attribute("path").as_string(""));
	paralax_tex_3 = App->tex->Load(node.child("background3").attribute("path").as_string(""));

	background_width = node.child("background_dimension").attribute("width").as_uint(0u);
	background_high = node.child("background_dimension").attribute("high").as_uint(0u);
	max_background_layers = node.child("max_background_layers").attribute("value").as_uint(0u);
	background_startpos = node.child("background_startpos").attribute("value").as_uint(0u);
	

	for (uint i = 0; i < 4; i++)
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

	parallax_speed_11 = node.child("parallax_speed").attribute("low").as_float(0.0F);
	parallax_speed_22 = node.child("parallax_speed").attribute("medium").as_float(0.0F);
	parallax_speed_33 = node.child("parallax_speed").attribute("high").as_float(0.0F);

	// Anchor =============================================

	game_over_anchor = App->gui->CreateObject(iPoint(1280 * 0.5F, 720 * 0.5F), this);
	paused_menu = App->gui->CreateObject(iPoint(1280 * 0.5F, 720 * 0.5F), this);
	
	//Labels ==============================================
	karma_font_buttons = App->font->Load("fonts/KarmaSuture.ttf", 24);
	SDL_Color color = { 231,94,152,255 };

	// Buttons ============================================
	Button_Definition button_rectangle({ 219 , 0, 122, 36 }, { 219, 36, 122, 36 }, { 219, 72, 122, 36 });

	button_resume = App->gui->CreateButton(iPoint(321, 89), button_rectangle, this);
	button_resume->SetLabel(iPoint(321, 85), p2SString("RESUME"), karma_font_buttons, color);
	button_resume->SetAnchor(paused_menu);

	button_save = App->gui->CreateButton(iPoint(321, 146), button_rectangle, this);
	button_save->SetLabel(iPoint(321, 142), p2SString("SAVE"), karma_font_buttons, color);
	button_save->SetAnchor(paused_menu);

	button_load = App->gui->CreateButton(iPoint(321, 199), button_rectangle, this);
	button_load->SetLabel(iPoint(321, 195), p2SString("LOAD"), karma_font_buttons, color);
	button_load->SetAnchor(paused_menu);

	if (!App->save_doc_exist)
	{
		button_load->SetState(ObjectState::locked);
	}

	button_exit_to_menu = App->gui->CreateButton(iPoint(321, 278), button_rectangle, this);
	button_exit_to_menu->SetLabel(iPoint(321, 274), p2SString("EXIT"), karma_font_buttons, color);
	button_exit_to_menu->SetAnchor(paused_menu);

	App->gui->SetStateToBranch(ObjectState::hidden, paused_menu);

	Button_Definition button_def_return_gameover({ 778 ,0, 42,45 }, { 778 ,45, 42,45 }, { 778 ,90, 42,45 });
	button_return_gameover = App->gui->CreateButton(iPoint(69, 268), button_def_return_gameover, this);
	button_return_gameover->SetAnchor(game_over_anchor);

	App->gui->SetStateToBranch(ObjectState::hidden, game_over_anchor);


	App->gui->show_cursor = false;
	App->hud->ShowHud();


	return true;
}


bool Level_1::Update(float dt)
{
	BROFILER_CATEGORY("Level_1 Update", Profiler::Color::MediumBlue);


	return true;
}

bool Level_1::PostUpdate()
{
	BROFILER_CATEGORY("Level_1 PostUpdate", Profiler::Color::MediumOrchid);

	bool ret = true;

	// Blit background--------------------------------------
	for (uint i = 0; i < max_background_layers; i++)
		App->render->Blit(paralax_tex_1, 0, background_startpos, &parallax1[i].rect_parallax, false, parallax_speed_11 /** dt*/);


	for (uint i = 0; i < max_background_layers; i++)
	{
		if (i == 0)
			App->render->Blit(paralax_tex_2, 0, background_startpos, &parallax2[i].rect_parallax, false, parallax_speed_33 /** dt*/);
		else if (i > 0)
			App->render->Blit(paralax_tex_2, 0, background_startpos, &parallax2[i].rect_parallax, false, parallax_speed_22/** dt*/);
	}

	for (uint i = 0; i < max_background_layers; i++)
		App->render->Blit(paralax_tex_3, 0, background_startpos, &parallax3[i].rect_parallax, false, parallax_speed_22 /** dt*/);

	App->map->Draw();
	
	// Blit Game Over + Music

	SDL_Rect game_over_rect = { 0, 0, 640, 360 };

	if (App->hud->Getlife() <= 0)
	{
		App->gui->show_cursor = true;
		App->render->Blit(App->gui->game_over, 0, 0, &game_over_rect, false, 0.0f);
	}



	return ret;
}

// Called before quitting
bool Level_1::UnloadScene()
{
	BROFILER_CATEGORY("Level_1 Unload", Profiler::Color::MediumPurple);

	LOG("Freeing level_1");
	App->tex->UnLoad(paralax_tex_1);
	App->tex->UnLoad(paralax_tex_2);
	App->tex->UnLoad(paralax_tex_3);

	App->gui->DeleteObject(button_resume);
	App->gui->DeleteObject(button_save);
	App->gui->DeleteObject(button_load);
	App->gui->DeleteObject(button_exit_to_menu);
	App->gui->DeleteObject(paused_menu);

	App->gui->DeleteObject(button_return_gameover);
	App->gui->DeleteObject(game_over_anchor);
	
	return true;
}

bool Level_1::OnClick(Object * object)
{
	
	return false;
}

bool Level_1::OutClick(Object * object)
{
	if (object == button_resume)
	{
		ResumeScene();
	}
	else if (object == button_exit_to_menu || object == button_return_gameover)
	{
		App->scene_manager->ChangeScene("main_menu", 1);
	}
	else if (object == button_save)
	{
		ResumeScene();
		App->SaveGame();
	}
	else if (object == button_load)
	{
		App->LoadGame();
	}
	
	return true;
}

bool Level_1::PauseScene()
{
	App->pause_game = true;
	App->gui->SetStateToBranch(ObjectState::visible, paused_menu);
	App->gui->show_cursor = true;
	return true;
}

bool Level_1::ResumeScene()
{
	App->pause_game = false;
	App->gui->SetStateToBranch(ObjectState::hidden, paused_menu);
	App->gui->show_cursor = false;
	return true;
}

bool Level_1::SetGameOver()
{
	App->entity_manager->UnloadEntities();
	App->hud->HideHud();
	App->gui->SetStateToBranch(ObjectState::visible, game_over_anchor);
	App->gui->SetStateToBranch(ObjectState::hidden, paused_menu);

	App->audio->PlayMusic(game_over_path.GetString());
	App->pause_game = true;

	return true;
}

bool Level_1::OutGameOver()
{
	App->gui->SetStateToBranch(ObjectState::hidden, game_over_anchor);
	App->pause_game = false;

	return true;
}
