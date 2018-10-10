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
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = false;


	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;


	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;

}

// Destructor

j1Collision::~j1Collision()
{}

bool j1Collision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	p2List_item<Collider*>* item = nullptr;
	p2List_item<Collider*>* item_2 = nullptr;
	Collider* c1;
	Collider* c2;
	item = colliders.start;

	while (item != NULL)
	{
		if (item->data != nullptr && item->data->to_delete == true)
		{
			RELEASE(item->data);
			item->data = nullptr;
		}
		item = item->next;
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
	if (App->input->keyboard[SDL_SCANCODE_F1] == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return true;

	Uint8 alpha = 150;
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
			App->render->DrawCircle(640, 360, 1, 0, 255, 0, alpha); // middle of the screen

			break;
		case COLLIDER_ENEMY: // red
			App->render->DrawQuad(item->data->rect, 255, 0, 0, alpha);
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

    p2List_item <Collider*> * j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* collider = new Collider(rect, type , callback);

	return colliders.add(collider);
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}
