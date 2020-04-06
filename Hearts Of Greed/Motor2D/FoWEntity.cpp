#include "App.h"
#include "FoWManager.h"
#include "FoWEntity.h"
#include "FoWBitDefs.h"
#include "Map.h"

FoWEntity::FoWEntity(fMPoint WorldPos, bool providesVisibility) :boundingBoxRadius(3), deleteEntity(false), providesVisibility(providesVisibility), posInMap({ 0,0 }), isVisible(false)
{
	SetNewPosition(WorldPos);
}


FoWEntity::~FoWEntity()
{
	CleanUp();
}


bool FoWEntity::CleanUp()
{
	bool ret = true;

	return ret;
}


void FoWEntity::SetNewPosition(fMPoint pos)
{
	posInMap = app->map->WorldToMap((int)pos.x, (int)pos.y);

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
	if (radius >= fow_MIN_CIRCLE_RADIUS && radius <= fow_MAX_CIRCLE_RADIUS)
	{
		boundingBoxRadius = radius;
		app->fowManager->MapNeedsUpdate();
	}
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

//TODO 3: Comprehend and complete this function: (this is the function that does the magic for us)
void FoWEntity::ApplyMaskToTiles(std::vector<iMPoint>tilesAffected)
{

	//We first take the correct precomputed mask and store it in the precMask variable (it is recommended to see what they are made of. You can find the masks at the FoWManager.h module)
	//Note that it is an array
	unsigned short* precMask = &app->fowManager->circleMasks[boundingBoxRadius - fow_MIN_CIRCLE_RADIUS][0];

	//You have to complete the code inside this for
	for (int i = 0; i < tilesAffected.size(); i++)
	{
		//You have to request the fog & shroud values of each affected tile. Hint:(You can take both with a single function call)
		FoWDataStruct* tileValue = app->fowManager->GetFoWTileState(tilesAffected[i]);

		//And (bitwise AND) them with the mask if the tile FoW values are not nullptr
		//To bitwise AND values you just simply do this: value1 &= value2 
		//the operation result will be stored in the variable on the left side. 
		//In this case you want to modify the fog and shroud values that you have requested above

		if (tileValue != nullptr)
		{
			tileValue->tileShroudBits &= *precMask;
			tileValue->tileFogBits &= *precMask;
		}
		precMask++;
	}

}

//Applies the Mask to all tiles inside the radius
void FoWEntity::Update()
{
	if (providesVisibility)
		ApplyMaskToTiles(GetTilesInsideRadius());
}


iMPoint FoWEntity::GetPos()const
{
	return posInMap;
}
