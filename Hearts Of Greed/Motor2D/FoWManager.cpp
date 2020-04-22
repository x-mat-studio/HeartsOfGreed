#include "FoWManager.h"
#include "App.h"
#include "Textures.h"
#include "Map.h"
#include "Render.h"
#include "Input.h"
#include "Brofiler/Brofiler/Brofiler.h"

ModuleFoWManager::ModuleFoWManager() :
	foWMapVisible(true),
	fowMap(nullptr),
	smoothFoWtexture(nullptr),
	debugFoWtexture(nullptr),
	debugMode(false),
	foWMapNeedsRefresh(false)
{}


ModuleFoWManager::~ModuleFoWManager()
{}


bool ModuleFoWManager::Awake(pugi::xml_node&)
{
	bool ret = true;

	return ret;
}


bool ModuleFoWManager::Start()
{
	bool ret = true;

	smoothFoWtexture = app->tex->Load("spritesheets/VFX/fogTiles.png");
	debugFoWtexture = app->tex->Load("spritesheets/VFX/fogTilesDebug.png");

	if (smoothFoWtexture == nullptr || debugFoWtexture == nullptr);
	ret = false;

	foWMapVisible = true;

	//---------Initialize the map being used to translate bits to texture ID---------//
	//Straight-forward cases
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_ALL, 0));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_NNN, 1));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_WWW, 2));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_EEE, 3));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_SSS, 4));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CNW, 5));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CSE, 6));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CNE, 7));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CSW, 8));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JNE, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JSW, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JNW, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JSE, 12));

	//more complicated cases (combinations)
	//diagonals
	bitToTextureTable.insert(std::pair<unsigned short, int>(20, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(80, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(17, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(272, 12));
	bitToTextureTable.insert(std::pair<unsigned short, int>(273, 13));
	bitToTextureTable.insert(std::pair<unsigned short, int>(84, 14));
	//lines
	bitToTextureTable.insert(std::pair<unsigned short, int>(23, 1));
	bitToTextureTable.insert(std::pair<unsigned short, int>(308, 3));
	bitToTextureTable.insert(std::pair<unsigned short, int>(89, 2));
	bitToTextureTable.insert(std::pair<unsigned short, int>(464, 4));
	//joints
	bitToTextureTable.insert(std::pair<unsigned short, int>(6, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(36, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(72, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(192, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(3, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(9, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(384, 12));
	bitToTextureTable.insert(std::pair<unsigned short, int>(288, 12));
	//corners
	bitToTextureTable.insert(std::pair<unsigned short, int>(4, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(64, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(1, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(256, 12));
	//------------------------end of map initialization------------------------//

	fowUpdateTimer = 0.0f;

	return ret;
}


bool ModuleFoWManager::PreUpdate(float dt)
{
	bool ret = true;

	//deletes all the entities that request to do so
	for (int i = 0; i < fowEntities.size(); i++)
	{
		if (fowEntities[i]->deleteEntity)
		{
			delete fowEntities[i];
			fowEntities[i] = nullptr;
			fowEntities.erase(fowEntities.begin() + i);
			if (fowMap != nullptr)
			{
				UpdateFoWMap();
			}
			i--;
		}

	}

	//TODO FOR OSCAR: THOSE DEBUG KEYS ARE NOT LONGER WORKING WITH DIRECT INPUT
	//debug input handling
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_STATE::KEY_DOWN)
	{
		ResetFoWMap();
		MapNeedsUpdate();
	}
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_STATE::KEY_DOWN)
	{
		debugMode = !debugMode;
		MapNeedsUpdate();
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_STATE::KEY_DOWN)
	{
		foWMapVisible = !foWMapVisible;
		MapNeedsUpdate();
	}

	return ret;
}


bool ModuleFoWManager::Update(float dt)
{
	BROFILER_CATEGORY("FOW Update", Profiler::Color::AliceBlue);
	bool ret = true;
	fowUpdateTimer += dt;

	//We update the fowMap only when its needed
	if (fowUpdateTimer >= 0.5f)
	{
		fowUpdateTimer = 0.0f;
		if (foWMapNeedsRefresh)
		{
			UpdateFoWMap();
			foWMapNeedsRefresh = false;
		}
	}
	return ret;
}


bool ModuleFoWManager::PostUpdate(float dt)
{
	bool ret = true;
	BROFILER_CATEGORY("FOW PostUpdate", Profiler::Color::BlanchedAlmond);
	if (foWMapVisible)
	{
		DrawFoWMap();
	}
	return ret;
}


bool ModuleFoWManager::CleanUp()
{
	bool ret = true;
	DeleteFoWMap();

	DeleteAllFoWEntites();

	fowEntities.clear();

	if (debugFoWtexture != nullptr)
	{
		app->tex->UnLoad(debugFoWtexture);
		debugFoWtexture = nullptr;
	}
	if (smoothFoWtexture != nullptr)
	{
		app->tex->UnLoad(smoothFoWtexture);
		smoothFoWtexture = nullptr;
	}

	return ret;
}

void ModuleFoWManager::DeleteAllFoWEntites()
{
	for (int i = 0; i < fowEntities.size(); i++)
	{
		RELEASE(fowEntities[i]);
		fowEntities[i] = nullptr;
	}

	fowEntities.clear();
}

void ModuleFoWManager::ResetFoWMap()
{
	if (fowMap != nullptr)
	{
		memset(fowMap, NULL, width * height);

		for (int i = 0; i < width * height; i++)
		{
			fowMap[i].tileShroudBits = fow_ALL;
			fowMap[i].tileFogBits = fow_ALL;
		}
	}
}


FoWDataStruct* ModuleFoWManager::GetFoWTileState(iMPoint mapPos)const
{
	FoWDataStruct* ret = nullptr;
	if (CheckFoWTileBoundaries(mapPos) && fowMap != nullptr)
	{
		ret = &fowMap[(mapPos.y * width) + mapPos.x];
	}

	return ret;
}


bool ModuleFoWManager::CheckFoWTileBoundaries(iMPoint mapPos)const
{
	bool ret = false;
	if (mapPos.x >= 0 && mapPos.x < width && mapPos.y >= 0 && mapPos.y < height)
		ret = true;

	return ret;
}

void ModuleFoWManager::CreateFoWMap(uint w, uint h)
{
	width = w;
	height = h;

	DeleteFoWMap();
	fowMap = new FoWDataStruct[width * height];
	ResetFoWMap();

	MapNeedsUpdate();
}


void ModuleFoWManager::DeleteFoWMap()
{
	if (fowMap != nullptr)
	{
		RELEASE_ARRAY(fowMap);
		fowMap = nullptr;
	}
}


void ModuleFoWManager::UpdateFoWMap()
{
	if (fowMap != nullptr)
	{
		for (int i = 0; i < width * height; i++)
		{
			fowMap[i].tileFogBits = fow_ALL;
		}

		for (int i = 0; i < fowEntities.size(); i++)
		{
			fowEntities[i]->Update();
		}



		if (!debugMode && foWMapVisible)
		{
			for (int i = 0; i < fowEntities.size(); i++)
			{
				if (CheckTileVisibility(fowEntities[i]->GetPos()))
				{
					fowEntities[i]->isVisible = true;
				}
				else fowEntities[i]->isVisible = false;

			}
		}
		else
		{
			for (int i = 0; i < fowEntities.size(); i++)
			{
				fowEntities[i]->isVisible = true;
			}
		}


	}
}

void ModuleFoWManager::DrawFoWMap()
{
	BROFILER_CATEGORY("FOW DRAW", Profiler::Color::DarkGoldenRod);

	SDL_Texture* displayFogTexture = nullptr;
	if (debugMode)
	{
		displayFogTexture = debugFoWtexture;
	}
	else displayFogTexture = smoothFoWtexture;


	float halfTileHeight = app->map->data.tileHeight * 0.5f;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{

			fMPoint worldPos;
			app->map->MapToWorldCoords(x, y, app->map->data, worldPos.x, worldPos.y);
			if (app->map->InsideCamera(worldPos.x, worldPos.y) == true)
			{
				FoWDataStruct* tileInfo = GetFoWTileState({ x, y });
				int fogId = -1;
				int shroudId = -1;

				if (tileInfo != nullptr)
				{

					if (bitToTextureTable.find(tileInfo->tileFogBits) != bitToTextureTable.end())
					{
						fogId = bitToTextureTable[tileInfo->tileFogBits];
					}

					if (bitToTextureTable.find(tileInfo->tileShroudBits) != bitToTextureTable.end())
					{
						shroudId = bitToTextureTable[tileInfo->tileShroudBits];
					}



					//draw fog
					if (fogId != -1)
					{

						SDL_SetTextureAlphaMod(displayFogTexture, 128);//set the alpha of the texture to half to reproduce fog
						SDL_Rect r = { fogId * 64,0,64,64 }; //this rect crops the desired fog Id texture from the fogTiles spritesheet
						app->render->Blit(displayFogTexture, worldPos.x, worldPos.y - halfTileHeight, &r);
					}
					if (shroudId != -1)
					{
						SDL_SetTextureAlphaMod(displayFogTexture, 255);//set the alpha to white again
						SDL_Rect r = { shroudId * 64,0,64,64 }; //this rect crops the desired fog Id texture from the fogTiles spritesheet
						app->render->Blit(displayFogTexture, worldPos.x, worldPos.y - halfTileHeight, &r);
					}

				}

			}

		}
	}
}

FoWEntity* ModuleFoWManager::CreateFoWEntity(fMPoint pos, bool providesVisibility, int visionRadius)
{
	FoWEntity* entity = nullptr;

	entity = new FoWEntity(pos, providesVisibility, visionRadius);

	if (entity != nullptr)
	{
		fowEntities.push_back(entity);
		MapNeedsUpdate();
	}

	return entity;
}


bool ModuleFoWManager::CheckTileVisibility(iMPoint mapPos)const
{
	bool ret = false;

	FoWDataStruct* tileState = GetFoWTileState(mapPos);

	if (tileState != nullptr)
	{

		if (tileState->tileFogBits != fow_ALL)
			ret = true;
	}

	return ret;
}

void ModuleFoWManager::MapNeedsUpdate()
{
	if (foWMapNeedsRefresh == false)
		foWMapNeedsRefresh = true;
}


unsigned short* ModuleFoWManager::GetMaskFromRadius(int radius)
{
	unsigned short* ret = nullptr;

	if (maskMap.count(radius) > 0) //if the key is found
	{
		ret = maskMap.at(radius).mask;
	}


	return ret;
}


void ModuleFoWManager::RequestMaskGeneration(int radius)
{

	if (maskMap.count(radius) > 0)
	{
		maskMap.at(radius).numberOfUsers += 1;
	}
	else
	{
		MaskData data;
		data.numberOfUsers = 1;
		data.mask = GenerateCircleJoints(radius, GenerateCircleBorders(radius, GenerateCircle(radius)));
		maskMap.insert(std::pair<uint, MaskData>(radius, data));
		//Fill Corners
		//Fill joints
	}


}


void ModuleFoWManager::RequestMaskDeletion(int radius)
{
	if (radius > 0)
	{
		if (maskMap.count(radius) > 0)
		{
			if (maskMap.at(radius).numberOfUsers > 1)
			{
				maskMap.at(radius).numberOfUsers -= 1;
			}
			else
			{
				//delete mask
				RELEASE_ARRAY(maskMap.at(radius).mask);
				maskMap.at(radius).mask = nullptr;
				maskMap.erase(radius);
			}
		}
	}

}


unsigned short* ModuleFoWManager::GenerateCircle(int radius)
{
	unsigned short* circle = nullptr;


	int diameter = (radius * 2) + 1;
	iMPoint center = { radius,radius };
	circle = new unsigned short[diameter * diameter];

	for (int y = 0; y < diameter; y++)
	{
		for (int x = 0; x < diameter; x++)
		{
			if (InsideCircle(center, { x,y }, radius) == true)
			{
				circle[(y * diameter) + x] = fow_NON;
			}
			else
			{
				circle[(y * diameter) + x] = fow_ALL;
			}
		}
	}

	return circle;
}

unsigned short* ModuleFoWManager::GenerateCircleBorders(int radius, unsigned short* mask)
{
	int diameter = (radius * 2) + 1;

	for (int y = 0; y < diameter; y++)
	{


		for (int x = 0; x < diameter; x++)
		{


			if (mask[(y * diameter) + x] == fow_NON)
			{
				//do tile check and change
				unsigned short aux = CheckCornersFromNeighbours({ x,y }, diameter, mask);
				switch (aux)
				{
				case fow_neighbour_W:
					mask[(y * diameter) + x] = fow_WWW;
					break;
				case fow_neighbour_E:
					mask[(y * diameter) + x] = fow_EEE;
					break;
				case fow_neighbour_N:
					mask[(y * diameter) + x] = fow_NNN;
					break;
				case fow_neighbour_S:
					mask[(y * diameter) + x] = fow_SSS;
					break;
				case fow_neighbour_CNE:
					mask[(y * diameter) + x] = fow_CNE;
					break;
				case fow_neighbour_CNW:
					mask[(y * diameter) + x] = fow_CNW;
					break;
				case fow_neighbour_CSE:
					mask[(y * diameter) + x] = fow_CSE;
					break;
				case fow_neighbour_CSW:
					mask[(y * diameter) + x] = fow_CSW;
					break;
				case fow_neighbour_HE:
					mask[(y * diameter) + x] = fow_ALL;
					mask[(y * diameter) + x + 1] = fow_WWW;
					break;
				case fow_neighbour_HW:
					mask[(y * diameter) + x] = fow_ALL;
					mask[(y * diameter) + x - 1] = fow_EEE;
					break;
				case fow_neighbour_HN:
					mask[(y * diameter) + x] = fow_ALL;
					mask[((y - 1) * diameter) + x] = fow_SSS;
					break;
				case fow_neighbour_HS:
					mask[(y * diameter) + x] = fow_ALL;
					mask[((y + 1) * diameter) + x + 1] = fow_NNN;
					break;
				}
			}


		}


	}

	return mask;
}

unsigned short* ModuleFoWManager::GenerateCircleJoints(int radius, unsigned short* mask)
{
	int diameter = (radius * 2) + 1;

	for (int y = 0; y < diameter; y++)
	{


		for (int x = 0; x < diameter; x++)
		{


			if (mask[(y * diameter) + x] == fow_NON)
			{
				//do tile check and change
				unsigned short aux = CheckJointsFromNeighbours({ x,y }, diameter, mask);

				switch (aux)
				{
				case fow_neighbour_CNE:
					mask[(y * diameter) + x] = fow_JNE;
					break;
				case fow_neighbour_CNW:
					mask[(y * diameter) + x] = fow_JNW;
					break;
				case fow_neighbour_CSE:
					mask[(y * diameter) + x] = fow_JSE;
					break;
				case fow_neighbour_CSW:
					mask[(y * diameter) + x] = fow_JSW;
					break;
				}




			}


		}


	}

	return mask;
}


bool ModuleFoWManager::InsideCircle(iMPoint center, iMPoint tile, float radius)
{
	float distance_squared = center.DistanceNoSqrt(tile);

	return distance_squared < (radius + 0.5f) * (radius + 0.5f);
}

unsigned short ModuleFoWManager::CheckCornersFromNeighbours(iMPoint pos, int diameter, unsigned short* mask)
{
	unsigned short ret = 0;

	//check West tile
	if (pos.x == 0)
	{
		ret += fow_neighbour_W;
	}
	else
	{
		if (mask[(pos.y * diameter) + pos.x - 1] == fow_ALL)
		{
			ret += fow_neighbour_W;
		}
	}

	//check East tile
	if (pos.x == diameter - 1)
	{
		ret += fow_neighbour_E;
	}
	else
	{
		if (mask[(pos.y * diameter) + pos.x + 1] == fow_ALL)
		{
			ret += fow_neighbour_E;
		}
	}

	//check North tile
	if (pos.y == 0)
	{
		ret += fow_neighbour_N;
	}
	else
	{
		if (mask[((pos.y - 1) * diameter) + pos.x] == fow_ALL)
		{
			ret += fow_neighbour_N;
		}
	}

	//check South tile
	if (pos.y == diameter - 1)
	{
		ret += fow_neighbour_S;
	}
	else
	{
		if (mask[((pos.y + 1) * diameter) + pos.x] == fow_ALL)
		{
			ret += fow_neighbour_S;
		}
	}

	return ret;
}

unsigned short ModuleFoWManager::CheckJointsFromNeighbours(iMPoint pos, int diameter, unsigned short* mask)
{
	unsigned short ret = 0;
	int leftTileId = (pos.y * diameter) + pos.x - 1;
	int rightTileId = (pos.y * diameter) + pos.x + 1;
	int upperTileId = ((pos.y - 1) * diameter) + pos.x;
	int bottomTileId = ((pos.y + 1) * diameter) + pos.x;





	if (mask[leftTileId] == fow_CNW || mask[leftTileId] == fow_CNE || mask[leftTileId] == fow_CSE || mask[leftTileId] == fow_CSW ||
		mask[leftTileId] == fow_WWW || mask[leftTileId] == fow_SSS || mask[leftTileId] == fow_NNN || mask[leftTileId] == fow_EEE)
	{
		ret += fow_neighbour_W;

	}

	//check East tile

	if (mask[rightTileId] == fow_CNW || mask[rightTileId] == fow_CNE || mask[rightTileId] == fow_CSE || mask[rightTileId] == fow_CSW ||
		mask[rightTileId] == fow_WWW || mask[rightTileId] == fow_SSS || mask[rightTileId] == fow_NNN || mask[rightTileId] == fow_EEE)
	{
		ret += fow_neighbour_E;
	}


	//check North tile

	if (mask[upperTileId] == fow_CNW || mask[upperTileId] == fow_CNE || mask[upperTileId] == fow_CSE || mask[upperTileId] == fow_CSW ||
		mask[upperTileId] == fow_WWW || mask[upperTileId] == fow_SSS || mask[upperTileId] == fow_NNN || mask[upperTileId] == fow_EEE)
	{
		ret += fow_neighbour_N;
	}


	//check South tile

	if (mask[bottomTileId] == fow_CNW || mask[bottomTileId] == fow_CNE || mask[bottomTileId] == fow_CSE || mask[bottomTileId] == fow_CSW ||
		mask[bottomTileId] == fow_WWW || mask[bottomTileId] == fow_SSS || mask[bottomTileId] == fow_NNN || mask[bottomTileId] == fow_EEE)
	{
		ret += fow_neighbour_S;
	}


	return ret;
}

