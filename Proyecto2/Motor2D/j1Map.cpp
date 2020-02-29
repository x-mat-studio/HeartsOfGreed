#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Window.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{
}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	map_name.create(config.child("load").attribute("mapname").as_string());
	return ret;
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;


	uint window_w;
	uint window_h;
	App->win->GetWindowSize(window_w, window_h);


	int i=0;

	while (i < data.layers.size())
	{
		int scale = App->win->GetScale();

		int up_left_cam_cornerX;
		int up_left_cam_cornerY;
		int down_right_cam_cornerX;
		int down_right_cam_cornerY;

		WorldToMap(App->render->camera.x * -1, App->render->camera.y * -1, data,up_left_cam_cornerX,up_left_cam_cornerY);
		WorldToMap((App->render->camera.x * -1) + window_w, (App->render->camera.y * -1) + window_h, data,down_right_cam_cornerX,down_right_cam_cornerY);

		for (int i = 0; i < data.layers[i]->height; i++)//number of rows
		{
			for (int j = 0; j < data.layers[i]->width; j++)//number of columns
			{

				if (i<down_right_cam_cornerY + 1 && i>up_left_cam_cornerY - 1)//These are a camera culling implementation the game just draws what's seen in the camera
				{
					if (j<down_right_cam_cornerX + 1 && j>up_left_cam_cornerX - 1)
					{
						int id = data.layers[i]->gid[Get(j, i, data.layers[i])];
						if (id > 0)
						{
							float worldX;
							float worldY;
							MapToWorldCoordinates(j, i, data, worldX, worldY);
							App->render->Blit(GetTilesetFromTileId(id)->texture,worldX, worldY, &RectFromTileId(id, GetTilesetFromTileId(id)));
						}
					}
				}
			}
		}
	}

	//LOG("%i %i,%i %i", up_left_cam_corner.x, up_left_cam_corner.y, down_right_cam_corner.x, down_right_cam_corner.y);

	i++;
	
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	TileSet* set = nullptr;

	int numTilesets = data.tilesets.size();
	for(int i = 0; i < numTilesets; i++)
	{
		if (id < data.tilesets[i]->firstgid)
		{
			set = data.tilesets[i-1];
			break;
		}

		set = data.tilesets[i];
	}
	return set;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	int numTilesets = data.tilesets.size();

	for (int i = 0; i < numTilesets; i++)
	{
		App->tex->UnLoad(data.tilesets[i]->texture);
		RELEASE(data.tilesets[i]);
	}
	data.tilesets.clear();

	// Remove all layers
	int numLayers = data.layers.size();

	for(int i = 0; i < numLayers; i++)
	{
		int numProperties = data.layers[i]->layerPropVector.size();

		for(int j = 0; j < numProperties; j++)
		{
			RELEASE(data.layers[i]->layerPropVector[j]);
		}
		data.layers[i]->layerPropVector.clear();

		RELEASE(data.layers[i]);
	}
	data.layers.clear();

	//Remove all the objectlayers
	int numObjectGroups = data.objGroups.size();

	for(int i = 0; i < numObjectGroups; i++)
	{
		//remove all the objects
		int numObjects = data.objGroups[i]->objectVector.size();
		
		for(int j = 0; j < numObjects; j++)
		{
			RELEASE(data.objGroups[i]->objectVector[j]);
		}
		data.objGroups[i]->objectVector.clear();

		//remove all the properties
		int numProperties = data.objGroups[i]->propVector.size();

		for(int j = 0; j < numProperties; j++)
		{
			RELEASE(data.objGroups[i]->propVector[j]);
		}
		data.objGroups[i]->propVector.clear();

		RELEASE(data.objGroups[i]);
	}
	data.objGroups.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::LoadNew(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap(tmp.GetString(), file_name);
	}

	// Load music ----------------------------------------------------------

	if (ret == true) {
		data.music_path = map_file.child("map").child("properties").child("property").attribute("value").as_string();
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

		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node currentlayer;
	for (currentlayer = map_file.child("map").child("layer"); currentlayer && ret; currentlayer = currentlayer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(currentlayer, set);
		}

		data.layers.push_back(set);
	}
	//Load object info-----------------------------------------------
	for (pugi::xml_node currentobjgroup = map_file.child("map").child("objectgroup"); currentobjgroup && ret; currentobjgroup = currentobjgroup.next_sibling("objectgroup"))
	{
		ObjectGroup* set = new ObjectGroup();

		if (ret == true)
		{
			ret = LoadObjGroup(currentobjgroup, set);
		}

		data.objGroups.push_back(set);
	}

	map_loaded = ret;
	// Clean up the pugui tree
	map_file.reset();
	return ret;
}

// Load map general properties
bool j1Map::LoadMap(p2SString path, p2SString name)
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.name = name;
		data.path = path;
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
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
	set->columns = tileset_node.attribute("columns").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
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

	if (image == NULL)
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

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

//Loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& layer_node, MapLayer* layer)
{
	bool ret = true;
	layer->name.create(layer_node.attribute("name").as_string());
	layer->width = layer_node.attribute("width").as_uint();
	layer->height = layer_node.attribute("height").as_uint();
	layer->gid = new uint[layer->width * layer->height];
	memset(layer->gid, 0, layer->width * layer->height * sizeof(uint));

	//Load properties--------------------------------------------
	for (pugi::xml_node currentprop = layer_node.child("properties").child("property"); currentprop; currentprop = currentprop.next_sibling("property"))
	{
		Properties* set = new Properties;
		set->name = currentprop.attribute("name").as_string();
		set->value = currentprop.attribute("value").as_float();
		layer->layerPropVector.push_back(set);
	}

	pugi::xml_node* gid_iterator = &layer_node.child("data").child("tile");
	for (int i = 0; i < layer->width * layer->height; i++)
	{

		layer->gid[i] = gid_iterator->attribute("gid").as_uint();
		if (gid_iterator->attribute("gid").as_uint() == NULL) {
			layer->gid[i] = 0;
		}

		gid_iterator = &gid_iterator->next_sibling("tile");
	}


	return ret;
}

bool j1Map::LoadObjGroup(pugi::xml_node& objgroupnode, ObjectGroup* group)
{
	bool ret = true;
	group->name.create(objgroupnode.attribute("name").as_string());
	group->id = objgroupnode.attribute("id").as_uint();

	//Load properties--------------------------------------------
	for (pugi::xml_node currentprop = objgroupnode.child("properties").child("property"); currentprop; currentprop = currentprop.next_sibling("property"))
	{
		Properties* set = new Properties;
		set->name = currentprop.attribute("name").as_string();
		set->value = currentprop.attribute("value").as_bool();
		group->propVector.push_back(set);
	}

	//Load objects-----------------------------------------------
	for (pugi::xml_node currentobj = objgroupnode.child("object"); currentobj; currentobj = currentobj.next_sibling("object"))
	{
		Object* set = new Object;
		set->id = currentobj.attribute("id").as_uint();
		set->type = currentobj.attribute("type").as_int();
		set->boundingBox.x = currentobj.attribute("x").as_int();
		set->boundingBox.y = currentobj.attribute("y").as_int();
		set->boundingBox.h = currentobj.attribute("height").as_int();
		set->boundingBox.w = currentobj.attribute("width").as_int();

		group->objectVector.push_back(set);
	}

	return ret;
}


//gets the position in a 1 row vector
//X horizontal, Y vertical, data.layers.(currentlayer)
inline uint j1Map::Get(int x, int y, MapLayer* currentlayer) const
{
	return (y * currentlayer->width) + x;
}

//converts an id from a 1 row vector(Tileset position) into a Rectangle
SDL_Rect j1Map::RectFromTileId(uint tile_id, TileSet* currenttileset)
{
	int firstgid = currenttileset->firstgid;
	int margin = currenttileset->margin;
	int spacing = currenttileset->spacing;
	int texturetile_width = currenttileset->tile_width;//has to be improved in case the tile from the image is not the same size as the output tile
	int texturetile_height = currenttileset->tile_height;//to do that calculate tile width and height takin into account the image w/h and spacing + margin
	int columns = currenttileset->columns;

	SDL_Rect rect;
	rect.h = currenttileset->tile_height;
	rect.w = currenttileset->tile_height;

	int tileX = (tile_id - firstgid) % columns;//number of tiles full in a row
	int tileY = (tile_id - firstgid) / columns;//number of rows full
	rect.x = margin + (tileX * texturetile_width) + tileX * spacing;
	rect.y = margin + (tileY * texturetile_height) + tileY * spacing;

	return rect;
}

//isometric map to world
void j1Map::MapToWorldCoordinates(int posX,int posY, MapData& dat,float& outX,float& outY)
{
	outX = (posX-posY)*dat.tile_width*0.5f; 
	outY = (posX + posY) * dat.tile_height * 0.5f;
}

//isometric world to map
void j1Map::WorldToMap(int x, int y, MapData& dat,int& outX, int& outY) const
{
	outX = (x / (dat.tile_width * 0.5f) + y / (dat.tile_height * 0.5f)) * 0.5f;
	outY = (y / (dat.tile_height * 0.5f) - (x / (dat.tile_width * 0.5f))) * 0.5f;

}

bool j1Map::ReloadMap(p2SString newmap)
{
	CleanUp();//clears the map

	LOG("Loading Map Parser");
	bool ret = true;

	LoadNew(newmap.GetString());

	return ret;
}

TileSet::~TileSet()
{
	if (this->texture != NULL)
		App->tex->UnLoad(this->texture);
	this->name.Clear();
}
MapLayer::~MapLayer()
{
	if (this->gid != nullptr)
		delete this->gid;
}

bool j1Map::Load(pugi::xml_node& ldata)
{
	p2SString newMapname = ldata.attribute("name").as_string("first_level.tmx");//loads the map name from the saves doc, if gets errror, replaces it with "first_level.tmx"
	if (newMapname != data.name)//if the map that you request to load isn't the same as the one you are currently in, load it, else do nothing 
	{
		ReloadMap(newMapname);
	}
	return true;

}
bool j1Map::Save(pugi::xml_node& ldata) const
{
	ldata.append_attribute("name") = data.name.GetString();
	return true;
}