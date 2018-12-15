#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "p2SString.h"
#include "p2List.h"
#include "SDL/include/SDL.h"
#include "SceneManager.h"

struct SDL_Texture;

struct Parallax {
	SDL_Rect rect_parallax = {0,0,0,0};
};

struct Phase {
	uint       id = 0;
	p2SString  map_path;
	int        x_limit = 0;
	int        y_limit = 0;
	bool       camera_follow_player = true;
};

class j1Scene
{
public:

	j1Scene() {}

	virtual ~j1Scene() {}

	virtual bool PreUpdate() { return true; };

	virtual bool Update(float dt) { return true; }

	virtual bool PostUpdate() { return true; }

	virtual bool PauseScene() { return true; }

	virtual bool ResumeScene() { return true; }

	//Scene Load & Unload ===============

	virtual bool LoadScene(pugi::xml_node& node) { return true; }

	virtual bool UnloadScene() { return true; }

public:
	bool               is_pausable = true;

protected:

	p2SString          name;
	p2List<Phase *>    phases;
	uint               default_phase;

private:
	friend SceneManager;
};

#endif // __j1SCENE_H__