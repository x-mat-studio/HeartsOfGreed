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

	Base(fMPoint position, Collider* collider, int maxTurrets, int maxBarricades, UpgradeCenter* baseUpgradeCenter , std::vector <Turret*> baseTurrets, std::vector <Barricade*> baseBarricades, 
		 Collider* baseArea, int resourcesProduced, float resourcesRate, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int transparency);

	Base(fMPoint position, Collider* collider, int maxTurrets, int maxBarricades, UpgradeCenter* baseUpgradeCenter, Collider* baseArea, int resourcesProduced, float resourcesRate, 
		int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int transparency);

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

	void ChangeAligment();

	int RecieveDamage(int damage);
private:
	
	void DisableTurrets();
	bool RessurectHero(ENTITY_TYPE heroType);
	void GainResources(float dt);

	void Die();

public:
	int GetHP();
	int GetMaxHP();
	int GetRsrc();

private:
	int maxTurrets;
	int maxBarricades;
	Collider* baseAreaAlarm; //if this seems odd, ask Jose -Adri


	//Childs---
	UpgradeCenter* baseUpgradeCenter;
	std::vector <Turret*> turretsVector;
	std::vector <Barricade*> barricadesVector;

	int resourcesProduced;
	float resourcesRate;
	float resourcesCooldown;

	

};


#endif // !
