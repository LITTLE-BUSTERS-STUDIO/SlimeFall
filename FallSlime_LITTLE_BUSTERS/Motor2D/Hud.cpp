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
#include "j1Scene.h"

Hud::Hud() :j1Module()
{
	name.create("hud");
}

bool Hud::Awake(pugi::xml_node &node)
{
	lifes_counter = MAX_LIFES; //MAGIC NUMBER to xml 
	coin_counter = 0; //MAGIC NUMBER to xml 
	return true;
}

bool Hud::Start()
{
	hud_object = App->gui->CreateObject(iPoint(App->render->camera.w * 0.5F, App->render->camera.h * 0.5F), this);
	
	Animation anim_lives;
	anim_lives.PushBack({ 0, 96, 17, 14 });
	anim_lives.PushBack({ 50, 165, 29, 32 });

	for (uint i = 0 ; i < MAX_LIFES; ++i)
	{
		Image* live = App->gui->CreateImage(iPoint(25 * i + 40, 30), anim_lives, this);
		lifes_list.add(live);
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

	App->gui->SetStateToBranch(ObjectState::hidden, hud_object);

	return true;
}

bool Hud::PreUpdate()
{	
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
	App->gui->DeleteObject(hud_object);
	App->gui->DeleteObject(panel_stages);

	for (p2List_item<Image*>* item = lifes_list.start; item; item = item->next)
	{
		if (item->data)
		{
			App->gui->DeleteObject(item->data);
		}
	}
	lifes_list.clear();


	for (p2List_item<Image*>* item = coins_list.start; item; item = item->next)
	{
		if (item->data)
		{
			App->gui->DeleteObject(item->data);
		}
	}
	coins_list.clear();
	return true;
}

bool Hud::Reset()
{
	BROFILER_CATEGORY("Hud Reset", Profiler::Color::LightGray);
	lifes_counter = 5;
	coin_counter = 0;

	for (uint i = 0; i < MAX_COINS; ++i)
	{
		coins_list[i]->SetAnimationFrame(0);
	}

	for (uint i = 0; i < MAX_LIFES; ++i)
	{
		lifes_list[i]->SetAnimationFrame(0);
	}

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

int Hud::Getlife()
{
	return lifes_counter;
}


 int Hud::SubstractLife()
{
	 --lifes_counter;
	 if (lifes_counter <= 0)
	 {
		 lifes_counter = 0;
		 App->scene_manager->GetCurrentScene()->SetGameOver();
	 }

	 for (uint i= 0 ; i < MAX_LIFES; ++i)
	 {
		 if (i >= lifes_counter)
		 {
			 lifes_list[i]->SetAnimationFrame(1);
		 }
		 else
		 {
			 lifes_list[i]->SetAnimationFrame(0);
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

