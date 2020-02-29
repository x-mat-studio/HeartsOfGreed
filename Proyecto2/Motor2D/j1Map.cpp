#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "Enemy.h"
#include "j1EntityManager.h"
#include "Brofiler/Brofiler/Brofiler.h"
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
	BROFILER_CATEGORY("Map Awake", Profiler::Color::BlueViolet)
	bool ret = true;

	folder.create(config.child("folder").child_value());


	return ret;
}

bool j1Map::Start() {
	BROFILER_CATEGORY("Map Start", Profiler::Color::CadetBlue)

	for (int i = 0; i < MAX_MAP_COLLIDERS; i++)
	{
		col[i] = nullptr; //make sure our collider array is reseted
	}

	return true;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("Map Draw", Profiler::Color::Brown)
	if (map_loaded == false) {
		return;
	}

	p2List_item<Layer*>* item_layer = data.layers.start;
	
	while (item_layer != NULL)
	{
		Layer* l = item_layer->data;
		item_layer = item_layer->next;
		for (int i = 0; i <l->width ; i++)
		{
			for (int j = 0; j < l->height; j++)
			{
				int tile_id = l->gid[l->Get(i, j)];
				if (tile_id > 0)
				{
					l->Get(i, j);
					SDL_Texture* texture = data.tilesets.start->data->texture;
					iPoint position = PosConverter(i, j);
					SDL_Rect* sect = &data.tilesets.start->data->TileToRect(l->gid[l->Get(i, j)]);
					App->render->Blit(texture, position.x, position.y, sect, l->layer_speed);
				}
			}
		}
	}
}

//Create walkability map used in pathfinding

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;

	p2List_item<Layer*>* item;
	item = data.layers.start;

	uchar* map = new uchar[data.width*data.height];
	memset(map, 1, data.width*data.height);

	for (item = data.layers.start; item != NULL; item = item->next)
	{

		Layer* layer = item->data;

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->gid[layer->Get(x, y)];
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					if (tileset->collisionBoxArray[tile_id].w != 0)
					{
						map[i] = 0;
					}
				}
			}
		}

	}


	*buffer = map;
	width = data.width;
	height = data.height;
	ret = true;

	return ret;
}

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
// Called before quitting
bool j1Map::CleanUp()
{
	//Clean all map colliders

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (col[i] != nullptr)
		{
			col[i] = nullptr;
		}
	}



	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		App->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();


	// Remove all layers
	p2List_item<Layer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		delete (item2->data->gid);
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	
	// Clean up the pugui tree
	map_file.reset();

	return true;
}

void j1Map::Unload()
{
	//Clean all map colliders

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (col[i] != nullptr)
		{
			col[i]->to_delete = true;
			col[i] = nullptr;
		}
	}



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
	p2List_item<Layer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		delete (item2->data->gid);
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();


	// Clean up the pugui tree
	map_file.reset();
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	BROFILER_CATEGORY("Load Map", Profiler::Color::Aqua);


	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	//Starting position for our player is contained in each map's information
	

	if(result == NULL)
	{
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
			ret = LoadTilesetDetails(tileset, set); //tileset general information
		}
		if (ret == true)
		{
			ret = LoadTilesetCollisions(tileset, set); //collisions related to that tileset, defined in Tiled
		}
		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set); //relate tileset information to source tile image
		}

		data.tilesets.add(set);
	}
	
	// Load layer info ----------------------------------------------
	pugi::xml_node layernode;
	for (layernode = map_file.child("map").child("layer"); layernode && ret; layernode = layernode.next_sibling("layer"))
	{
		Layer* set1 = new Layer();

		if (ret == true)
		{
			ret = LoadLayer(layernode, set1);
		}
		data.layers.add(set1);
	}

	//Check if everything went as expected and LOG the results

	if(ret == true)
	{

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			item = item->next;
		}
		
		p2List_item<Layer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			Layer* l = item_layer->data;
			item_layer = item_layer->next;
		}
	}


	p2List_item<Layer*>* item_layer = data.layers.start;
	int collider_iterator = 0;

	while (item_layer != NULL)
	{
		Layer* l = item_layer->data;
		item_layer = item_layer->next; //iterate all layers

		for (int i = 0; i < l->width; i++) //iterate all rows in layer
		{
			for (int j = 0; j < l->height; j++) //iterate all columns in layer
			{
				int tile_id = l->gid[l->Get(i, j)];
				if (tile_id > 0)
				{

					iPoint position = PosConverter(i, j);
					
					SDL_Rect auxRect;
					auxRect.x = data.tilesets.start->data->collisionBoxArray[tile_id].x + position.x;
					auxRect.y = data.tilesets.start->data->collisionBoxArray[tile_id].y + position.y;
					auxRect.h = data.tilesets.start->data->collisionBoxArray[tile_id].h;
					auxRect.w = data.tilesets.start->data->collisionBoxArray[tile_id].w;

					//Check if tile has a collider attached. If it does, added to our array of colliders
					if ((auxRect.h != 0) && (auxRect.w != 0) && data.tilesets.start->data->type[tile_id] == 0) {
						col[collider_iterator] = App->colliders->AddCollider(auxRect, COLLIDER_WALL, this);
						collider_iterator++;
					}
					else if ((auxRect.h != 0) && (auxRect.w != 0) && data.tilesets.start->data->type[tile_id] == 1)
					{
						col[collider_iterator] = App->colliders->AddCollider(auxRect, COLLIDER_TRANSPASSABLE_WALL, this);
						collider_iterator++;
					}
					else if ((auxRect.h != 0) && (auxRect.w != 0) && data.tilesets.start->data->type[tile_id] == 2)
					{
						col[collider_iterator] = App->colliders->AddCollider(auxRect, COLLIDER_END_LEVEL, App->scene);
						collider_iterator++;
					}
				}
			}
		}
	}
	map_loaded = ret;
	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	BROFILER_CATEGORY("Load map properties", Profiler::Color::BurlyWood)
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
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
	set->tile_count = tileset_node.attribute("tilecount").as_int();
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

bool j1Map::LoadTilesetCollisions(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node tilebox;
	
	for (tilebox = tileset_node.child("tile"); tilebox && ret; tilebox = tilebox.next_sibling()) {
		
		SDL_Rect this_tile_box;
		this_tile_box.x = tilebox.child("objectgroup").child("object").attribute("x").as_int();
		this_tile_box.y = tilebox.child("objectgroup").child("object").attribute("y").as_int();
		this_tile_box.w = tilebox.child("objectgroup").child("object").attribute("width").as_int();
		this_tile_box.h = tilebox.child("objectgroup").child("object").attribute("height").as_int();


		int id = tilebox.attribute("id").as_int();

		set->type[id + 1] = tilebox.child("properties").child("property").attribute("value").as_int(); //only works if tiles have only 1 property
		set->collisionBoxArray[id+1] = this_tile_box;

		// "+1" is not a magic number: array size starts at 0 but tile id starts at 1
	}
	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
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

bool j1Map::LoadLayer(pugi::xml_node& layer, Layer* set)
{
	bool ret = true;
	set->name.create(layer.attribute("name").as_string());
	set->width = layer.attribute("width").as_int();
	set->height = layer.attribute("height").as_int();
	set->gid = new uint[set->width * set->height];
	set->layer_speed = layer.child("properties").child("property").attribute("value").as_float();

	memset(set->gid, 0, sizeof(uint)*(set->height * set->width));

	pugi::xml_node tilesgid;
	int i = 0;
	for (tilesgid = layer.child("data").child("tile"); tilesgid && ret; tilesgid = tilesgid.next_sibling("tile"), i++)
	{
		set->gid[i] = tilesgid.attribute("gid").as_uint();
	}
	
	if (LoadEntitiesFromTiled(layer, set)) {
	};
	return ret;
}

bool j1Map::LoadEntitiesFromTiled(pugi::xml_node& layer, Layer* set) {

	if (set->name != ("entities")) {

		return false;

	}
	bool ret = true;
	
	//36 is gid for coin
	//40 is gid for player
	//42 is the gid that coresponds to flying enemies
	//43 is for walking enemy
	if (App->scene->load_from_save == false)
	{
		for (int i = 0; i < set->width; i++)
		{
			for (int j = 0; j < set->height; j++)
			{
				int tile_id = set->gid[set->Get(i, j)];
				if (tile_id != 0)
				{

					iPoint spawn = PosConverter(i, j);

					switch (tile_id - 1) //compensating for TILED nomenclature, not a magic number
					{
					case 36:
						App->entityManager->CreateEntity(Entity::EntityTypes::COIN, spawn.x, spawn.y);
						break;

					case 40:
						App->entityManager->CreateEntity(Entity::EntityTypes::PLAYER, spawn.x, spawn.y);
						break;

					case 42:
						App->entityManager->CreateEntity(Entity::EntityTypes::FLY_ENEMY, spawn.x, spawn.y);
						break;

					case 43:
						App->entityManager->CreateEntity(Entity::EntityTypes::WALK_ENEMY, spawn.x, spawn.y);
						break;

					default:

						break;
					}
				}
			}
		}
	}
	return ret;
}

SDL_Rect TileSet::TileToRect(uint tileid) 
{
	uint id= tileid - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (id % num_tiles_width));	
	rect.y = margin + ((rect.h + spacing) * (id / num_tiles_width));	
	return rect;
}

iPoint j1Map::PosConverter(int x, int y){

	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);
	
	ret.x = x / data.tile_width;
	ret.y = y / data.tile_height;

	return ret;
}