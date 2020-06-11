#ifndef __FOW_ENTITY_H__
#define __FOW_ENTITY_H__
#include "p2Point.h"
#include "FoWBitDefs.h"
#include <vector>

class FoWEntity
{
	//FUNCTIONS
public:
	FoWEntity(fMPoint WorldPos,bool providesVisibility, int visionRadius, int isVisbleRadius);
	~FoWEntity();

	bool CleanUp();
	void Update();
	//Changes the position to the given one in world coordinates
	void SetNewPosition(fMPoint pos);
	iMPoint GetPos()const;

	void SetNewVisionRadius(uint radius);
	void SetEntityProvideVision(bool providesVision);
	bool GetEntityProvideVision();

	void SetNewVisibleRadius(uint radius);
	bool UpdateVisibility();

	std::vector<iMPoint> GetTilesInsideRadius(uint radius)const;


private:
	
	void ApplyMaskToTilesAuto(std::vector<iMPoint>tilesAffected);
	bool IsEntityVisible();

	//VARIABLES
public:
	bool deleteEntity;	
	bool isVisible;
private:
	iMPoint posInMap;

	bool providesVisibility;

	//Max radius from the entity at which tiles are affected (square to be checked)
	uint boundingBoxRadius;

	uint isVisibleRadius;

};

#endif // !__FOW_ENTITY_H__
