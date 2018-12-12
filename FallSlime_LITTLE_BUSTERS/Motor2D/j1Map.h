#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

struct Collider;
enum COLLIDER_TYPE;

struct MapLayer 
{
	p2SString			name;
	uint				width = 0u;
	uint				height = 0u;
	uint*	            tiles = nullptr;
	inline uint Get(int x, int y) const;
	~MapLayer()
	{
		if (tiles != NULL)
		delete[]  tiles;
	}
};
// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			    name;
	uint					firstgid;
	uint					margin;
	uint					spacing;
	uint					tile_width;
	uint					tile_height;
	SDL_Texture*		    texture;
	uint					tex_width;
	uint					tex_height;
	uint					num_tiles_width;
	uint					num_tiles_height;
	uint					offset_x;
	uint					offset_y;
};

struct CollidersGroup
{
	p2SString			    name;
	COLLIDER_TYPE		    type;
	Collider**              colls = nullptr;
	uint                    num_colliders = 0u;

	~CollidersGroup()
	{
		if (colls != nullptr)
		{
			delete[] colls;
			colls = nullptr;
		}
	}
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	uint					      width;
	uint					      height;
	uint					      tile_width;
	uint					      tile_height;
	MapTypes			          type;
	SDL_Color			          background_color;
	
	p2List<TileSet*>	          tilesets;
	p2List<MapLayer*>	          layers;
	p2List<CollidersGroup*>       coll_groups;           

	fPoint                        initial_position = {0.0f,0.0f};
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	TileSet* GetTileset(int id) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadColliders(pugi::xml_node& object_node, CollidersGroup* group);
	bool UnloadMap();

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__