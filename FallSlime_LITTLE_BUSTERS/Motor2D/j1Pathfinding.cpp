#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1PathFinding.h"
#include "j1Input.h"
#include "j1Map.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");

}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}
bool j1PathFinding::PreUpdate()
{
	// Debug pathfing =====================================
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x + App->render->camera.x , y + App->render->camera.y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true && IsWalkable(p))
		{
			App->path_finding->DeleteLastPath();
			App->path_finding->CreatePath(origin, p);
			origin_selected = false;
		}
		else if (origin_selected == false && IsWalkable(p))
		{
			origin = p;
			origin_selected = true;
		}
		else
			origin_selected = false;
	}
	return true;
}

bool j1PathFinding::PostUpdate()
{

	//  Draw Walkable map ----------------------------
	for (int j = 0 ; j < height ; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			if (!IsWalkable(iPoint(i, j)))
			{
				App->render->DrawQuad({ i * 16, j * 16, 16, 16}, 255, 255 ,255, 100);
			}
		}
	}

	// Draw Final path ------------------------------

	const p2DynArray<iPoint>* path = GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->DrawQuad({ pos.x, pos.y ,16,16 }, 0, 0, 0, 200);
	}

	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

void j1PathFinding::DeleteLastPath() 
{
	last_path.Clear();
}


// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
const p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(float g, float h, const iPoint& pos, PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill, iPoint destination)
{
	iPoint position;
	iPoint offset;
	fPoint distance;
	fPoint destin(destination.x, destination.y);
	
	// south-west 
	offset.create(-1, 1);
	position.create(this->pos.x + offset.x, this->pos.y + offset.y);
	distance.create(position.x, position.y);
	if (App->path_finding->IsWalkable(position))
		list_to_fill.list.add(PathNode(this->g +1.4f, distance.DistanceTo(destin), position, this));

	// south-est
	offset.create(1, 1);
	position.create(this->pos.x + offset.x, this->pos.y + offset.y);
	distance.create(position.x, position.y);
	if (App->path_finding->IsWalkable(position))
		list_to_fill.list.add(PathNode(this->g + 1.4f, distance.DistanceTo(destin), position, this));

	// north-east
	offset.create(1, -1);
	position.create(this->pos.x + offset.x, this->pos.y + offset.y);
	distance.create(position.x, position.y);
	if (App->path_finding->IsWalkable(position))
		list_to_fill.list.add(PathNode(this->g +1.4f, distance.DistanceTo(destin), position, this));

	// north-west
	offset.create(-1, -1);
	position.create(this->pos.x + offset.x, this->pos.y + offset.y);
	distance.create(position.x, position.y);
	if (App->path_finding->IsWalkable(position))
		list_to_fill.list.add(PathNode(this->g + 1.4f, distance.DistanceTo(destin), position, this));
	// west
	offset.create(-1, 0);
	position.create(this->pos.x + offset.x, this->pos.y + offset.y);
	distance.create(position.x, position.y);
	if (App->path_finding->IsWalkable(position))
		list_to_fill.list.add(PathNode(this->g + 1, distance.DistanceTo(destin), position, this));
	// south
	offset.create(0, 1);
	position.create(this->pos.x + offset.x, this->pos.y + offset.y);
	distance.create(position.x, position.y);
	if (App->path_finding->IsWalkable(position))
		list_to_fill.list.add(PathNode(this->g + 1, distance.DistanceTo(destin), position, this));
	// east
	offset.create(1, 0);
	position.create(this->pos.x + offset.x, this->pos.y + offset.y);
	distance.create(position.x, position.y);
	if (App->path_finding->IsWalkable(position))
		list_to_fill.list.add(PathNode(this->g + 1, distance.DistanceTo(destin), position, this));
	// north
	offset.create(0, -1);
	position.create(this->pos.x + offset.x, this->pos.y + offset.y);
	distance.create(position.x, position.y);
	if (App->path_finding->IsWalkable(position))
		list_to_fill.list.add(PathNode(this->g + 1, distance.DistanceTo(destin), position, this));


	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
float PathNode::Score() const
{
	return (float)g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	// If origin or destination are not walkable, return -1
	if (IsWalkable(origin) == false && IsWalkable(destination) == false)
	{
		return -1;
	}
	// Create two lists: open, close
	PathList opened;
	PathList closed;

	// Add the origin tile to open
	PathNode origin_node(1, origin.DistanceTo(destination), origin, nullptr);
	opened.list.add(origin_node);

	// Iterate while we have tile in the open list

	while (opened.list.count() != 0)
	{
		// Move the lowest score cell from open list to the closed list
		p2List_item<PathNode>* iterate_node = opened.GetNodeLowestScore();
		p2List_item<PathNode>* current_node = closed.list.add(iterate_node->data);
		opened.list.del(iterate_node);
		iterate_node = nullptr;

		// If we just added the destination, we are done!
		if (current_node->data.pos == destination)
		{
			// Backtrack to create the final path

			for (PathNode* node = (PathNode*)closed.list.end ; node ; node = (PathNode*)node->parent)
			{
				last_path.PushBack(node->pos);
			}
			last_path.Flip();

			return last_path.Count();
		}

		// Fill a list of all adjancent nodes
		PathList adjacent_nodes;
		current_node->data.FindWalkableAdjacents(adjacent_nodes, destination);

		for (p2List_item<PathNode>* item = adjacent_nodes.list.start; item; item = item->next)
		{
			if (closed.Find(item->data.pos) != nullptr)
			{
				continue;
			}

			p2List_item<PathNode>*  found_node_opened = (p2List_item<PathNode>*) opened.Find(item->data.pos);

			if (found_node_opened != nullptr)
			{
				if (found_node_opened->data.Score() > item->data.Score())
				{
					found_node_opened->data.g = item->data.g;
					found_node_opened->data.parent = &current_node->data;
				}
			}
			else  
			{
				opened.list.add(item->data);
			}
		}
	}

	return -1;
}