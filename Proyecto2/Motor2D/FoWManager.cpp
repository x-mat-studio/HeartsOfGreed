#include "FoWManager.h"
#include "App.h"
#include "Textures.h"
#include "Map.h"
#include "Render.h"
#include "Input.h"

FoWManager::FoWManager()
{

}


FoWManager::~FoWManager()
{
	CleanUp();
}


bool FoWManager::Awake(pugi::xml_node&)
{
	bool ret = true;

	return ret;
}


bool FoWManager::Start()
{
	bool ret = true;

	smoothFoWtexture = app->tex->Load("spritesheets/VFX/fogTiles.png");
	debugFoWtexture = app->tex->Load("spritesheets/VFX/fogTilesDebug.png");

	if (smoothFoWtexture == nullptr || debugFoWtexture == nullptr);
	ret = false;

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


bool FoWManager::PreUpdate(float dt)
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
	}

	return ret;
}


bool FoWManager::Update(float dt)
{
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


bool FoWManager::PostUpdate(float dt)
{
	bool ret = true;
	if (foWMapVisible)
	{
		DrawFoWMap();
	}
	return ret;
}


bool FoWManager::CleanUp()
{
	bool ret = true;
	DeleteFoWMap();

	int numElements = fowEntities.size();

	for (int i = 0; i < numElements; i++)
	{
		RELEASE(fowEntities[i]);
		fowEntities[i] = nullptr;
	}

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


void FoWManager::ResetFoWMap()
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


FoWDataStruct* FoWManager::GetFoWTileState(iMPoint mapPos)const
{
	FoWDataStruct* ret = nullptr;
	if (CheckFoWTileBoundaries(mapPos) && fowMap != nullptr)
	{
		ret = &fowMap[(mapPos.y * width) + mapPos.x];
	}

	return ret;
}


bool FoWManager::CheckFoWTileBoundaries(iMPoint mapPos)const
{
	bool ret = false;
	if (mapPos.x >= 0 && mapPos.x < width && mapPos.y >= 0 && mapPos.y < height)
		ret = true;

	return ret;
}

void FoWManager::CreateFoWMap(uint w, uint h)
{
	width = w;
	height = h;

	DeleteFoWMap();
	fowMap = new FoWDataStruct[width * height];
	ResetFoWMap();

	MapNeedsUpdate();
}


void FoWManager::DeleteFoWMap()
{
	if (fowMap != nullptr)
	{
		RELEASE_ARRAY(fowMap);
		fowMap = nullptr;
	}
}


void FoWManager::UpdateFoWMap()
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



		if (!debugMode)
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

void FoWManager::DrawFoWMap()
{
	float halfTileHeight=app->map->data.tileHeight*0.5f;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			
			fMPoint worldPos;
			app->map->MapToWorldCoords(x, y,app->map->data,worldPos.x,worldPos.y);
			if (app->map->InsideCamera(worldPos.x,worldPos.y))
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

				}


				SDL_Texture* displayFogTexture = nullptr;
				if (debugMode)
				{
					displayFogTexture = debugFoWtexture;
				}
				else displayFogTexture = smoothFoWtexture;

				//draw fog
				if (fogId != -1)
				{
					SDL_SetTextureAlphaMod(displayFogTexture, 128);//set the alpha of the texture to half to reproduce fog
					SDL_Rect r = { fogId * 64,0,64,64 }; //this rect crops the desired fog Id texture from the fogTiles spritesheet
					app->render->Blit(displayFogTexture, worldPos.x, worldPos.y-halfTileHeight, &r);
				}
				if (shroudId != -1)
				{
					SDL_SetTextureAlphaMod(displayFogTexture, 255);//set the alpha to white again
					SDL_Rect r = { shroudId * 64,0,64,64 }; //this rect crops the desired fog Id texture from the fogTiles spritesheet
					app->render->Blit(displayFogTexture, worldPos.x, worldPos.y-halfTileHeight, &r);
				}

			}

		}
	}
}

FoWEntity* FoWManager::CreateFoWEntity(fMPoint pos, bool providesVisibility)
{
	FoWEntity* entity = nullptr;

	entity = new FoWEntity(pos, providesVisibility);

	if (entity != nullptr)
	{
		fowEntities.push_back(entity);
		MapNeedsUpdate();
	}

	return entity;
}


bool FoWManager::CheckTileVisibility(iMPoint mapPos)const
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

void FoWManager::MapNeedsUpdate()
{
	if (foWMapNeedsRefresh == false)
		foWMapNeedsRefresh = true;
}
