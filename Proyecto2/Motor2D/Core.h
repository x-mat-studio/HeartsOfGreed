#ifndef _CORE_H_
#define _CORE_H_

#include "Module.h"
#include "Building.h"
#include "Collision.h"
#include "Hero.h"


class Core : public Building 
{

public:

	Core(int resourceProductionRate, fMPoint position, Collider* collider, int hitPoints = 100, int recoveryHitPointsRate = 5, int xpOnDeath = 100, int buildingCost = 50, int transparency = 0);
	Core(Core* copy);

	~Core();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void RessurectHero(Hero* hero);
	void GainResources();

private: 

	int resourceProductionRate;
};



#endif // _CORE_H_