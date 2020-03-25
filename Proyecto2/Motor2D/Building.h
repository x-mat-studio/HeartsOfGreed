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

	Building(int hitPoints, int recoveryHitPointsRate, int xpOnDeath, int buildingCost);

	Building(fMPoint position, Building* copy);

	Building();

	void Destroy();

	//Destructor
	~Building();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	//Called when loading a save
	bool Load(pugi::xml_node&);
	//Called to save the game
	bool Save(pugi::xml_node&) const;

private:
	void ChangeBuildingState(BUILDING_STATE state);

	void BeingRepaired();
	void RecoverHealth();

	void Contruct();
	void Draw();


public:

	BUILDING_STATE	currentState;
	Base*			myBase;


private:

	int			hitPointsMax;
	int			hitPointsCurrent;
	int			recoveryHitPointsRate;
	int			xpOnDeath;
	int			buildingCost;

	bool		selected;




};


#endif // !
