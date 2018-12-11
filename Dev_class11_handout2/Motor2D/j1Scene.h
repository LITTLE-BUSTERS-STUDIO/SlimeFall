#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Gui.h"

struct SDL_Texture;
class Image;
class Object;
class Label;
class Button_Input;
class Gui_Listener;
class Button_Animation;

class j1Scene : public j1Module , public Gui_Listener
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

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

	bool OnHover(Object* object);

	bool OutHover(Object* object);

private:
	SDL_Texture* debug_tex;
	Image* banner = nullptr;
	Label* label = nullptr;
	Button_Input* button_1 = nullptr;
	//Test
	Animation animation_button;


};

#endif // __j1SCENE_H__