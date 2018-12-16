#include "j1App.h"
#include "Entity.h"
#include "EntityManager.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "Brofiler/Brofiler.h"

Entity::Entity(fPoint position, fPoint spawn_pos, Properties *properties) : position( position) , spawn_pos(spawn_pos), properties(properties)
{

}

Entity::~Entity()
{

}

fPoint Entity::GetPosition()
{
	return position;
}

void Entity::SetPosition( fPoint position)
{
	this->position = position;
}

Collider* Entity::GetMainCollider() 
{
	return main_collider;
}


