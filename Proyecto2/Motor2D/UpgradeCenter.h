#ifndef _UPGRADECENTER_H_
#define _UPGRADECENTER_H_

#include "Module.h"
#include "Building.h"
#include "Collision.h"


class UpgradeCenter : public Building
{

public:

	UpgradeCenter(int turretLvl, int barricadeLvl);
	~UpgradeCenter();


	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void UpgradeTurrets();
	void UpgradeBarricades();


private:

	int turretLvl;
	int barricadeLvl;

};



#endif // _UPGRADECENTER_H_
