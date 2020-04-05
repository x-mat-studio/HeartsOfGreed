#ifndef __MELEE_HERO_H__
#define __MELEE_HERO_H__

#include "Hero.h"

class MeleeHero : public Hero
{
public:
	MeleeHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
		Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft, 
		Animation& idleLeftUp, Animation& idleLeftDown, int level, int hitPoints, int recoveryHitPointsRate, int energyPoints, int recoveryEnergyRate,
		int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float attackCooldown, float skill1ExecutionTime,
		float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime);

	MeleeHero(fMPoint position, MeleeHero* copy);

	bool UseHability1();
	bool UseHability2();
	bool UseHability3();
};


#endif //__MELEE_HERO_H__