#ifndef __LEVEL_1_H__
#define __LEVEL_1_H__

#include "j1Module.h"
#include "j1Scene.h"

struct SDL_Texture;

class Level_1 : public j1Scene
{
public:

	Level_1();

	virtual ~Level_1();

	 bool Awake(pugi::xml_node&);

	bool Start();

	 bool Update(float dt);

	 bool PostUpdate(float dt);

	 bool CleanUp();

private:
	p2SString music_path;
	p2SString background_path1;
	p2SString background_path2;
	p2SString background_path3;
	p2SString background_path;
	SDL_Texture * background_parallax1 = nullptr;
	SDL_Texture * background_parallax2 = nullptr;
	SDL_Texture * background_parallax3 = nullptr;
	SDL_Texture * background_parallax = nullptr;
	Parallax parallax1[4];
	Parallax parallax2[4];
	Parallax parallax3[3];
	Parallax parallax[11];

public:
	uint		phase1_width;
	uint		phase1_high;
	uint		phase2_width;
	uint		phase2_high;
	uint		background_width;
	uint		background_high;
	uint		max_background_layers;
	uint		background_startpos;
	float		parallax_speed_1;
	float		parallax_speed_2;
	float		parallax_speed_3;


};

#endif // __LEVEL_1_H__
