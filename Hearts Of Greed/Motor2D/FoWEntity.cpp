#include "App.h"
#include "FoWManager.h"
#include "FoWEntity.h"
#include "FoWBitDefs.h"
#include "Map.h"

FoWEntity::FoWEntity(fMPoint WorldPos, bool providesVisibility, int visionRadius) : deleteEntity(false), providesVisibility(providesVisibility), posInMap({ 0,0 }), isVisible(false)
{
	if (providesVisibility)
	{
		SetNewVisionRadius(visionRadius);
	}
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
	app->fowManager->MapNeedsUpdate();

}

std::vector<iMPoint> FoWEntity::GetTilesInsideRadius()const
{
	std::vector<iMPoint> ret;
	int length = (boundingBoxRadius * 2) + 1;
	iMPoint startingPos = posInMap - boundingBoxRadius;
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

//Applies the Mask to all tiles inside the radius
void FoWEntity::Update()
{
	if (providesVisibility)
		ApplyMaskToTilesAuto(GetTilesInsideRadius());
}


iMPoint FoWEntity::GetPos()const
{
	return posInMap;
}
