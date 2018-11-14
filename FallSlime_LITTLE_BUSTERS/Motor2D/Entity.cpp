#include "j1App.h"
#include "Entity.h"
#include "EntityManager.h"
#include "j1Render.h"
#include "j1Collision.h"

Entity::Entity(iPoint position, Entity_Info info) : position( position), collider(nullptr)
{
	collider = App->collision->AddCollider(info.properties.collider_rect, COLLIDER_ENEMY, App->entity_manager);
}

Entity::~Entity()
{

}

const Collider* Entity::GetCollider() const
{
	return collider;
}

