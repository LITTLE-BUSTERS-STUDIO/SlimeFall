#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	
	return ret;
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;
	int scale = App->win->GetScale(); 
	SDL_Rect cam(App->render->camera);

	TileSet *tileset = nullptr;

	for(p2List_item <MapLayer*>*layer = data.layers.start; layer; layer = layer->next)
	{
		for (uint j = 0; j < data.height; j++)
		{
			for (uint i = 0; i < data.width; i++)
			{
				uint id = layer->data->tiles [layer->data->Get(i, j)];

				tileset = GetTileset(id);

				iPoint map_pos = MapToWorld(i, j);
				SDL_Rect rect = tileset->GetTileRect(id);


				if (id == 0)
				{
					continue;
				}
				
				if (!((cam.x / scale < map_pos.x + rect.w) && (map_pos.x < (cam.x + cam.w) / scale)
					&& (cam.y / scale < map_pos.y + rect.h) && (map_pos.y < (cam.y + cam.h) / scale)))
				{
					continue;
				}

				App->render->Blit(tileset->texture, map_pos.x, map_pos.y, &rect);
			}
		}
	}
}

inline uint MapLayer::Get(int x, int y) const
{

	return y*this->width + x;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0,0);

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;



	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item_2;
	item_2 = data.layers.start;

	while (item_2 != NULL)
	{
		RELEASE(item_2->data);
		item_2 = item_2->next;
	}

	data.layers.clear();

	// Remove all collider groups
	p2List_item<CollidersGroup*>* item_3;
	item_3 = data.coll_groups.start;

	while (item_3 != NULL)
	{
		RELEASE(item_3->data);
		item_3 = item_3->next;
	}

	data.coll_groups.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------

	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, set);
		}
		data.layers.add(set);
	}

	// Load colliders info ----------------------------------------------

	pugi::xml_node objectgroup;

	for (objectgroup = map_file.child("map").child("objectgroup"); objectgroup && ret; objectgroup = objectgroup.next_sibling("objectgroup"))
	{
		CollidersGroup* group = new CollidersGroup();

		if (ret == true)
		{
			ret = LoadColliders(objectgroup, group);
		}

		data.coll_groups.add(group);
	}


	//LOG All info ------------------------------------------------------

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}
		
		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}




bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& layer_node, MapLayer* layer)
{
	
	bool ret = true;

	layer->name = layer_node.attribute("name").as_string();
	layer->width = layer_node.attribute("width").as_int();
	layer->height = layer_node.attribute("height").as_int();
	pugi::xml_node layer_data = layer_node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->tiles = new uint[layer->width*layer->height];
		memset(layer->tiles, 0, layer->width*layer->height);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->tiles[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}


bool j1Map::LoadColliders(pugi::xml_node& object_node, CollidersGroup* group)
{
	bool ret = true;
	
	COLLIDER_TYPE collyder_type;
	group->name.create(object_node.attribute("name").as_string());

	if (group->name.GetString() == "colliders_static")
		collyder_type = COLLIDER_WALL;
	else if (group->name.GetString() == "colliders_death")
		collyder_type = COLLIDER_DEATH;
	else if (group->name.GetString() == "colliders_next_level")
		collyder_type = COLLIDER_NEXT_LEVEL;

	for (pugi::xml_node object_data = object_node.child("object"); object_data; object_data = object_data.next_sibling("object"))
	{
		++group->num_colliders;
	}

	group->colls = new Collider*[group->num_colliders];
	uint counter = 0;

	for (pugi::xml_node object_data = object_node.child("object"); object_data; object_data = object_data.next_sibling("object"))
	{
		SDL_Rect rect;
		rect.x = object_data.attribute("x").as_int(0);
		rect.y = object_data.attribute("y").as_int(0);
		rect.w = object_data.attribute("width").as_int(0);
		rect.h = object_data.attribute("height").as_int(0);
		group->colls[counter] = App->collision->AddCollider(rect, COLLIDER_WALL, this);
		++counter;
	}

	LOG("Added colliders %s----", object_node.attribute("name").as_string());


	return ret;
}

TileSet* j1Map::GetTileset(int id) const
{
	// Tileset based on a tile id

	p2List_item<TileSet*>* tileset = data.tilesets.start;
	while (tileset != NULL)
	{
		if (tileset->next && id < tileset->next->data->firstgid) return tileset->data;
		tileset = tileset->next;
	}
	return data.tilesets.end->data;

}
