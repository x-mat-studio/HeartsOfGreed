#ifndef _TURRET_H_
#define _TURRET_H_

#include "Module.h"
#include "Building.h"
#include "Collision.h"


class Turret : public Building
{

public:

	Turret(int turretLvl, int attackDmg, int attackSpeed, int range,fMPoint position, Collider* collider, int hitPoints=100,int recoveryHitPointsRate=5, int xpOnDeath=100, int buildingCost=50, int transparency = 0);
	Turret(Turret* copy);
	~Turret();


	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void Target();
	void Attack();

private:
	int turretLvl;
	int attackDmg;
	int attackSpeed;
	int range;
};



#endif // _TURRET_H_
