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
#include "j1Render.h"

Hud::Hud() :j1Module()
{
	name.create("hud");
}

bool Hud::Awake(pugi::xml_node &node)
{
	hp_counter = 5; //MAGIC NUMBER to xml 
	return true;
}

bool Hud::Start()
{
	Animation hud_anim;
	hud_anim.PushBack( { 0, 0, 1, 1 } );
	hud_object = App->gui->CreateImage(iPoint(App->render->camera.w * 0.5F, App->render->camera.h * 0.5F), hud_anim, this);
	

	Animation anim_lives;
	anim_lives.PushBack({ 0, 165, 29, 32 });
	anim_lives.PushBack({ 0, 96, 17, 14 });
	lives_1 = App->gui->CreateImage(iPoint(40, 30), anim_lives, this);
	lives_2 = App->gui->CreateImage(iPoint(65, 30), anim_lives, this);
	lives_3 = App->gui->CreateImage(iPoint(90, 30), anim_lives, this);
	lives_4 = App->gui->CreateImage(iPoint(115, 30), anim_lives, this);

	lives_1->SetAnchor(hud_object);
	lives_2->SetAnchor(hud_object);
	lives_3->SetAnchor(hud_object);
	lives_4->SetAnchor(hud_object);

	
	Animation anim_score;
	anim_score.PushBack({ 33, 96, 12, 18 });
	anim_score.PushBack({ 19, 96, 12, 18 });
	score_1 = App->gui->CreateImage(iPoint(550, 30), anim_score, this);
	score_2= App->gui->CreateImage(iPoint(575, 30), anim_score, this);
	score_3 = App->gui->CreateImage(iPoint(600, 30), anim_score, this);

	score_1->SetAnchor(hud_object);
	score_2->SetAnchor(hud_object);
	score_3->SetAnchor(hud_object);

	Animation anim_wood_panel;
	anim_wood_panel.PushBack({ 0, 114, 235, 40 });
	wood_panel = App->gui->CreateImage(iPoint(320, 30), anim_wood_panel, this);

	wood_panel->SetAnchor(hud_object);
	
	App->gui->SetStateToBranch(ObjectState::hidden, hud_object);
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

bool Hud::HideHud()
{
	if (hud_object == nullptr)
	{
		return false;
	}
	App->gui->SetStateToBranch(ObjectState::hidden, hud_object);
	return true;
}

bool Hud::ShowHud()
{
	if (hud_object == nullptr)
	{
		return false;
	}
	App->gui->SetStateToBranch(ObjectState::visible, hud_object);
	return true;
}


uint Hud::SubstractLife()
{
	return hp_counter--;
}
