#include "j1App.h"
#include "j1Render.h"
#include "Object.h"
#include "j1Gui.h"
#include "p2Log.h"

Object::Object(const iPoint position, Gui_Listener *listener)
{
	this->position = position;
	this->listener = listener;

	section.x = section.y = 0;
	section.w = DEFAULT_MARGIN_SECTION;
	section.h = DEFAULT_MARGIN_SECTION;
}

Object::~Object()
{
}

iPoint Object::GetPosition() const
{
	return position;
}

void Object::SetPosition(const iPoint position) 
{
	this->position = position;
	UpdateRelativePosition();
}

bool Object::UpdateRelativePosition()
{
	if (anchor_parent == nullptr)
	{
		return false;
	}

	relative_position = position - anchor_parent->GetPosition();
	return true;
}

bool Object::SetAnchor(Object * anchor)
{
	if (anchor == nullptr)
	{
		LOG("Failed SetAnchor, anchor was nullptr");
		return false;
	}
	// Delete previous parent =====================
	if (anchor_parent)
	{
		p2List<Object*> *sons = anchor_parent->GetAnchorSons();
		int object_index = sons->find(this);

		if (object_index != -1)
		{
			sons->del(sons->At(object_index));
			anchor_parent = nullptr;
		}
		else
		{
			LOG("Failed SetAnchor, object as son not found");
			return false;
		}
	}

	// Set Parent =================================
	anchor_parent = anchor;

	relative_position = position - anchor_parent->position;

	// Add to parent sons =========================
	anchor_parent->GetAnchorSons()->add(this);

	return true;
}

p2List<Object*>* Object::GetAnchorSons() 
{
	return &anchor_sons;
}

Object * Object::GetAnchorParent()
{
	return anchor_parent;
}

void Object::IsDraggable(const bool is_draggable)
{
	this->is_draggable = is_draggable;
}

void Object::SetState(const ObjectState state)
{
	this->state = state;
}


