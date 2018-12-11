#ifndef __Object_H__
#define __Object_H__

#include "p2Point.h"
#include "Animation.h"
#include "p2List.h"
#include "j1Module.h"

struct SDL_Texture;
class Gui_Listener;

enum class HoverState
{
	On,
	Out,
	Repeat,
	None
};

class Object
{

public:
	Object(iPoint position , Gui_Listener *listener);
	
	virtual ~Object();

	// Virtual methods ================================
	virtual bool Draw() { return true; };

	// Common methods =================================
	iPoint GetPosition() const;

	void SetPosition(const iPoint position);

	void SetRelativePosition(const iPoint position);

	bool SetAnchor(Object* anchor);

	p2List<Object*>* GetAnchorSons(); 

	Object* GetAnchorParent();

	void IsDraggable(const bool is_draggable);

	// Variables ======================================
	HoverState hover_state = HoverState::None;

protected:

	iPoint                position;
	iPoint                relative_position;
	Animation			  animation;
	SDL_Rect			  section;
	SDL_Texture         * texture = nullptr;
	Gui_Listener        * listener = nullptr;

	Object              * anchor_parent = nullptr;
	p2List<Object*>       anchor_sons;

	// Properties --------------------------------------
	bool is_draggable = false;

	friend class j1Gui;
};

#endif // __Object_H__