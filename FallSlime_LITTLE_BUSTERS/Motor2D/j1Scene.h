#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2List.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

struct Parallax {

	SDL_Rect rect_parallax = {0,0,0,0};
};

struct Phase {
	uint id;
	p2SString map_path;
};

class j1Scene : public j1Module
{
public:

	j1Scene() {}

	// Destructor
	virtual ~j1Scene() {}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&) { return true; }

	// Called before the first frame
	virtual bool Start() {return true; }

	// Called before all Updates
	virtual bool PreUpdate();

	// Called each loop iteration
	virtual bool Update(float dt) { return true; }

	// Called before all Updates
	virtual bool PostUpdate(float dt) { return true; }

	// Called before quitting
	virtual bool CleanUp() { return true; }

	// Load & Save Game

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Load/ Unload phases and its map 
	bool LoadPhase(uint phase_number, bool spawn = true);
	bool UnloadPhase(uint phase_number);
	bool NextPhase();

protected:
	p2List<Phase *> phases;
	uint current_phase;
};

#endif // __j1SCENE_H__