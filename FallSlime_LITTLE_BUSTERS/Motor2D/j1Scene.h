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
	uint       id;
	p2SString  map_path;
	int        phase_width;
	int        phase_hight;
};

class j1Scene
{
public:

	j1Scene() {}

	virtual ~j1Scene() {}

	virtual bool PreUpdate() { return true; };

	virtual bool Update(float dt) { return true; }

	virtual bool PostUpdate() { return true; }

	//Scene Load & Unload ===============

	virtual bool LoadScene(pugi::xml_node& node) { return true; }

	virtual bool UnloadScene() { return true; }

protected:

	p2SString          name;
	p2List<Phase *>    phases;
	uint               default_phase;

private:

	friend SceneManager;
};

#endif // __j1SCENE_H__