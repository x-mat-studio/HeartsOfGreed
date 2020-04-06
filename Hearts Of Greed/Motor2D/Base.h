#pragma once

#ifndef __BASE_H__
#define __BASE_H__

#include "vector"
#include "Building.h"

class Turret;
class Barricade;
class UpgradeCenter;
class Hero;

struct Collider;

class Base : public Building
{
public:

	Base(fMPoint position, Collider* collider, int maxTurrets, int maxBarricades, UpgradeCenter* baseUpgradeCenter , std::vector <Turret*> baseTurrets, 
		std::vector <Barricade*> baseBarricades, Collider* baseArea, int resourceProductionRate, int hitPoints, int recoveryHitPointsRate, int transparency);

	Base(fMPoint position, Base* copy, ENTITY_ALIGNEMENT alignement);

	~Base();



	bool AddTurret(Turret* turret);
	bool AddBarricade(Barricade* barricade);
	bool AddUpgradeCenter(UpgradeCenter* upgradeCenter);

	void RemoveTurret(Turret* turret);
	void RemoveBarricade(Barricade* barricade);
	void RemoveUpgradeCenter();

	bool Update(float dt);
	bool PostUpdate(float dt);

	void ChangeAligment(ENTITY_ALIGNEMENT aligment);
private:
	
	void DisableTurrets();
	bool RessurectHero(ENTITY_TYPE heroType);
	void GainResources(float dt);


private:
	int maxTurrets;
	int maxBarricades;
	Collider* baseAreaAlarm; //if this seems odd, ask Jose -Adri


	//Childs---
	UpgradeCenter* baseUpgradeCenter;
	std::vector <Turret*> turretsVector;
	std::vector <Barricade*> barricadesVector;

	int resourceProductionRate;

};


#endif // !
