#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Gui.h"
#include "Hud.h"
#include "Brofiler/Brofiler.h"

Hud::Hud() :j1Module()
{
}

bool Hud::Awake(pugi::xml_node &)
{

	return true;
}

bool Hud::Start()
{
	Animation anim_lives;
	anim_lives.PushBack({ 0, 96, 17, 14 });
	lives = App->gui->CreateImage(iPoint(30, 20), anim_lives, this);
	lives = App->gui->CreateImage(iPoint(55, 20), anim_lives, this);
	lives = App->gui->CreateImage(iPoint(80, 20), anim_lives, this);
	lives = App->gui->CreateImage(iPoint(105, 20), anim_lives, this);
	lives->IsDraggable(false);

	Animation anim_score;
	anim_score.PushBack({ 19, 96, 12, 18 });
	score = App->gui->CreateImage(iPoint(550, 20), anim_score, this);
	score = App->gui->CreateImage(iPoint(575, 20), anim_score, this);
	score = App->gui->CreateImage(iPoint(600, 20), anim_score, this);
	score->IsDraggable(false);

	Animation anim_score_locked;
	anim_score_locked.PushBack({ 33, 96, 12, 18 });
	score = App->gui->CreateImage(iPoint(550, 20), anim_score_locked, this);
	score = App->gui->CreateImage(iPoint(575, 20), anim_score_locked, this);
	score = App->gui->CreateImage(iPoint(600, 20), anim_score_locked, this);
	score->IsDraggable(false);

	Animation anim_wood_panel;
	anim_wood_panel.PushBack({ 0, 114, 235, 54 });
	wood_panel = App->gui->CreateImage(iPoint(320, 30), anim_wood_panel, this);
	wood_panel->IsDraggable(false);
	return true;
}

bool Hud::PreUpdate()
{

	return true;
}

bool Hud::Update(float dt)
{
	App->gui->Disable();
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
