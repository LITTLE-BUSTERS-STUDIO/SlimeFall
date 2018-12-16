#include <iostream> 
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1App.h"
#include "Level_1.h"
#include "j1FadeToBlack.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "j1Gui.h"
#include "j1PathFinding.h"
#include "j1Fonts.h"
#include "Hud.h"
#include "Brofiler/Brofiler.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	collision = new j1Collision();
	audio = new j1Audio();
	font = new j1Fonts();
	map = new j1Map();
	scene_manager = new SceneManager();
	entity_manager = new EntityManager();
	gui = new j1Gui();
	fade_to_black = new j1FadeToBlack();
	path_finding = new j1PathFinding();
	hud = new Hud();

	// Add modules ================================================
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(font);
	AddModule(map);
	AddModule(scene_manager);      
	AddModule(path_finding);       
	AddModule(entity_manager);     
	AddModule(collision);          
	AddModule(gui);
	AddModule(hud);
	AddModule(render);             
}

// Destructor
j1App::~j1App()
{
	// Release modules ==================================
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}
	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	if (module)
	{
		module->Init();
		modules.add(module);
	}
}

// Called before render is available
bool j1App::Awake()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
	
	save_game.create("data/save_game.xml");
	load_game.create("data/save_game.xml");

	pugi::xml_document	save_doc;
	save_doc.load_file(save_game.GetString());
	if (save_doc != NULL)
	{
		save_doc_exist = true;
	}

	save_doc.reset();

	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());
		framerate_cap = app_config.attribute("framerate_cap").as_int(0);
	}

	if(ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	BROFILER_CATEGORY("App Update", Profiler::Color::Gold);

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("data/config.xml");

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{

	frame_count++;
	last_sec_frame_count++;
	
	if (pause_game)
	{
		dt = 0.0f;
	}
	else if (first_frame)
	{
		dt = 1.0f / framerate_cap;
	}
	else
	{
		double framerate = 1000.00 / perfect_frame_time.ReadMs();

		dt = 1.0f / framerate;

		if (dt > 1.0f / (float)framerate_cap + 0.02f)
		{
			dt = 1.0f / (float)framerate_cap + 0.02f;
		}
	}

	perfect_frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if(want_to_save == true)
		SavegameNow();

	if(want_to_load == true)
		LoadGameNow();

	if (first_frame == true)
	{
		first_frame = false;
	}

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = (float)frame_count / start_time.ReadSec();
	float seconds_since_startup = start_time.ReadSec();
	double last_frame_ms = perfect_frame_time.ReadMs();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	if (last_frame_ms > 16.0F)
		LOG("LAG MS : %u", last_frame_ms);


	// Assigment 2 Title ===================================================
	static char title_info[256];


	p2SString FramerateCap;
	if (apply_cap_frames)
		FramerateCap = "ON";
	else
		FramerateCap = "OFF";

	p2SString VsyncCap;

	if (App->render->vsync)
		VsyncCap = "ON";
	else
		VsyncCap = "OFF";

	sprintf_s(
		title_info,
		256,
		"Slime Fall || Framerate Cap: %s || Vsync: %s || FPS: %i || Av.FPS: %.2f || Last Frame Ms: %0.1f", 
		FramerateCap.GetString(), VsyncCap.GetString(), frames_on_last_update, avg_fps, last_frame_ms);


	App->win->SetTitle(title_info);

	if (!App->render->vsync && framerate_cap != 0 && apply_cap_frames)
	{

		uint32 frame_cap_ms = 1000.0F / (float)framerate_cap;

		if (frame_cap_ms > last_frame_ms)
		{
			SDL_Delay((uint32)frame_cap_ms - last_frame_ms);
		}
		else
			SDL_Delay(frame_cap_ms - (uint32)last_frame_ms % (uint32)frame_cap_ms);
	}

}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	// Change frame cap ========================================
	if (App->input->keyboard[SDL_SCANCODE_F11] == KEY_DOWN)
	{
		apply_cap_frames = !apply_cap_frames;
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	
	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void j1App::LoadGame()
{
	want_to_load = true;
}

// ---------------------------------------
void j1App::SaveGame() const
{
	want_to_save = true;
}

bool j1App::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.GetString());

	if(result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.GetString());

		root = data.child("game_state");

		p2List_item<j1Module*>* item = modules.start;
		ret = true;

		while(item != NULL && ret == true)
		{
			ret = item->data->Load(root.child(item->data->name.GetString()));
			item = item->next;
		}

		data.reset();
		if(ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (item != NULL) ? item->data->name.GetString() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.GetString(), result.description());

	save_doc_exist = true;
	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.GetString());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	p2List_item<j1Module*>* item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Save(root.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if(ret == true)
	{
		data.save_file(save_game.GetString());
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", (item != NULL) ? item->data->name.GetString() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}

float j1App::GetDeltaTime() const
{
	return dt;
}