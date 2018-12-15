#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "Brofiler/Brofiler.h"



j1Collision::j1Collision()
{
	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_NEXT_LEVEL] = false;
	matrix[COLLIDER_WALL][COLLIDER_GOD] = true;
	matrix[COLLIDER_WALL][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_WALL][COLLIDER_ATTACK] = true;
	matrix[COLLIDER_WALL][COLLIDER_COIN] = false;


	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_DEATH] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_NEXT_LEVEL] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_GOD] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_ATTACK] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_COIN] = false;


	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_NEXT_LEVEL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_GOD] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ATTACK] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_COIN] = true;


	matrix[COLLIDER_DEATH][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEATH][COLLIDER_NEXT_LEVEL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_GOD] = false;
	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_DEATH][COLLIDER_ATTACK] = false;
	matrix[COLLIDER_DEATH][COLLIDER_COIN] = false;

 
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_WALL] = false;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_DEATH] = false;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_GOD] = true;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_NEXT_LEVEL] = false;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_ATTACK] = true;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_COIN] = false;


	matrix[COLLIDER_GOD][COLLIDER_WALL] = true;
	matrix[COLLIDER_GOD][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_GOD][COLLIDER_DEATH] = false;
	matrix[COLLIDER_GOD][COLLIDER_NEXT_LEVEL] = true;
	matrix[COLLIDER_GOD][COLLIDER_GOD] = false;
	matrix[COLLIDER_GOD][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_GOD][COLLIDER_ATTACK] = false;
	matrix[COLLIDER_GOD][COLLIDER_COIN] = true;
	
	matrix[COLLIDER_ATTACK][COLLIDER_WALL] = true;
	matrix[COLLIDER_ATTACK][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_ATTACK][COLLIDER_DEATH] = false;
	matrix[COLLIDER_ATTACK][COLLIDER_NEXT_LEVEL] = true;
	matrix[COLLIDER_ATTACK][COLLIDER_GOD] = false;
	matrix[COLLIDER_ATTACK][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_ATTACK][COLLIDER_ATTACK] = false;
	matrix[COLLIDER_ATTACK][COLLIDER_COIN] = true;

	matrix[COLLIDER_COIN][COLLIDER_WALL] = false;
	matrix[COLLIDER_COIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_COIN][COLLIDER_DEATH] = false;
	matrix[COLLIDER_COIN][COLLIDER_NEXT_LEVEL] = false;
	matrix[COLLIDER_COIN][COLLIDER_GOD] = true;
	matrix[COLLIDER_COIN][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_COIN][COLLIDER_COIN] = false;
	matrix[COLLIDER_COIN][COLLIDER_ATTACK] = true;
}

// Destructor

j1Collision::~j1Collision()
{}

bool j1Collision::Update(float dt)
{
	BROFILER_CATEGORY("Collision Update", Profiler::Color::LightPink);

	// Remove all colliders scheduled for deletion
	p2List_item<Collider*>* collider = colliders.start;
	p2List_item<Collider*>* iterator = nullptr;
	p2List_item<Collider*>* item_2 = nullptr;
	Collider* c1 = nullptr;
	Collider* c2 = nullptr;

	int deleted_colliders = 0;

	// Collision detection and callbacks ============================================== 
	collider = colliders.start;

	while (collider != NULL)
	{
		c1 = collider->data;


		if (collider != NULL)
		{
			item_2 = collider->next;
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

		collider = collider->next;
	}
	return true;
}

// Called before render is available
bool j1Collision::PostUpdate()
{
	BROFILER_CATEGORY("Collision PostUpdate", Profiler::Color::LightSalmon);

	if (App->input->keyboard[SDL_SCANCODE_F9] == KEY_DOWN) {
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
		case COLLIDER_NEXT_LEVEL:  // yellow
			App->render->DrawQuad(item->data->rect, 255, 215, 0, alpha);
			break;
		case COLLIDER_GOD: // orange
			App->render->DrawQuad(item->data->rect, 255, 128, 0, alpha);
			break;
		case COLLIDER_ENEMY: // violet
			App->render->DrawQuad(item->data->rect, 132, 48, 206, alpha);
			break;
		case COLLIDER_ATTACK: // pink
			App->render->DrawQuad(item->data->rect, 199, 21, 133, alpha);
			break;
		case COLLIDER_COIN: // yellow
			App->render->DrawQuad(item->data->rect, 255, 255, 0, alpha);
			break;
		}
		item = item->next;
	}
	return true;
}

// Called before quitting
bool j1Collision:: CleanUp()
{
	BROFILER_CATEGORY("Collision CleanUp", Profiler::Color::LightSeaGreen);

	LOG("Freeing all colliders");

	// Remove all colliders =====================
	int count = 0;
	for (p2List_item<Collider*>* item = colliders.start; item; item = item->next)
	{
		if (item->data)
		{
			++count;
			RELEASE(item->data);
		}

	}
	colliders.clear();

	LOG("Deleted %i colliders", count);


	return true;
}

Collider * j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* collider = new Collider(rect, type, callback);
	
	return  colliders.add(collider)->data;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return !(r.x >= (rect.x + rect.w) || (r.x + r.w) <= rect.x || r.y >= (rect.y + rect.h) || (r.y + r.h) <= rect.y);
}

bool j1Collision::CheckOverlap(p2List<Direction> &directions , Collider *dynamic_col, COLLIDER_TYPE type, fPoint &position, fPoint &velocity)
{
	for (p2List_item<Collider*> * item = colliders.start; item; item = item->next)
	{
		if (type != item->data->type  || item->data == dynamic_col)
		{
			continue;
		}

		if (dynamic_col->CheckCollision(item->data->rect))
		{
			directions.add(SolveOverlap(dynamic_col, item->data, position, velocity));
		}
	}

	return true;
}

bool j1Collision::DeleteCollider(Collider * collider)
{
	if (colliders.count() == 0)
	{
		return true;
	}

	for (p2List_item<Collider*> * item = colliders.start; item != nullptr; item = item->next)
	{
		if (item->data == collider)
		{
			RELEASE(item->data);
			colliders.del(item);
			return true;
		}
	}

	LOG("Collider to delete not found");
	return false;
}

Direction j1Collision::SolveOverlap(Collider *dynamic_col, Collider *static_col, fPoint &position , fPoint &velocity)
{
	SDL_Rect dynamic = dynamic_col->rect;
	SDL_Rect rigid = static_col->rect;

	bool directions[(uint)Direction::unknown];
	directions[(uint)Direction::left] = velocity.x < 0;
	directions[(uint)Direction::right] = velocity.x > 0;
	directions[(uint)Direction::up] = velocity.y < 0;
	directions[(uint)Direction::down] = velocity.y > 0;

	uint distances[(uint)Direction::unknown];
	distances[(uint)Direction::right] = dynamic.x + dynamic.w - rigid.x;
	distances[(uint)Direction::left] = rigid.x + rigid.w - dynamic.x;
	distances[(uint)Direction::up] = rigid.y + rigid.h - dynamic.y;
	distances[(uint)Direction::down] = dynamic.y + dynamic.h - rigid.y;

	int offset_direction = -1;

	for (uint i = 0; i < (uint)Direction::unknown; ++i)
	{
		if (directions[i]) {

			if (offset_direction == -1)
				offset_direction = i;
			else if (distances[i] < distances[(uint)offset_direction])
				offset_direction = i;
		}
	}

	if (offset_direction == -1)
	{
		return Direction::unknown;
	}

	switch ((Direction)offset_direction)
	{
	case Direction::right:
		position.x = (float)rigid.x - (float)dynamic.w / 2.0f;
		break;
	case Direction::left:
		position.x = (float)rigid.x + (float)rigid.w + (float)dynamic.w / 2.0f;
		break;
	case Direction::up:
		position.y = (float)rigid.y + (float)rigid.h + (float)dynamic.h / 2.0f;
		break;
	case Direction::down:
		position.y = (float)rigid.y - (float)dynamic.h / 2.0f;
		break;
	}
	return (Direction)offset_direction;
}
