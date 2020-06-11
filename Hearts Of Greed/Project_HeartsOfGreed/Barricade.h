#ifndef _BARRICADE_H_
#define _BARRICADE_H_

#include "Building.h"

enum class DIRECTION_BARRICADE : int
{
	NONE = -1,

	HORIZONTAL,
	VERTICAL
};


class Barricade : public Building
{

public:

	Barricade(fMPoint position, int maxHitPoints, int currenthitPoints, int recoveryHitPointsRate, int xpOnDeadth, int buildingCost,
			  int transparency, Collider* collider, SDL_Rect& verticalRect, SDL_Rect& horizontalRect, float hpIncrease);
	Barricade(fMPoint position, Barricade* copy, ENTITY_ALIGNEMENT align);
	~Barricade();

	void Flip();

	int GetLevel() const;
	void SetLevel(int level);

	void Draw(float dt);
	void DrawSelected();

	int RecieveDamage(float damage, bool ignoreArmor = false);

	void LevelUp();

private:
	void Die();


private:
	SDL_Rect verticalRect;
	SDL_Rect horizontalRect;

	SDL_Rect* currentRect;

	float hpIncrease;

	int barricadeLvl;

	DIRECTION_BARRICADE direction;
};



#endif // _BARRICADE_H_
