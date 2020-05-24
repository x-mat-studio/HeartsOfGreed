#ifndef _UPGRADECENTER_H_
#define _UPGRADECENTER_H_

#include "Building.h"

struct Collider;
class Base;

class UpgradeCenter : public Building
{

public:

	UpgradeCenter(fMPoint pos, int upgradeTurretCost, int upgradeBarricadeCost, int maxHitPoints, int currenthitPoints, int recoveryHitPointsRate, 
				  int xpOnDeath, int buildingCost, int transparency, Collider* collider);
	UpgradeCenter(fMPoint position, UpgradeCenter* copy, ENTITY_ALIGNEMENT alignement);
	~UpgradeCenter();

	void UpgradeTurrets();
	void UpgradeBarricades();

	void ChangeTextures();

	int RecieveDamage(int damage);

private:
	void Die();

private:

	int turretLvl;
	int barricadeLvl;

	int upgradeTurretCost;
	int upgradeBarricadeCost;
};



#endif // _UPGRADECENTER_H_
