#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

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
public:
	SDL_Texture * background_parallax = nullptr;

	

	SDL_Rect parallax_1;
	SDL_Rect parallax_2;
	SDL_Rect parallax_3;
	SDL_Rect parallax_4;
	SDL_Rect parallax_5;
	SDL_Rect parallax_6;
	SDL_Rect parallax_7;
	SDL_Rect parallax_8;
	SDL_Rect parallax_9;
	SDL_Rect parallax_10;
	SDL_Rect parallax_11;


};

#endif // __j1SCENE_H__