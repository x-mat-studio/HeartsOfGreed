#pragma once

#ifndef __BASE_H__
#define __BASE_H__
#include "Entity.h"
#include "Building.h"
#include "Entity.h"

class Base
{
public:

	Base(int maxTurrets, int maxBarricades, Core* baseCore, UpgradeCenter* baseUpgradeCenter , std::vector <Turret*> baseTurrets, std::vector <Barricade*> baseBarricades, Collider* baseArea);
	Base(Base* copy);

	~Base();

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
	
	void DisableTurrets();
	void AddBuilding(Building* building);
	void RemoveBuilding(Building* building);


private:
	int maxTurrets;
	int maxBarricades;
	Collider* baseAreaAlarm; //if this seems odd, ask Jose -Adri

	//Childs---
	Core* baseCore;
	UpgradeCenter* baseUpgradeCenter;
	std::vector <Turret*> baseTurrets;
	std::vector <Barricade*> baseBarricades;
};


#endif // !
