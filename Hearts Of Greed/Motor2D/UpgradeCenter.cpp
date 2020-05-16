#include "UpgradeCenter.h"
#include "EventManager.h"

#include "Base.h"


UpgradeCenter::UpgradeCenter(fMPoint pos, int upgradeTurretCost, int upgradeBarricadeCost, int maxHitPoints, int currenthitPoints, int recoveryHitPointsRate, int xpOnDeath, int buildingCost, int transparency, Collider* collider) :
	
	Building(pos, maxHitPoints, currenthitPoints, recoveryHitPointsRate, xpOnDeath, buildingCost, transparency, collider, ENTITY_TYPE::BLDG_UPGRADE_CENTER),

	turretLvl(1),
	barricadeLvl(1),

	upgradeTurretCost(upgradeTurretCost),
	upgradeBarricadeCost(upgradeBarricadeCost)
{}


UpgradeCenter::UpgradeCenter(fMPoint position, UpgradeCenter * copy, ENTITY_ALIGNEMENT alignement) : 
	
	Building(position, copy, alignement),

	turretLvl(1), 
	barricadeLvl(1),

	upgradeTurretCost(copy->upgradeTurretCost),
	upgradeBarricadeCost(copy->upgradeBarricadeCost)
{
}


UpgradeCenter::~UpgradeCenter()
{
	turretLvl = -1;
	barricadeLvl = -1;

	upgradeTurretCost = -1;
	upgradeBarricadeCost = -1;
}


void UpgradeCenter::UpgradeTurrets()
{
	turretLvl++;
	myBase->LevelUpTurrets(turretLvl);
}


void UpgradeCenter::UpgradeBarricades()
{
	barricadeLvl++;
	myBase->LevelUpBarricades(barricadeLvl);
}


int UpgradeCenter::RecieveDamage(int damage)
{
	if (hitPointsCurrent > 0)
	{
		hitPointsCurrent -= damage;

		if (hitPointsCurrent <= 0)
		{
			Die();
		}
	}

	return 0;
}


void UpgradeCenter::Die()
{
	app->eventManager->GenerateEvent(EVENT_ENUM::ENTITY_DEAD, EVENT_ENUM::NULL_EVENT);
	toDelete = true;

	if (minimapIcon != nullptr)
	{
		minimapIcon->toDelete = true;
		minimapIcon->minimapPos = nullptr;
	}

	if (visionEntity != nullptr)
	{
		visionEntity->deleteEntity = true;
		visionEntity = nullptr;
	}

	if (myBase != nullptr)
		myBase->RemoveUpgradeCenter();

	myBase = nullptr;
}