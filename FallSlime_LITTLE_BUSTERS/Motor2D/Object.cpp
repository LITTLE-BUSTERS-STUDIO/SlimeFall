#include "j1App.h"
#include "Entity.h"
#include "EntityManager.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "Brofiler/Brofiler.h"
#include "Object.h"

Object::Object(iPoint position)
{
	this->position = position;
}

Object::~Object()
{
}
