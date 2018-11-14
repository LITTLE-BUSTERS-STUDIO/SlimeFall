#ifndef __j1FADETOBLACK_H__
#define __j1FADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack() {};
	~j1FadeToBlack();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PostUpdate(float dt);
	bool FadeToBlack(float time = 1.0f);
	bool IsFading() const;

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;

	j1Module* fade_out = nullptr;
	j1Module* fade_in = nullptr;

};

#endif //__MODULEFADETOBLACK_H__