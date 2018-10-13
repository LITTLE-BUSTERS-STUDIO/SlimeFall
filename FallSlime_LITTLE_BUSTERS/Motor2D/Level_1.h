#ifndef __LEVEL_1_H__
#define __LEVEL_1_H__

#include "j1Module.h"
#include "j1Scene.h"

struct SDL_Texture;

class Level_1 : public j1Scene
{
public:

	Level_1();

	// Destructor
	virtual ~Level_1();

	// Called before render is available
	 bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	 bool PreUpdate();

	// Called each loop iteration
	 bool Update(float dt);

	// Called before all Updates
	 bool PostUpdate();

	// Called before quitting
	 bool CleanUp();

private:
	p2SString music_path;
	p2SString background_path;
	SDL_Texture * background_parallax = nullptr;
	Parallax parallax[11];
};

#endif // __LEVEL_1_H__
