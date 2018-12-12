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

bool Entity::FindCollider( Collider *collider) const
{
	for (p2List_item<Collider*>*item = colliders.start ; item ; item = item->next )
	{
		if (item->data == collider) 
		{
			return true;
		}
	}
	
	return false;
}


