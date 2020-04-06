#pragma once

#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Entity.h"

class Core;
class UpgradeCenter;
class Turret;
class Barricade;
class Base;

enum class BUILDING_STATE : int
{
	ST_UNKNOWN = -1,

	ST_ALLY,
	ST_NEUTRAL,
	ST_ENEMY,

	ST_ALL,
};

class Building : public Entity
{
public:

	Building(fMPoint position, int hitPoints, int recoveryHitPointsRate, int xpOnDeath, int buildingCost, int transparency, Collider* collider);

	Building(fMPoint position, Building* copy, ENTITY_ALIGNEMENT alignement);

	Building();

	void Destroy();

	//Destructor
	~Building();

	// Called each loop iteration
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void OnCollision(Collider* collider);
	void Draw(float dt);

	void ActivateTransparency();

private:
	void ChangeBuildingState(BUILDING_STATE state);

	void BeingRepaired();
	void RecoverHealth();

	void Contruct();



public:

	BUILDING_STATE	currentState;
	Base* myBase;


private:
	int			transparencyValue;

	int			hitPointsMax;
	int			hitPointsCurrent;
	int			recoveryHitPointsRate;
	int			xpOnDeath;
	int			buildingCost;

	bool		transparent;

	bool		selected;
};


#endif // !
