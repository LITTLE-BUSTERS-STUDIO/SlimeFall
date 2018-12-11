#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "j1Module.h"
#include "j1Scene.h"

struct SDL_Texture;

class MainMenu : public j1Scene
{
public:

	MainMenu();

	virtual ~MainMenu();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool PostUpdate(float dt);

	bool CleanUp();

private:

	p2SString music_path;

};

#endif // __MAINMENU_H__

