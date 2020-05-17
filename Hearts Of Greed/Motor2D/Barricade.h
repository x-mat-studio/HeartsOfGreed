#ifndef _BARRICADE_H_
#define _BARRICADE_H_

#include "Module.h"
#include "Building.h"

class Barricade : public Building
{

public:

	Barricade::Barricade(fMPoint position, int maxHitPoints, int currenthitPoints, int recoveryHitPointsRate, int xpOnDeadth, int buildingCost, int transparency, Collider* collider, int barricadeLvl);
	Barricade(fMPoint position, Barricade* copy, ENTITY_ALIGNEMENT align);
	~Barricade();

	void Flip();
	void SetLevel(int level);

	void DrawSelected();

	int RecieveDamage(int damage);

private:
	void Die();


private:
	int barricadeLvl;

	bool vertical;
};



#endif // _BARRICADE_H_
