#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2SString.h"
#include "p2Point.h"
#include "j1Module.h"




struct LayerInfo 
{
	p2SString name;
	uint width = 0u;
	uint height = 0u;
	uint* tileArray = nullptr;
	uint size = 0;
	float fParallaxSpeed = 0.0f;
	bool navigation = false;
	bool draw = true;

	LayerInfo() : tileArray(NULL)
	{}

	~LayerInfo()
	{
		RELEASE(tileArray);
	}

	//Gets the layer id from the position
	inline uint Get(int x, int y) const {

		return tileArray[x + (y * width)];
	}
};


struct TileSet
{

	p2SString			name;
	int					firstgid = 0;
	int					margin = 0;
	int					spacing = 0;
	int					tile_width = 0;
	int					tile_height = 0;
	SDL_Texture*		texture = nullptr;
	int					tex_width = 0;
	int					tex_height = 0;
	int					num_tiles_width = 0;
	int					num_tiles_height = 0;
	int					offset_x = 0;
	int					offset_y = 0;

	//Return a rect in a specified id in the tileset
	SDL_Rect GetTileRect(uint tileId);
};

struct MapInfo
{
	p2SString			name;
	int					position = 0; // Map position in the game's loop
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
	int					width = 0;
	int					height = 0;
	int					tile_width = 0;
	int					tile_height = 0;
	SDL_Color			background_color = {0,0,0,0};
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<LayerInfo*> layerList;
	p2List<MapInfo*>	maplist;
	p2SString			currentmap;
	p2SString			music;

};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:
	//----------INTERNAL CONTROL-----------//
	//Constructor
	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	//-------------MAP--------------//
	// Load new map
	bool Load(const char* path);

	//Returns x,y coordinates in the world
	iPoint MapToWorld(int x, int y) const;

	//Returns x,y coordinates in the Map
	iPoint WorldToMap(int x, int y) const;

	//Returns true if the path is a map on the list
	const char* MapExist(const char* mapPath) const;

private:

	//Load map data
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool loadLayer(pugi::xml_node& node, LayerInfo* layer);
	bool loadObjects(pugi::xml_node& node);
	//Gets an id and return its tileset
	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;
	const char* GetNextMap();
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;
	p2SString menu_tmx;


private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__