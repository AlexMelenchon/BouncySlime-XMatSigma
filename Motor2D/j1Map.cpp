#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1Textures.h"
#include <math.h>
#include "j1Audio.h"
#include "j1EntityManager.h"

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

	pugi::xml_node map;
	int numberOfMaps = 0;

	//Load the list of maps
	for (map = config.child("level"); map; map = map.next_sibling("level"))
	{
		MapInfo* newMap = new MapInfo();
		newMap->name = map.attribute("name").as_string();
		newMap->position = numberOfMaps;
		numberOfMaps++;
		
		data.maplist.add(newMap);
	}
	data.maplist = data.maplist;

	return ret;
}

//Draws the current map (is called in the scene)
void j1Map::Draw()
{
 BROFILER_CATEGORY("Map Draw", Profiler::Color::Aqua)

	if(map_loaded == false)
		return;
	p2List_item<LayerInfo*>* layerIt = data.layerList.start;
	
	//Variables that do not change between layers
	float scale = App->win->GetScale();
	SDL_Rect camera = App->render->camera;

	//Create internal variables here in order to not create 1 for each loop iteration
	iPoint wPos = { 0,0 };
	SDL_Rect r;
	float parallaxSpeed;

	//Iterate all layers that are avaliable
	for (layerIt; layerIt != NULL; layerIt = layerIt->next)
	{
		LayerInfo* layer = layerIt->data;

		if (layer->draw == true)
		{
			for (int y = 0; y < data.height; ++y) //From array to array 2D
			{
				for (int x = 0; x < data.width; ++x) //From array to array 2D
				{
					int tile_id = layer->Get(x, y); //We get the id in each position
					if (tile_id > 0)
					{
						TileSet* tileset = GetTilesetFromTileId(tile_id); //We get the tileset of the layer
						if (tileset != nullptr)
						{
							SDL_Rect r = tileset->GetTileRect(tile_id);
							wPos = MapToWorld(x, y);
							parallaxSpeed = layer->fParallaxSpeed;

							//Camera culling: x axis
							if (wPos.x > -camera.x * parallaxSpeed / scale - data.tile_width && wPos.x < -camera.x * parallaxSpeed / scale + camera.w)
								//Camera culling: y axis
								if (wPos.y > -camera.y * parallaxSpeed / scale - data.tile_height && wPos.y < -camera.y + camera.h)
									App->render->Blit(tileset->texture, wPos.x, wPos.y, &r, parallaxSpeed);
						}
					}
				}
			}
		}
	}	

}
bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	p2List_item<LayerInfo*>* item;
	item = data.layerList.start;

	for (item = data.layerList.start; item != NULL; item = item->next)
	{
		LayerInfo* layer = item->data;

		if (!layer->navigation)
		{
			continue;
		}

			uchar* map = new uchar[layer->width*layer->height];
			memset(map, 1, layer->width*layer->height);

			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int i = (y*layer->width) + x;

					int tile_id = layer->Get(x, y);
					TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

					if (tileset != NULL)
					{
						if(tile_id > 0)
						map[i] = 0;
						else
						map[i] = 1;
					}
				}
			}

			*buffer = map;
			width = data.width;
			height = data.height;
			ret = true;

			break;
	}
	

	return ret;
}
//Returns x,y coordinates in the world
iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_width;

	return ret;
}

//Returns x,y coordinates in the Map
iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	ret.x = x / data.tile_width;
	ret.y = y / data.tile_width;

	return ret;
}

//Gives the tileset of an specific id
TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}
	return set;
}

//Return a rect in a specified id in the tileset
SDL_Rect TileSet::GetTileRect(uint tileId)
{

	SDL_Rect toReturn;

	int relative_id = tileId - firstgid;

	toReturn.w = tile_width;
	toReturn.h = tile_height;

	toReturn.x = margin + ((toReturn.w + spacing) * (relative_id % num_tiles_width));
	toReturn.y = margin + ((toReturn.h + spacing) * (relative_id / num_tiles_width));



	return(toReturn);
};



// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");
	Mix_FadeOutMusic(200);

	map_loaded = false;


	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		App->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<LayerInfo*>* item2;
	item2 = data.layerList.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layerList.clear();

	// Remove all the collision stored related to the map
	App->collision->CleanMap();

	// Remove all the entities stored related to the map
	App->entities->CleanMapEnt();

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

	data.currentmap.create(file_name);
	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		LayerInfo* layerInfo = new LayerInfo();

		ret = loadLayer(layer, layerInfo);

		if (ret == true)
			data.layerList.add(layerInfo);
	}
	// Load colliders info ----------------------------------------------
	for (pugi::xml_node nodeColliderGroup = map_file.child("map").child("objectgroup"); nodeColliderGroup; nodeColliderGroup = nodeColliderGroup.next_sibling("objectgroup"))
	{
		for (pugi::xml_node nodeCollider = nodeColliderGroup.child("object"); nodeCollider; nodeCollider = nodeCollider.next_sibling("object"))
		{
			loadCollider(nodeCollider);
		}

	}

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
		
		p2List_item<LayerInfo*>* item_layer = data.layerList.start;
		while(item_layer != NULL)
		{
			LayerInfo* l = item_layer->data;
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
		data.music = map.child("properties").child("property").attribute("value").as_string();
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

//Loads all the information we need from the tileset (.tmx)
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

//Loads the image from the tileset (.png)
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

//Loads a layer from the tileset
bool j1Map::loadLayer(pugi::xml_node& node, LayerInfo* layerInfo)
{
	bool ret = true;

	layerInfo->name = node.attribute("name").as_string();
	layerInfo->width = node.attribute("width").as_uint();
	layerInfo->height = node.attribute("height").as_uint();

	//We create string to compare the attributes we have strored in teh layers
	p2SString speed("speed");
	p2SString navigation("navigation");
	p2SString draw("draw");

	for (pugi::xml_node propIterator = node.child("properties").first_child(); propIterator; propIterator = propIterator.next_sibling())
	{
		if(speed == propIterator.attribute("name").as_string())
			layerInfo->fParallaxSpeed = propIterator.attribute("value").as_float(0.0f);
		if (navigation == propIterator.attribute("name").as_string())
			layerInfo->navigation = propIterator.attribute("value").as_bool(true);
		if (draw == propIterator.attribute("name").as_string())
			layerInfo->draw = propIterator.attribute("value").as_bool(true);

	}	

	pugi::xml_node layer_data = node.child("data");


	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layerInfo);
	}
	else
	{
		layerInfo->tileArray = new uint[layerInfo->width * layerInfo->height];

		memset(layerInfo->tileArray, 0, layerInfo->width * layerInfo->height);

		uint i = 0;
		for (pugi::xml_node tile = node.child("data").child("tile"); tile; tile = tile.next_sibling("tile")) {
			layerInfo->tileArray[i] = tile.attribute("gid").as_uint(0);
			++i;
		}

	}

	return ret;
}

//Loads colliders and sorts each type
bool j1Map::loadCollider(pugi::xml_node& node)
{
	bool ret = true;

	//First we store all the info
	SDL_Rect colliderRect;

	colliderRect.x = node.attribute("x").as_int();
	colliderRect.y = node.attribute("y").as_int();
	colliderRect.w = node.attribute("width").as_int();
	colliderRect.h = node.attribute("height").as_int();
	p2SString name = node.attribute("name").as_string();


	//Then we asign a type for each collider
	COLLIDER_TYPE type;

	if (name == "Floor")
		type = COLLIDER_WALL;

	else if (name == "Death")
		type = COLLIDER_DEATH;
	else if (name == "Win")
		type = COLLIDER_WIN;
	else if (name == "Start")
	{
		type = COLLIDER_START;
		App->entities->player->SetPos(colliderRect.x, colliderRect.y);
		App->entities->player->ReSetMovement();
		return ret;
	}
	else if (name == "EnemyL")
	{
		j1Entity* tmp = App->entities->AddEntity(entityType::LAND_ENEMY, { colliderRect.x, colliderRect.y });
		return ret;
	}
	else if (name == "EnemyF")
	{
		//App->entities->AddEntity(entityType::FLYING_ENEMY, { 400, 935 });
		return ret;
	}
	else
		type = COLLIDER_NONE;
		
	//If there is no error, we add the collider to the list
	if (type == COLLIDER_NONE)
	{
		LOG("Error loading collider. Type not especified");
		ret = false;
	}
	else
	{
		Collider* collision  = new Collider(colliderRect, type, nullptr);
		App->collision->AddCollider(collision);
	}

	return ret;
}

const char* j1Map::GetNextMap()
{
	p2List_item<MapInfo*>* iterator = data.maplist.start;

	while (iterator != NULL)
	{
		if (iterator->data->name == App->map->data.currentmap)
		{
			iterator = iterator->next;
			if (iterator == NULL)
			{
				iterator = data.maplist.start;
				break;
			}
			else
				break;
			
		}
		iterator = iterator->next;
	}

	return iterator->data->name.GetString();

};