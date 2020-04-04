#pragma once

#ifndef __BASE_H__
#define __BASE_H__

#include "vector"
#include "Entity.h"

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

	void AddTurret(Turret* turret);
	void AddBarricade(Barricade* barricade);
	void AddUpgradeCenter(UpgradeCenter* upgradeCenter);

	void RemoveTurret(Turret* turret);
	void RemoveBarricade(Barricade* barricade);
	void RemoveUpgradeCenter();

	void ChangeAligment(ENTITY_ALIGNEMENT aligment);
private:
	
	void DisableTurrets();
	


private:
	int maxTurrets;
	int maxBarricades;
	Collider* baseAreaAlarm; //if this seems odd, ask Jose -Adri

	ENTITY_ALIGNEMENT baseAligment;

	//Childs---
	Core* baseCore;
	UpgradeCenter* baseUpgradeCenter;
	std::vector <Turret*> turretsVector;
	std::vector <Barricade*> barricadesVector;


};


#endif // !
