#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Gui.h"
#include "Hud.h"
#include "Brofiler/Brofiler.h"
#include "j1Input.h"
#include "j1Render.h"
#include "EntityManager.h"
#include "j1Textures.h"
#include "Image.h"

Hud::Hud() :j1Module()
{
}

bool Hud::Awake(pugi::xml_node &node)
{
	return true;
}

bool Hud::Start()
{

	Animation anim_deaths;
	anim_deaths.PushBack({ 0, 165, 29, 32 });
	lives = App->gui->CreateImage(iPoint(38, 32), anim_deaths, this);
	lives = App->gui->CreateImage(iPoint(63, 32), anim_deaths, this);
	lives = App->gui->CreateImage(iPoint(88, 32), anim_deaths, this);
	lives = App->gui->CreateImage(iPoint(113, 32), anim_deaths, this);
	lives->IsDraggable(false);

	Animation anim_lives;
	anim_lives.PushBack({ 0, 96, 17, 14 });
	lives = App->gui->CreateImage(iPoint(40, 30), anim_lives, this);
	lives = App->gui->CreateImage(iPoint(65, 30), anim_lives, this);
	lives = App->gui->CreateImage(iPoint(90, 30), anim_lives, this);
	lives = App->gui->CreateImage(iPoint(115, 30), anim_lives, this);
	lives->IsDraggable(false);


	Animation anim_score;
	anim_score.PushBack({ 19, 96, 12, 18 });
	score = App->gui->CreateImage(iPoint(550, 30), anim_score, this);
	score = App->gui->CreateImage(iPoint(575, 30), anim_score, this);
	score = App->gui->CreateImage(iPoint(600, 30), anim_score, this);
	score->IsDraggable(false);

	Animation anim_score_locked;
	anim_score_locked.PushBack({ 33, 96, 12, 18 });
	//score = App->gui->CreateImage(iPoint(550, 30), anim_score_locked, this);
	score = App->gui->CreateImage(iPoint(575, 30), anim_score_locked, this);
	score = App->gui->CreateImage(iPoint(600, 30), anim_score_locked, this);
	score->IsDraggable(false);

	Animation anim_wood_panel;
	anim_wood_panel.PushBack({ 0, 114, 235, 40 });
	wood_panel = App->gui->CreateImage(iPoint(320, 30), anim_wood_panel, this);
	wood_panel->IsDraggable(false);
	/*SDL_ShowCursor(SDL_DISABLE);*/

	return true;
}

bool Hud::PreUpdate()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	//App->render->Blit(App->);
	return true;
}

bool Hud::Update(float dt)
{
	return true;
}

bool Hud::PostUpdate()
{
	return true;
}

bool Hud::CleanUp()
{
	return true;
}

bool Hud::Load(pugi::xml_node &)
{
	return true;
}

bool Hud::Save(pugi::xml_node &) const
{
	return true;
}
