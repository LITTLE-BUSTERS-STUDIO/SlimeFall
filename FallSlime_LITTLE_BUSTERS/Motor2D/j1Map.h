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
		RELEASE_ARRAY(tiles) ;
	}
};
// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			    name;
	uint					firstgid = 0u;
	uint					margin = 0u;
	uint					spacing = 0u;
	uint					tile_width = 0u;
	uint					tile_height = 0u;
	SDL_Texture*		    texture = nullptr;
	uint					tex_width = 0u;
	uint					tex_height = 0u;
	uint					num_tiles_width = 0u;
	uint					num_tiles_height = 0u;
	uint					offset_x = 0u;
	uint					offset_y = 0u;
};

struct CollidersGroup
{
	p2SString			    name;
	COLLIDER_TYPE		    type;
	p2List<Collider*>       colliders;

	~CollidersGroup()
	{

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
	uint					      width = 0u;
	uint					      height = 0u;
	uint					      tile_width = 0u;
	uint					      tile_height = 0u;
	MapTypes			          type;
	SDL_Color			          background_color = {255, 255 , 255 , 255 };
	
	p2List<TileSet*>	          tilesets;
	p2List<MapLayer*>	          layers;
	p2List<CollidersGroup*>       coll_groups;           
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

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__