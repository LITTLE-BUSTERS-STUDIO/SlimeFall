#include "j1App.h"
#include "Entity.h"
#include "EntityManager.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "Brofiler/Brofiler.h"

Entity::Entity(fPoint position, Entity_Info info) : position( position)
{
	this->position = position;
}

Entity::~Entity()
{

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

