#include "RangedHero.h"

RangedHero::RangedHero(iMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, int level, int hitPoints, int recoveryHitPointsRate, int energyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float attackCooldown, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime) :

	Hero(position, ENTITY_TYPE::HERO_RANGED, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, level, hitPoints, recoveryHitPointsRate, energyPoints, recoveryEnergyRate,
		attackDamage, attackSpeed, attackRange, movementSpeed, vision, attackCooldown, skill1ExecutionTime, skill2ExecutionTime,
		skill3ExecutionTime, skill1RecoverTime, skill2RecoverTime, skill3RecoverTime)
{}


bool RangedHero::UseHability1()
{

	return true;
}

bool RangedHero::UseHability2()
{

	return true;
}

bool RangedHero::UseHability3()
{

	return true;
}