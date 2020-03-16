#pragma once

#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Entity.h"
#include "vector"

class Core;
class UpgradeCenter;
class Turret;
class Barricade;

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

	Building(int hitPoints, int recoveryHitPointsRate, int maxTurrets, int maxBarricades, int turretsLevel, int resourceOutput, int xpOnDeath);

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
	bool Load(pugi::xml_node&) { return true; };
	//Called to save the game
	bool Save(pugi::xml_node&) const { return true; }

private:

	//void ChangeToNeutral();
	//void DisableTurrets();

	//void BeingRepaired();

	//void RecoverHealth();

	//void AddTurret();
	//void AddBarricade();

	//void InConstruction();

private:

	int hitPoints;
	int recoveryHitPointsRate;

	int maxTurrets;
	int maxBarricades;
	int turretsLevel;

	int resourceOutput;
	int xpOnDeath;

	BUILDING_STATE currentState;

	bool selected;

	//Childs---
	Core* baseCore;
	UpgradeCenter* upgrCenterBase;
	std::vector<Turret*> baseTurrets;
	std::vector <Barricade*> baseBarr;

};


#endif // !
