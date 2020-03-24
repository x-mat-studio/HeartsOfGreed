#include "GathererHero.h"

GathererHero::GathererHero(iMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, int level, int hitPoints, int recoveryHitPointsRate, int energyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float attackCooldown, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime) :

	Hero(position, ENTITY_TYPE::HERO_GATHERER, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, level, hitPoints, recoveryHitPointsRate, energyPoints, recoveryEnergyRate,
		attackDamage, attackSpeed, attackRange, movementSpeed, vision, attackCooldown, skill1ExecutionTime, skill2ExecutionTime,
		skill3ExecutionTime, skill1RecoverTime, skill2RecoverTime, skill3RecoverTime)
{}


GathererHero::GathererHero(iMPoint position, GathererHero* copy) :

	Hero(position, ENTITY_TYPE::HERO_GATHERER, copy->GetCollider(), copy->walkLeft, copy->walkLeftUp, copy->walkLeftDown, copy->walkRightUp, copy->walkRightDown, copy-> walkRight,
		copy->idleRight, copy->idleRightDown, copy->idleRightUp, copy->idleLeft, copy->idleLeftUp, copy->idleLeftDown, copy->level, copy->hitPoints, copy->recoveryHitPointsRate, 
		copy->energyPoints, copy->recoveryEnergyRate, copy->attackDamage, copy->attackSpeed, copy->attackRange, copy->movementSpeed, copy->vision, copy->attackCooldown, 
		copy->skill1ExecutionTime, copy->skill2ExecutionTime, copy->skill3ExecutionTime, copy->skill1RecoverTime, copy->skill2RecoverTime, copy->skill3RecoverTime)
{}


bool GathererHero::UseHability1()
{
	//Hola soy Marc :)
	return true;
}

bool GathererHero::UseHability2()
{

	return true;
}

bool GathererHero::UseHability3()
{

	return true;
}