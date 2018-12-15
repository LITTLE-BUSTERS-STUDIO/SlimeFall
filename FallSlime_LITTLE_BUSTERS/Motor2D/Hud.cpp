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
#include "j1Window.h"

Hud::Hud() :j1Module()
{
	name.create("hud");
}

bool Hud::Awake(pugi::xml_node &node)
{
	lives_counter = MAX_LIVES; //MAGIC NUMBER to xml 
	coin_counter = 0; //MAGIC NUMBER to xml 
	return true;
}

bool Hud::Start()
{
	hud_object = App->gui->CreateObject(iPoint(App->render->camera.w * 0.5F, App->render->camera.h * 0.5F), this);
	
	Animation anim_lives;
	anim_lives.PushBack({ 0, 96, 17, 14 });
	anim_lives.PushBack({ 50, 165, 29, 32 });

	for (uint i = 0 ; i < MAX_LIVES; ++i)
	{
		Image* live = App->gui->CreateImage(iPoint(25 * i + 40, 30), anim_lives, this);
		lives_list.add(live);
		live->SetAnchor(hud_object);
	}

	Animation anim_score;
	anim_score.PushBack({ 33, 96, 12, 18 });
	anim_score.PushBack({ 19, 96, 12, 18 });

	for (uint i = 0; i < MAX_COINS; ++i)
	{
		Image* coin = App->gui->CreateImage(iPoint(25 * i + 550, 30), anim_score, this);
		coins_list.add(coin);
		coin->SetAnchor(hud_object);
	}



	//Animation anim_panel;
	//anim_panel.PushBack({ 0, 446, 800, 61 });
	//panel_pos = { -320, 170 }; //Camera w && Camera h / GetScale()
	//panel_stages = App->gui->CreateImage(iPoint(panel_pos), anim_panel, this);

	//panel_stages->SetAnchor(hud_object);

	App->gui->SetStateToBranch(ObjectState::hidden, hud_object);

	return true;
}

bool Hud::PreUpdate()
{	
	return true;
}

bool Hud::Update(float dt)
{
	/*if (panel_pos.x <= 320)
		panel_pos.x += 20;
	panel_stages->SetPosition(panel_pos);*/
	/*
	if (App->pause_game)
	{
		App->render->DrawQuad({ App->render->camera.x, App->render->camera.y, App->render->camera.w, App->render->camera.h }, 0, 0, 0, 200, false);
	}*/
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


 int Hud::SubstractLife()
{
	 --lives_counter;
	 if (lives_counter < 0)
	 {
		 lives_counter = 0;
	 }

	 for (uint i= 0 ; i < MAX_LIVES ; ++i)
	 {
		 if (i >= lives_counter)
		 {
			 lives_list[i]->SetAnimationFrame(1);
		 }
		 else
		 {
			 lives_list[i]->SetAnimationFrame(0);
		 }
	 }

	return true;
}

int Hud::AddCoin()
{
	++coin_counter;
	if (coin_counter > 3)
	{
		coin_counter = 3;
	}

	for (uint i = 0; i < MAX_COINS; ++i)
	{
		if (i >= coin_counter)
		{
			coins_list[i]->SetAnimationFrame(0);
		}
		else
		{
			coins_list[i]->SetAnimationFrame(1);
		}
	}
	return true;
}

