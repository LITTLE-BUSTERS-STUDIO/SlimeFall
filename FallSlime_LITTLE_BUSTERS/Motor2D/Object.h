#ifndef __Object_H__
#define __Object_H__

#include "p2Point.h"
#include "Animation.h"
#include "p2List.h"

struct SDL_Texture;


class Object
{
public:
	//------------Valeues-------------------
	

	//-----------Animations-----------------


	//-----------Textures-------------------


	//------------Sfx----------------------

public:
	Object(iPoint position, SDL_Rect section);
	
	virtual ~Object();

	p2SString             name;
	iPoint                position;
	Animation			  anim;
	SDL_Rect			  section;
	SDL_Texture*		  tex;


	// Virtual methods ================================
	virtual bool Draw() { return true; };



};

#endif // __Entity_H__
