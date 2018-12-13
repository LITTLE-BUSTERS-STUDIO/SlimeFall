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

Hud::Hud() :j1Module()
{
}

bool Hud::Awake(pugi::xml_node &node)
{
	return true;
}

bool Hud::Start()
{
	SDL_Rect lives_rect = { 0, 96, 17, 14 };
	lives = App->gui->CreateImage(iPoint(30, 20), lives_rect, this);
	lives = App->gui->CreateImage(iPoint(55, 20), lives_rect, this);
	lives = App->gui->CreateImage(iPoint(80, 20), lives_rect, this);
	lives = App->gui->CreateImage(iPoint(105, 20), lives_rect, this);

	SDL_Rect score_rect = { 19, 96, 12, 18 };
	score = App->gui->CreateImage(iPoint(550, 20), score_rect, this);
	score = App->gui->CreateImage(iPoint(575, 20), score_rect, this);
	score = App->gui->CreateImage(iPoint(600, 20), score_rect, this);
	
	SDL_Rect score_locked = { 33, 96, 12, 18 };
	score = App->gui->CreateImage(iPoint(550, 20), score_locked, this);
	score = App->gui->CreateImage(iPoint(575, 20), score_locked, this);
	score = App->gui->CreateImage(iPoint(600, 20), score_locked, this);

	SDL_Rect wood_panel_rect = { 0, 114, 235, 54 };
	wood_panel = App->gui->CreateImage(iPoint(320, 30), wood_panel_rect, this);
	wood_panel->IsDraggable(false);
	SDL_ShowCursor(SDL_DISABLE);

	return true;
}

bool Hud::PreUpdate()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	App->render->Blit(App->);
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
