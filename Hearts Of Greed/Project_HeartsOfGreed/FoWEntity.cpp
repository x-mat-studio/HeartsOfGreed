#include "App.h"
#include "FoWManager.h"
#include "FoWEntity.h"
#include "FoWBitDefs.h"
#include "Map.h"

FoWEntity::FoWEntity(fMPoint WorldPos, bool providesVisibility, int visionRadius, int isVisbleRadius) : deleteEntity(false), providesVisibility(providesVisibility), posInMap({ 0,0 }), isVisible(false), boundingBoxRadius(0), isVisibleRadius(0)
{

	SetNewVisionRadius(visionRadius);
	SetNewVisibleRadius(isVisbleRadius);
	SetNewPosition(WorldPos);

}


FoWEntity::~FoWEntity()
{
	CleanUp();
}


bool FoWEntity::CleanUp()
{
	bool ret = true;
	app->fowManager->RequestMaskDeletion(boundingBoxRadius);
	app->fowManager->RequestMaskDeletion(isVisibleRadius);
	return ret;
}


void FoWEntity::SetNewPosition(fMPoint pos)
{
	posInMap = app->map->WorldToMap(pos.x, pos.y);

	if (providesVisibility)
	{
		//updates the visibility map
		app->fowManager->MapNeedsUpdate();
	}
	else
	{
		//only updates this entity visibility
		if (app->fowManager->CheckTileVisibility(posInMap))
		{
			isVisible = true;
		}
		else isVisible = false;
	}

}


void FoWEntity::SetNewVisionRadius(uint radius)
{
	//Changes the vision radius of the entity if there's a precomputed shape with that radius
	app->fowManager->RequestMaskDeletion(boundingBoxRadius);
	boundingBoxRadius = radius;
	app->fowManager->RequestMaskGeneration(boundingBoxRadius);
	if (providesVisibility == true)
	{
		app->fowManager->MapNeedsUpdate();
	}

}

void FoWEntity::SetEntityProvideVision(bool providesVision)
{
	if (providesVisibility != providesVision)
	{
		providesVisibility = providesVision;

		app->fowManager->MapNeedsUpdate();
	}
}

bool FoWEntity::GetEntityProvideVision()
{
	return providesVisibility;
}


void FoWEntity::SetNewVisibleRadius(uint radius)
{
	//Changes the visible radius of the entity if there's a precomputed shape with that radius
	app->fowManager->RequestMaskDeletion(isVisibleRadius);
	isVisibleRadius = radius;

	if (radius > 1)
	{
		app->fowManager->RequestMaskGeneration(isVisibleRadius);
	}

	app->fowManager->MapNeedsUpdate();
}


bool FoWEntity::UpdateVisibility()
{
	bool retVisible = false;

	if (providesVisibility == true)
	{
		isVisible = true;
		return true;
	}

	if (isVisibleRadius > 1)
	{
		retVisible = IsEntityVisible();
	}
	else //when the area in which you are visible is only 1 or 0 tiles, simplified case
	{
		if (app->fowManager->CheckTileVisibility(posInMap))
		{
			retVisible = true;
		}
		else retVisible = false;
	}

	isVisible = retVisible;
	return retVisible;
}



std::vector<iMPoint> FoWEntity::GetTilesInsideRadius(uint radius)const
{
	std::vector<iMPoint> ret;
	int length = (radius * 2) + 1;
	iMPoint startingPos = posInMap - radius;
	iMPoint finishingPos = startingPos + length;

	//Creates a vector with all the tiles inside a bounding box delimited by the radius
	for (int i = startingPos.y; i < finishingPos.y; i++)
	{
		for (int j = startingPos.x; j < finishingPos.x; j++)
		{
			ret.push_back({ j,i });
		}
	}

	return ret;
}


void FoWEntity::ApplyMaskToTilesAuto(std::vector<iMPoint>tilesAffected)
{
	unsigned short* precMask = &app->fowManager->GetMaskFromRadius(boundingBoxRadius)[0];
	if (precMask != nullptr)
	{
		for (int i = 0; i < tilesAffected.size(); i++)
		{

			FoWDataStruct* tileValue = app->fowManager->GetFoWTileState(tilesAffected[i]);


			if (tileValue != nullptr)
			{
				tileValue->tileShroudBits &= *precMask;
				tileValue->tileFogBits &= *precMask;
			}

			precMask++;
		}
	}
}


bool FoWEntity::IsEntityVisible()
{
	bool ret = false;

	std::vector<iMPoint> tilesAffected = GetTilesInsideRadius(isVisibleRadius);

	unsigned short* precMask = &app->fowManager->GetMaskFromRadius(isVisibleRadius)[0];
	if (precMask != nullptr)
	{
		for (int i = 0; i < tilesAffected.size(); i++)
		{

			FoWDataStruct* tileValue = app->fowManager->GetFoWTileState(tilesAffected[i]);


			if (tileValue != nullptr&& tileValue->tileShroudBits!=fow_ALL)
			{
				ret = true;
				break;
			}

			precMask++;
		}
	}
	return ret;
}

//Applies the Mask to all tiles inside the radius
void FoWEntity::Update()
{
	if (providesVisibility)
		ApplyMaskToTilesAuto(GetTilesInsideRadius(boundingBoxRadius));
}


iMPoint FoWEntity::GetPos()const
{
	return posInMap;
}
