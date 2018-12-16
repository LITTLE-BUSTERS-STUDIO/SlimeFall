#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Collision;
class j1Audio;
class j1Scene;
class j1Map;
class j1Player;
class j1FadeToBlack;
class EntityManager;
class SceneManager;
class j1PathFinding;
class j1Gui;
class j1Fonts;
class Hud;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;

	const char* GetArgv(int index) const;

	const char* GetTitle() const;

	const char* GetOrganization() const;

	void LoadGame();

	void SaveGame() const;

	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

	float GetDeltaTime() const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();

	bool SavegameNow() const;



public:
	// Modules =================================
	j1Window*			win = nullptr;
	j1Input*			input = nullptr;
	j1Render*			render = nullptr;
	j1Textures*			tex = nullptr;
	j1Audio*			audio = nullptr;
	j1Fonts*            font = nullptr;
	j1Map*				map = nullptr;
	j1Gui*              gui = nullptr;
	j1Collision*        collision = nullptr;
	j1FadeToBlack*		fade_to_black = nullptr;
	j1PathFinding*      path_finding = nullptr;
	EntityManager *     entity_manager = nullptr;
	SceneManager *      scene_manager = nullptr;
	Hud *				hud = nullptr;

	mutable bool        save_doc_exist = false;

	// Framerate ==============================
	bool			    apply_cap_frames = true;
	j1Timer				timer;
	bool				pause_game = false;
	bool                first_frame = true;

private:

	p2List<j1Module*>	modules;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	p2SString			load_game;
	mutable p2SString	save_game;

	// Framerate =======================================                
	uint				frames = 0;
	uint64				frame_count = 0;
	int					framerate_cap = 0;
	float               dt = 0.00F;
	uint				last_sec_frame_count = 0;
	uint				prev_last_sec_frame_count = 0;

	j1Timer				last_sec_frame_time;
	j1PerfTimer         perfect_frame_time;
	j1Timer				frame_time;
	j1Timer				start_time;
};

extern j1App* App;

#endif