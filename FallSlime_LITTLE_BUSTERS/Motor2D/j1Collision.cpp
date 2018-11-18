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


	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_DEATH] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_NEXT_LEVEL] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_GOD] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_ATTACK] = true;


	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_NEXT_LEVEL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_GOD] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ATTACK] = false;


	matrix[COLLIDER_DEATH][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEATH][COLLIDER_NEXT_LEVEL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_GOD] = false;
	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_DEATH][COLLIDER_ATTACK] = false;

 
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_WALL] = false;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_DEATH] = false;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_GOD] = true;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_NEXT_LEVEL] = false;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_NEXT_LEVEL][COLLIDER_ATTACK] = true;


	matrix[COLLIDER_GOD][COLLIDER_WALL] = true;
	matrix[COLLIDER_GOD][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_GOD][COLLIDER_DEATH] = false;
	matrix[COLLIDER_GOD][COLLIDER_NEXT_LEVEL] = true;
	matrix[COLLIDER_GOD][COLLIDER_GOD] = false;
	matrix[COLLIDER_GOD][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_GOD][COLLIDER_ATTACK] = false;
	
	matrix[COLLIDER_ATTACK][COLLIDER_WALL] = true;
	matrix[COLLIDER_ATTACK][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_ATTACK][COLLIDER_DEATH] = false;
	matrix[COLLIDER_ATTACK][COLLIDER_NEXT_LEVEL] = true;
	matrix[COLLIDER_ATTACK][COLLIDER_GOD] = false;
	matrix[COLLIDER_ATTACK][COLLIDER_ENEMY] = true;


}

// Destructor

j1Collision::~j1Collision()
{}

bool j1Collision::Update(float dt)
{
	BROFILER_CATEGORY("Collision Update", Profiler::Color::LightPink);

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
bool j1Collision::PostUpdate(float dt)
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
			delete item->data;
			item->data = nullptr;
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

//bool j1Collision::RayCast(fPoint vector, fPoint origin, Collider* collider, COLLIDER_TYPE type)
//{
//	bool ret = false;
//	float divisions = 5;
//	vector.x /= divisions;
//	vector.y /= divisions;
//	Collider collider_1 = *collider;
//	SDL_Rect rect;
//
//	for (int i = 0; i < divisions ; ++i)
//	{
//		collider_1.rect.x += vector.x;
//		collider_1.rect.y += vector.y;
//
//		for (p2List_item<Collider*> * item = colliders.start; item; item = item->next)
//		{
//			if (item->data->type == type)
//			{
//				rect = item->data->rect;
//
//				if (collider_1.CheckCollision(rect))
//				{
//					PreventOverlap(collider_1, item->data);
//					collider->rect.x = collider_1.rect.x;
//					collider->rect.y = collider_1.rect.y;
//
//					return true;
//				}
//
//				
//			}
//		}
//	}
//	return ret;
//}


//bool j1Collision::PreventOverlap(Collider &dynamic_col, Collider *static_col)
//{
//	SDL_Rect dynamic = dynamic_col.rect;
//	SDL_Rect rigid = static_col->rect;
//
//	uint distances[(uint)Direction::unknown];
//	distances[(uint)Direction::right] = dynamic.x + dynamic.w - rigid.x;
//	distances[(uint)Direction::left] = rigid.x + rigid.w - dynamic.x;
//	distances[(uint)Direction::up] = rigid.y + rigid.h - dynamic.y;
//	distances[(uint)Direction::down] = dynamic.y + dynamic.h - rigid.y;
//
//	int shorter_distance = -1;
//	
//	for (uint i = 0; i < (uint)Direction::unknown; ++i)
//	{
//		if (shorter_distance == -1 && distances[i] < 2147483647)
//		{
//			shorter_distance = i;
//		}
//		else if (distances[i] < distances[(uint)shorter_distance])
//		{
//			shorter_distance = i;
//		}
//	}
//
//	switch ((Direction)shorter_distance)
//	{
//	case Direction::right:
//		dynamic_col.rect.x = rigid.x - dynamic.w;
//		break;
//	case Direction::left:
//		dynamic_col.rect.x = rigid.x + rigid.w;
//		break;
//	case Direction::up:
//		dynamic_col.rect.y = rigid.y + rigid.h;
//		break;
//	case Direction::down:
//		dynamic_col.rect.y = rigid.y - dynamic.h;
//		break;
//	}
//	return true;
//}


//bool j1Collision::RayCast(fPoint vector, fPoint origin, Collider* collider, COLLIDER_TYPE type)
//{
//	bool ret = false;
//	fPoint point = origin;
//	float divisions = 5;
//	vector.x /= divisions;
//	vector.y /= divisions;
//	SDL_Rect rect;
//
//	for (int i = 0; i < divisions; ++i)
//	{
//		point += vector;
//
//		for (p2List_item<Collider*> * item = colliders.start; item; item = item->next)
//		{
//			if (item->data->type == type)
//			{
//				rect = item->data->rect;
//				if (point.x > rect.x &&  point.x < (rect.x + rect.w) && point.y > rect.y && point.y < (rect.y + rect.h))
//				{
//
//					int distances[(uint)Direction::unknown];
//					distances[(uint)Direction::right] = rect.x + rect.w - point.x;
//					distances[(uint)Direction::left] = point.x - rect.x;
//					distances[(uint)Direction::up] = point.y - rect.y;
//					distances[(uint)Direction::down] = rect.y + rect.h - point.y;
//
//					int shorter_distance = -1;
//
//					for (uint i = 0; i < (uint)Direction::unknown; ++i)
//					{
//						if (shorter_distance == -1 && distances[i] < 2147483647)
//						{
//							shorter_distance = i;
//						}
//						else if (distances[i] < distances[(uint)shorter_distance])
//						{
//							shorter_distance = i;
//						}
//					}
//
//					switch ((Direction)shorter_distance)
//					{
//					case Direction::right:
//						collider->rect.x = rect.x + rect.w;
//						break;
//					case Direction::left:
//						collider->rect.x = rect.x - collider->rect.w;
//						break;
//					case Direction::up:
//						collider->rect.y = rect.y - collider->rect.h;
//						break;
//					case Direction::down:
//						collider->rect.x = rect.y + rect.h;
//						break;
//					}
//					ret = true;
//				}
//			}
//		}
//	}
//	return ret;
//}

