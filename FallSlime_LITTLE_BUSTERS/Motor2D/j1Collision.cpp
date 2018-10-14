#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Defs.h"
#include "p2Log.h"


j1Collision::j1Collision()
{
	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_NEXT_LEVEL] = false;
	matrix[COLLIDER_WALL][COLLIDER_GOD] = true;
	matrix[COLLIDER_WALL][COLLIDER_DEATH] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_NEXT_LEVEL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_GOD] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;

	matrix[COLLIDER_DEATH][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEATH][COLLIDER_NEXT_LEVEL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_GOD] = false;
	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
 
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_WALL] = false;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_DEATH] = false;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_GOD] = true;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_NEXT_LEVEL] = false;

	matrix[COLLIDER_GOD][COLLIDER_WALL] = true;
	matrix[COLLIDER_GOD][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_GOD][COLLIDER_DEATH] = false;
	matrix[COLLIDER_GOD][COLLIDER_NEXT_LEVEL] = true;
	matrix[COLLIDER_GOD][COLLIDER_GOD] = false;

}

// Destructor

j1Collision::~j1Collision()
{}

bool j1Collision::Update(float dt)
{
	// Remove all colliders scheduled for deletion
	p2List_item<Collider*>* item = nullptr;
	p2List_item<Collider*>* item_2 = nullptr;
	Collider* c1;
	Collider* c2;
	item = colliders.start;

	int deleted_colliders = 0;

	while (item != NULL)
	{
		if (item->data != nullptr && item->data->to_delete == true)
		{
		
			if (item->next == NULL)
			{
				RELEASE(item->data);
				colliders.del(item);
				item = item->next;
			}
			else
			{
				RELEASE(item->data);
				item = item->next;
				colliders.del(item->prev);
			}
			++deleted_colliders;
		
		}
		else
			item = item->next;
	}
	
	if (deleted_colliders != 0)
	{
		LOG("Deleted colliders %i", deleted_colliders);
	}

	// Collision detection and callbacks 
	item = colliders.start;

	while (item != NULL)
	{
		// Skip empty colliders
		if (item->data == nullptr)
		{
			item = item->next;
			continue;
		}

		c1 = item->data;


		if (item != NULL)
		{
			item_2 = item->next;
		}


		// Avoid checking collisions already checked

		while (item_2 != NULL)
		{
			if (item_2->data == nullptr)
			{
				item_2 = item_2->next;
				continue;
			}
				
			c2 = item_2->data;

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}

			item_2 = item_2->next;
		}

		item = item->next;
	}
	return true;
}

// Called before render is available
bool j1Collision::PostUpdate()
{
	if (App->input->keyboard[SDL_SCANCODE_F1] == KEY_DOWN) {
		debug = !debug;
	}
		

	if (debug == false)
		return true;

	Uint8 alpha = 90;
	p2List_item<Collider*>* item;
	item = colliders.start;


	while (item != NULL)
	{
		// Skip empty colliders
		if (item->data == nullptr)
		{
			item = item->next;
			continue;
		}
		
		// Draw quads
		switch (item->data->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(item->data->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(item->data->rect, 50, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(item->data->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_DEATH: // red
			App->render->DrawQuad(item->data->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_NEXT_LEVEL:
			App->render->DrawQuad(item->data->rect, 255, 215, 0, alpha);
			break;
		case COLLIDER_GOD:
			App->render->DrawQuad(item->data->rect, 255, 128, 0, alpha);
			break;
		}
		item = item->next;

	}
	return true;
}

// Called before quitting
bool j1Collision:: CleanUp()
{
	LOG("Freeing all colliders");

	// Remove all tilesets
	p2List_item<Collider*>* item;
	item = colliders.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	colliders.clear();

	return true;
}

Collider * j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* collider = new Collider(rect, type, callback);
	colliders.add(collider);
	return  collider;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return !(r.x >= (rect.x + rect.w) || (r.x + r.w) <= rect.x || r.y >= (rect.y + rect.h) || (r.y + r.h) <= rect.y);
}
