#ifndef __Hud_H__
#define __Hud_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "Entity.h"
#include "j1Timer.h"
#include "j1Gui.h"


struct SDL_Texture;



class Hud : public Gui_Listener
{
public:

	Hud();

	virtual ~Hud();

	
	bool Update(float dt);

	bool Draw();


public:
	bool			    reset = false;
private:

	//--------------GUI-----------------------
	Image *				lives = nullptr;
	Image *				score = nullptr;
	Image *				wood_panel = nullptr;
	uint				hp;
};


#endif // !__j1Player_H__

