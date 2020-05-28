#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Window.h"
#include "Collision.h"
#include "FoWManager.h"
#include "EntityManager.h"
#include "Building.h" //necessary to read buildings from tiled
#include "Quest.h"    //idem
#include "QuestManager.h"
#include "Minimap.h"
#include <math.h>
#include "Brofiler/Brofiler/Brofiler.h"


ModuleMap::ModuleMap() : Module(), mapLoaded(false)
{
	name.create("map");
}


// Destructor
ModuleMap::~ModuleMap()
{
}


// Called before render is available
bool ModuleMap::Awake(pugi::xml_node& config)
{
	bool ret = true;

	folder.create(config.child("folder").child_value());
	mapName.create(config.child("load").attribute("mapname").as_string());
	return ret;
}


void ModuleMap::Draw()
{
	BROFILER_CATEGORY("DRAW", Profiler::Color::Aquamarine);

	if (mapLoaded == false)
		return;


	int f = 0;


	while (f < data.layers.size())
	{
		if (data.layers[f]->name != P2SString("Collision") && data.layers[f]->name != P2SString("BuildingGeneration"))
		{

			for (int i = 0; i < data.layers[f]->height; i++)//number of rows
			{

				for (int j = 0; j < data.layers[f]->width; j++)//number of columns
				{
					int id = data.layers[f]->gid[Get(j, i, data.layers[f])];

					float worldX;
					float worldY;
					MapToWorldCoords(j, i, data, worldX, worldY);

					//whith camera culling

					if (InsideCamera(worldX, worldY) == true)
					{
						if (id > 0)
						{
							FoWDataStruct* fogData = app->fowManager->GetFoWTileState({ j,i });
							if (fogData != nullptr && app->fowManager->foWMapVisible)
							{
								if (fogData->tileShroudBits != fow_ALL)
								{
									app->render->Blit(GetTilesetFromTileId(id)->texture, worldX, worldY, &RectFromTileId(id, GetTilesetFromTileId(id)));
								}
							}
							else
							{
								app->render->Blit(GetTilesetFromTileId(id)->texture, worldX, worldY, &RectFromTileId(id, GetTilesetFromTileId(id)));
							}

						}
					}
				}
			}
		}
		f++;
	}
}


void ModuleMap::DrawMinimap()
{
	BROFILER_CATEGORY("DRAW Minimap", Profiler::Color::DarkGoldenRod);

	if (mapLoaded == false)
		return;

	int f = 0;

	float scale = app->minimap->minimapScaleRelation;
	float halfWidth = app->minimap->minimapWidth * 0.5f;
	while (f < data.layers.size())
	{
		if (data.layers[f]->name != P2SString("Collision")) {

			for (int i = 0; i < data.layers[f]->height; i++)//number of rows
			{

				for (int j = 0; j < data.layers[f]->width; j++)//number of columns
				{
					int id = data.layers[f]->gid[Get(j, i, data.layers[f])];


					if (id > 0)
					{
						float worldX;
						float worldY;
						MapToWorldCoords(j, i, data, worldX, worldY);

						worldX += app->render->currentCamX;
						worldY += app->render->currentCamY;
						FoWDataStruct* fogData = app->fowManager->GetFoWTileState({ j,i });
						if (fogData != nullptr && app->fowManager->foWMapVisible)
						{
							if (fogData->tileShroudBits != fow_ALL)
							{
								app->render->MinimapBlit(GetTilesetFromTileId(id)->texture, worldX + halfWidth, worldY, &RectFromTileId(id, GetTilesetFromTileId(id)), scale);
							}
						}
						else
						{
							app->render->MinimapBlit(GetTilesetFromTileId(id)->texture, worldX + halfWidth, worldY, &RectFromTileId(id, GetTilesetFromTileId(id)), scale);
						}

					}

				}
			}
		}
		f++;
	}
}



TileSet* ModuleMap::GetTilesetFromTileId(int id) const
{
	TileSet* set = nullptr;

	int numTilesets = data.tilesets.size();

	for (int i = 0; i < numTilesets; i++)
	{

		if (id < data.tilesets[i]->firstGid)
		{
			set = data.tilesets[i - 1];
			break;
		}


		set = data.tilesets[i];
	}


	return set;
}


// Called before quitting
bool ModuleMap::CleanUp()
{
	// Remove all tilesets
	int numTilesets = data.tilesets.size();


	for (int i = 0; i < numTilesets; i++)
	{
		app->tex->UnLoad(data.tilesets[i]->texture);
		RELEASE(data.tilesets[i]);
	}


	data.tilesets.clear();

	// Remove all layers
	int numLayers = data.layers.size();


	for (int i = 0; i < numLayers; i++)
	{
		int numProperties = data.layers[i]->layerPropVector.size();

		for (int j = 0; j < numProperties; j++)
		{
			RELEASE(data.layers[i]->layerPropVector[j]);
		}
		data.layers[i]->layerPropVector.clear();

		RELEASE(data.layers[i]);
	}


	data.layers.clear();

	//Remove all the objectlayers
	int numObjectGroups = data.objGroups.size();


	for (int i = 0; i < numObjectGroups; i++)
	{
		//remove all the objects
		int numObjects = data.objGroups[i]->objectVector.size();

		for (int j = 0; j < numObjects; j++)
		{
			RELEASE(data.objGroups[i]->objectVector[j]);
		}
		data.objGroups[i]->objectVector.clear();

		//remove all the properties
		int numProperties = data.objGroups[i]->propVector.size();

		for (int j = 0; j < numProperties; j++)
		{
			RELEASE(data.objGroups[i]->propVector[j]);
		}
		data.objGroups[i]->propVector.clear();

		RELEASE(data.objGroups[i]);
	}


	data.objGroups.clear();

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}


// Load new map
bool ModuleMap::LoadNew(const char* file_name)
{
	bool ret = true;
	P2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());


	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}


	// Load general info ----------------------------------------------
	if (ret == true)
		ret = LoadMap(tmp.GetString(), file_name);


	// Load music ----------------------------------------------------------
	if (ret == true)
		data.musicPath = mapFile.child("map").child("properties").child("property").attribute("value").as_string();


	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;


	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();


		if (ret == true)
			ret = LoadTilesetDetails(tileset, set);


		if (ret == true)
			ret = LoadTilesetImage(tileset, set);


		data.tilesets.push_back(set);
	}


	// Load layer info ----------------------------------------------
	pugi::xml_node currentLayer;


	for (currentLayer = mapFile.child("map").child("layer"); currentLayer && ret; currentLayer = currentLayer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();


		if (ret == true)
			ret = LoadLayer(currentLayer, set);


		data.layers.push_back(set);
	}


	//Load object info-----------------------------------------------
	for (pugi::xml_node currentobjgroup = mapFile.child("map").child("objectgroup"); currentobjgroup && ret; currentobjgroup = currentobjgroup.next_sibling("objectgroup"))
	{
		ObjectGroup* set = new ObjectGroup();


		if (ret == true)
			ret = LoadObjGroup(currentobjgroup, set);


		data.objGroups.push_back(set);
	}


	mapLoaded = ret;

	// Clean up the pugui tree
	mapFile.reset();
	return ret;
}


// Load map general properties
bool ModuleMap::LoadMap(P2SString path, P2SString name)
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");


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
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		P2SString bgColor(map.attribute("backgroundcolor").as_string());

		data.backgroundColor.r = 0;
		data.backgroundColor.g = 0;
		data.backgroundColor.b = 0;
		data.backgroundColor.a = 0;


		if (bgColor.Length() > 0)
		{
			P2SString red, green, blue;
			bgColor.SubString(1, 2, red);
			bgColor.SubString(3, 4, green);
			bgColor.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.b = v;
		}


		P2SString orientation(map.attribute("orientation").as_string());


		if (orientation == P2SString("orthogonal"))
			data.type = MAP_TYPES::MAP_TYPE_ORTHOGONAL;
		else if (orientation == P2SString("isometric"))
			data.type = MAP_TYPES::MAP_TYPE_ISOMETRIC;
		else if (orientation == P2SString("staggered"))
			data.type = MAP_TYPES::MAP_TYPE_STAGGERED;
		else
			data.type = MAP_TYPES::MAP_TYPE_UNKNOWN;


	}


	return ret;
}


bool ModuleMap::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstGid = tileset_node.attribute("firstgid").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->columns = tileset_node.attribute("columns").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");


	if (offset != NULL)
	{
		set->offsetX = offset.attribute("x").as_int();
		set->offsetY = offset.attribute("y").as_int();
	}
	else
	{
		set->offsetX = 0;
		set->offsetY = 0;
	}


	return ret;
}


bool ModuleMap::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
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
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->texWidth = image.attribute("width").as_int();


		if (set->texWidth <= 0)
			set->texWidth = w;

		set->texHeight = image.attribute("height").as_int();

		if (set->texHeight <= 0)
			set->texHeight = h;

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
	}


	return ret;
}


//Loads a single layer
bool ModuleMap::LoadLayer(pugi::xml_node& layer_node, MapLayer* layer)
{
	bool ret = true;
	layer->name.create(layer_node.attribute("name").as_string());
	layer->width = layer_node.attribute("width").as_uint();
	layer->height = layer_node.attribute("height").as_uint();
	layer->gid = new uint[layer->width * layer->height];
	memset(layer->gid, 0, layer->width * layer->height * sizeof(uint));


	//Load properties--------------------------------------------
	for (pugi::xml_node currentProp = layer_node.child("properties").child("property"); currentProp; currentProp = currentProp.next_sibling("property"))
	{
		Properties* set = new Properties;
		set->name = currentProp.attribute("name").as_string();
		set->value = currentProp.attribute("value").as_bool();
		layer->layerPropVector.push_back(set);
	}


	pugi::xml_node* gidIterator = &layer_node.child("data").child("tile");


	for (int i = 0; i < layer->width * layer->height; i++)
	{
		layer->gid[i] = gidIterator->attribute("gid").as_uint();
		if (gidIterator->attribute("gid").as_uint() == NULL) {
			layer->gid[i] = 0;
		}

		gidIterator = &gidIterator->next_sibling("tile");
	}

	/*if (layer->name == P2SString("Collision")) 
	{

		for (int i = 0; i < layer->width * layer->height; i++)
		{

			if (layer->gid[i] > 0) {

				SDL_Rect colliderRectAux = RectFromTileId(layer->gid[i], GetTilesetFromTileId(layer->gid[i]));

				int tilePosAux_x, tilePosAux_y;
				tilePosAux_x = (i % layer->width);
				tilePosAux_y = ((i - (i % layer->width)) / layer->width);

				colliderRectAux.x = tilePosAux_x * colliderRectAux.w / 2 - tilePosAux_y * colliderRectAux.w / 2;
				colliderRectAux.y = tilePosAux_x * colliderRectAux.h / 2 + tilePosAux_y * colliderRectAux.h / 2;
				colliderRectAux.w /= 2;

				app->coll->AddCollider(colliderRectAux, COLLIDER_WALL);

			}
		}
	}*/
	if (layer->name == P2SString("BuildingGeneration")) 
	{

		for (int i = 0; i < layer->width * layer->height; i++)
		{

			if (layer->gid[i] > 0) 
			{

				SDL_Rect colliderRectAux = RectFromTileId(layer->gid[i], GetTilesetFromTileId(layer->gid[i]));

				int tilePosAux_x, tilePosAux_y;
				tilePosAux_x = (i % layer->width);
				tilePosAux_y = ((i - (i % layer->width)) / layer->width);

				colliderRectAux.x = tilePosAux_x * colliderRectAux.w / 2 - tilePosAux_y * colliderRectAux.w / 2;
				colliderRectAux.y = tilePosAux_x * colliderRectAux.h / 2 + tilePosAux_y * colliderRectAux.h / 2;
				colliderRectAux.w /= 2;

				Building* bld = nullptr; Entity* bldgToBe = nullptr; //we cant do it inside the switch case
				Quest* qst = nullptr;
				Enemy* enemy = nullptr;
				Spawner* spawner = nullptr;

				switch (layer->gid[i])
				{
					//250 = id of the first tile of the tileset that codifies for building generation
				
				
				//BUILDINGS----------------
				case 250 + 0:
				{
					Base* base = (Base*)app->entityManager->AddEntity(ENTITY_TYPE::BLDG_BASE, colliderRectAux.x, colliderRectAux.y, ENTITY_ALIGNEMENT::ENEMY);
				}
					break;

				case 250 + 1:
					app->entityManager->AddEntity(ENTITY_TYPE::BLDG_BASE, colliderRectAux.x, colliderRectAux.y, ENTITY_ALIGNEMENT::PLAYER);
					break;

				case 250 + 2:
					app->entityManager->AddEntity(ENTITY_TYPE::QUEST, colliderRectAux.x, colliderRectAux.y);
					break;

				case 250 + 3:
					app->entityManager->AddEntity(ENTITY_TYPE::BLDG_UPGRADE_CENTER, colliderRectAux.x, colliderRectAux.y, ENTITY_ALIGNEMENT::ENEMY);
					break;

				case 250 + 4:
					app->entityManager->AddEntity(ENTITY_TYPE::BLDG_UPGRADE_CENTER, colliderRectAux.x, colliderRectAux.y, ENTITY_ALIGNEMENT::PLAYER);
					break;

				case 250 + 5:
					app->entityManager->AddEntity(ENTITY_TYPE::BLDG_UPGRADE_CENTER, colliderRectAux.x, colliderRectAux.y);
					break;

				case 250 + 6:
					bldgToBe = app->entityManager->AddDecorativeBuilding(BUILDING_DECOR::ST_01, colliderRectAux.x, colliderRectAux.y);
					break;

				case 250 + 7:
					bldgToBe = app->entityManager->AddDecorativeBuilding(BUILDING_DECOR::ST_02, colliderRectAux.x, colliderRectAux.y);
					break;

				case 250 + 8:
					bldgToBe = app->entityManager->AddDecorativeBuilding(BUILDING_DECOR::ST_03, colliderRectAux.x, colliderRectAux.y);
					
					break;

					
				//QUESTS---------------------------------------

				case 250 + 9:
					qst = (Quest*)app->entityManager->AddEntity(ENTITY_TYPE::QUEST, colliderRectAux.x, colliderRectAux.y);
					qst->SetId(1);
					app->questManager->AddQuest(qst);
					break;

				case 250 + 10:
					qst = (Quest*)app->entityManager->AddEntity(ENTITY_TYPE::QUEST, colliderRectAux.x, colliderRectAux.y);
					qst->SetId(2);
					app->questManager->AddQuest(qst);
					break;

				case 250 + 11:
					qst = (Quest*)app->entityManager->AddEntity(ENTITY_TYPE::QUEST, colliderRectAux.x, colliderRectAux.y);
					qst->SetId(3);
					app->questManager->AddQuest(qst);
					break;

				case 250 + 12:
					qst = (Quest*)app->entityManager->AddEntity(ENTITY_TYPE::QUEST, colliderRectAux.x, colliderRectAux.y);
					qst->SetId(4);
					app->questManager->AddQuest(qst);
					break;

				case 250 + 13:
					qst = (Quest*)app->entityManager->AddEntity(ENTITY_TYPE::QUEST, colliderRectAux.x, colliderRectAux.y);
					qst->SetId(5);
					app->questManager->AddQuest(qst);
					break;

				case 250 + 14:
					qst = (Quest*)app->entityManager->AddEntity(ENTITY_TYPE::QUEST, colliderRectAux.x, colliderRectAux.y);
					qst->SetId(0);
					app->questManager->AddQuest(qst);
					break;

				case 250 + 15:
					qst = (Quest*)app->entityManager->AddEntity(ENTITY_TYPE::QUEST, colliderRectAux.x, colliderRectAux.y);
					qst->SetId(6);
					app->questManager->AddQuest(qst);
					break;

				case 250 + 16:
					spawner = (Spawner*)app->entityManager->AddEntity(ENTITY_TYPE::SPAWNER, colliderRectAux.x, colliderRectAux.y);
					break;

				case 250 + 17:
					enemy = (Enemy*)app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, colliderRectAux.x, colliderRectAux.y);
					break;

				case 250 + 18:
					enemy = (Enemy*)app->entityManager->AddEntity(ENTITY_TYPE::ENEMY_GIGA, colliderRectAux.x, colliderRectAux.y);
					break;

				case 250 + 19:
					enemy = (Enemy*)app->entityManager->AddEntity(ENTITY_TYPE::ENEMY_NIGHT, colliderRectAux.x, colliderRectAux.y);
					break;

				case 250 + 20:
					enemy = (Enemy*)app->entityManager->AddEntity(ENTITY_TYPE::ENEMY_RANGED, colliderRectAux.x, colliderRectAux.y);
					break;

				default:
					break;
				}
			}
		}
	}

	return ret;
}


bool ModuleMap::LoadObjGroup(pugi::xml_node& objgroupnode, ObjectGroup* group)
{
	bool ret = true;
	group->name.create(objgroupnode.attribute("name").as_string());
	group->id = objgroupnode.attribute("id").as_uint();


	//Load properties--------------------------------------------
	for (pugi::xml_node currentProp = objgroupnode.child("properties").child("property"); currentProp; currentProp = currentProp.next_sibling("property"))
	{
		Properties* set = new Properties;
		set->name = currentProp.attribute("name").as_string();
		set->value = currentProp.attribute("value").as_bool();
		group->propVector.push_back(set);
	}


	//Load objects-----------------------------------------------
	for (pugi::xml_node currentObj = objgroupnode.child("object"); currentObj; currentObj = currentObj.next_sibling("object"))
	{
		Object* set = new Object;
		set->id = currentObj.attribute("id").as_uint();
		set->type = currentObj.attribute("type").as_int();
		set->boundingBox.x = currentObj.attribute("x").as_int();
		set->boundingBox.y = currentObj.attribute("y").as_int();
		set->boundingBox.h = currentObj.attribute("height").as_int();
		set->boundingBox.w = currentObj.attribute("width").as_int();

		group->objectVector.push_back(set);
	}


	return ret;
}


//gets the position in a 1 row vector
//gets the position in a 1 row vector
//X horizontal, Y vertical, data.layers.(currentLayer)
inline uint ModuleMap::Get(int x, int y, MapLayer* currentlayer) const
{
	return (y * currentlayer->width) + x;
}


//converts an id from a 1 row vector(Tileset position) into a Rectangle
SDL_Rect ModuleMap::RectFromTileId(uint tile_id, TileSet* currenttileset)
{
	int firstGid = currenttileset->firstGid;
	int margin = currenttileset->margin;
	int spacing = currenttileset->spacing;
	int textureTileWidth = currenttileset->tileWidth;//has to be improved in case the tile from the image is not the same size as the output tile
	int textureTileHeight = currenttileset->tileHeight;//to do that calculate tile width and height takin into account the image w/h and spacing + margin
	int columns = currenttileset->columns;

	SDL_Rect rect;
	rect.h = currenttileset->tileHeight;
	rect.w = currenttileset->tileWidth;

	int tileX = (tile_id - firstGid) % columns;//number of tiles full in a row
	int tileY = (tile_id - firstGid) / columns;//number of rows full
	rect.x = margin + (tileX * textureTileWidth) + tileX * spacing;
	rect.y = margin + (tileY * textureTileHeight) + tileY * spacing;

	return rect;
}


//isometric map to world
void ModuleMap::MapToWorldCoords(int posX, int posY, MapData& dat, float& outX, float& outY)
{
	outX = (posX - posY) * dat.tileWidth * 0.5f;
	outY = (posX + posY) * dat.tileHeight * 0.5f;
}


//isometric world to map
void ModuleMap::WorldToMapCoords(int x, int y, MapData& dat, int& outX, int& outY) const
{
	outX = int((x / (dat.tileWidth * 0.5f) + y / (dat.tileHeight * 0.5f)) * 0.5f);
	outY = int((y / (dat.tileHeight * 0.5f) - (x / (dat.tileWidth * 0.5f))) * 0.5f);
}


//Returns x,y coordinates in the world
iMPoint ModuleMap::MapToWorld(int x, int y) const
{
	iMPoint ret;

	ret.x = (x - y) * (data.tileWidth * 0.5f);
	ret.y = (x + y) * (data.tileHeight * 0.5f);

	return ret;
}

//Returns x,y coordinates in the Map
iMPoint ModuleMap::WorldToMap(float x, float y) const
{
	fMPoint ret(0, 0);

	float halWidth = data.tileWidth * 0.5f;
	float halfHeight = data.tileHeight * 0.5f;

	float x2 = (x), y2 = (y);

	ret.x = ((x2 / halWidth + y2 / halfHeight) * 0.5f);
	ret.y = ((y2 / halfHeight - (x2 / halWidth)) * 0.5f);

	iMPoint ret2 = {(int)ret.x, (int)ret.y};

	return ret2;
}


bool ModuleMap::ReloadMap(P2SString newmap)
{
	CleanUp();//clears the map

	bool ret = true;

	LoadNew(newmap.GetString());

	return ret;
}


TileSet::~TileSet()
{
	if (this->texture != NULL)
		app->tex->UnLoad(this->texture);

	this->name.Clear();
}


MapLayer::~MapLayer()
{
	if (this->gid != nullptr)
		delete this->gid;
}


bool ModuleMap::Load(pugi::xml_node& ldata)
{
	P2SString newMapname = ldata.attribute("name").as_string("first_level.tmx");//loads the map name from the saves doc, if gets errror, replaces it with "first_level.tmx"

	if (newMapname != data.name)//if the map that you request to load isn't the same as the one you are currently in, load it, else do nothing 
		ReloadMap(newMapname);

	return true;

}


bool ModuleMap::Save(pugi::xml_node& ldata) const
{
	ldata.append_attribute("name") = data.name.GetString();
	return true;
}

bool ModuleMap::InsideCamera(float& posX, float& posY) const {

	int camW;
	int camH;
	app->render->GetCameraMeasures(camW, camH);
	float scale = app->win->GetScale();
	float inverseScale = 1 / scale;
	float up_left_cam_cornerX = -app->render->currentCamX;
	float up_left_cam_cornerY = -app->render->currentCamY;
	float down_right_cam_cornerX = up_left_cam_cornerX + camW;
	float down_right_cam_cornerY = up_left_cam_cornerY + camH;

	if ((posX > (up_left_cam_cornerX *inverseScale) - data.tileWidth && posX < down_right_cam_cornerX * inverseScale) &&
		(posY > (up_left_cam_cornerY * inverseScale) - data.tileWidth && posY < down_right_cam_cornerY * inverseScale)) {
		return true;
	}
}

bool ModuleMap::EntityInsideCamera(float& posX, float& posY, float& w, float& h) const {

	int camW;
	int camH;
	app->render->GetCameraMeasures(camW, camH);
	float scale = app->win->GetScale();
	float inverseScale = 1 / scale;

	float up_left_cam_cornerX = -app->render->currentCamX;
	float up_left_cam_cornerY = -app->render->currentCamY;
	float down_right_cam_cornerX = up_left_cam_cornerX + camW;
	float down_right_cam_cornerY = up_left_cam_cornerY + camH;

	if ((posX + (2 * w) > up_left_cam_cornerX * inverseScale && posX - w < down_right_cam_cornerX * inverseScale) &&
		((posY + 2 * h > up_left_cam_cornerY * inverseScale) && posY - h < (down_right_cam_cornerY + data.tileHeight) * inverseScale)) {
		return true;
	}
}

bool ModuleMap::CreateWalkabilityMap(int& width, int& height, uchar** buffer)
{
	bool ret = false;
	std::vector<MapLayer*>::iterator layIt;

	for (layIt = data.layers.begin(); layIt != data.layers.end(); ++layIt)
	{
		MapLayer* layer = *layIt;

		//Search for Navigation Propierty
		for (int i = 0; i < layer->layerPropVector.size(); i++)
		{
			Properties* currProp = layer->layerPropVector[i];
			if (currProp->name != P2SString("Navigation"))
			{
				continue;
			}
			else if (currProp->value == 1)
			{
				ret = true;
			}
		}
		if (!ret) continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tile_id = layer->gid[Get(x, y, layer)];

				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = 0;

				}
				else
					map[i] = 1;

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


Properties::Properties() : name(), value(-1)
{}


MapLayer::MapLayer() : name(), width(0u), height(0u), gid(nullptr)
{}


Object::Object() : boundingBox({ NULL, NULL, NULL, NULL }), type (-1), id(-1)
{}


ObjectGroup::ObjectGroup() : id(-1), name()
{}


TileSet::TileSet() : name(), texture(nullptr), firstGid(-1), margin(-1), spacing(-1), tileWidth(-1), tileHeight(-1), texWidth(-1), texHeight(-1), numTilesWidth(-1), 
	numTilesHeight(-1), offsetX(-1), offsetY(-1), columns(-1)
{}


MapData::MapData() : width(-1), height(-1), tileWidth(-1), tileHeight(-1), backgroundColor({0, 0, 0, 0}), type(MAP_TYPES::MAP_TYPE_UNKNOWN), name(), path(), musicPath()
{}

