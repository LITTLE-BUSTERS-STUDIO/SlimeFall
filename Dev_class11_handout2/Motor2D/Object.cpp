#include "j1App.h"
#include "j1Render.h"
#include "Object.h"
#include "j1Gui.h"
#include "p2Log.h"

Object::Object(iPoint position, Gui_Listener *listener)
{
	this->position = position;
	this->listener = listener;
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
}

void Object::SetRelativePosition(const iPoint relative_position)
{
	this->relative_position = relative_position;
}

bool Object::SetAnchor(Object * anchor, bool transform_to_relative)
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
