#pragma once

#ifndef __BASE_H__
#define __BASE_H__

#include "vector"
class Turret;
class Barricade;
class Core;
class UpgradeCenter;
class Building;
struct Collider;

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
