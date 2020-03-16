#pragma once

#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Entity.h"
#include "vector"

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

	Building(int hitPoints, int recoveryHitPointsRate, int xpOnDeath);

	Building(SDL_Point position, Building* copy);

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


public:

	BUILDING_STATE currentState;

	Base* myBase;


private:

	void ChangeBuildingState(BUILDING_STATE state);

	void BeingRepaired();
	void RecoverHealth();

	void Contruct();


private:

	int hitPointsMax;
	int hitPointsCurrent;
	int recoveryHitPointsRate;
	int xpOnDeath;

	bool selected;
};


#endif // !
