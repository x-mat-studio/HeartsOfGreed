#include "MeleeHero.h"
#include "EntityManager.h"

MeleeHero::MeleeHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp, 
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft, 
	Animation& idleLeftUp, Animation& idleLeftDown, int level, int hitPoints, int recoveryHitPointsRate, int energyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime,
	int skill1Dmg, SKILL_ID skill1Id, SKILL_TYPE skill1Type, ENTITY_ALIGNEMENT skill1Target) :
	
	Hero(position, ENTITY_TYPE::HERO_MELEE, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, level, hitPoints, recoveryHitPointsRate, energyPoints, recoveryEnergyRate,
		attackDamage, attackSpeed, attackRange, movementSpeed, vision, skill1ExecutionTime, skill2ExecutionTime, 
		skill3ExecutionTime, skill1RecoverTime, skill2RecoverTime, skill3RecoverTime,
		skill1Dmg, skill1Id, skill1Type, skill1Target)
{}


MeleeHero::MeleeHero(fMPoint position, MeleeHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement)
{}

bool MeleeHero::ActivateSkill1()
{

	return true;
}

bool MeleeHero::ActivateSkill2()
{

	return true;
}

bool MeleeHero::ActivateSkill3()
{

	return true;
}

bool MeleeHero::PreProcessSkill1()
{
	app->entityManager->RequestArea(SKILL_ID::MELEE_SKILL1, &this->currAoE, this->origin);
	return true;
}

bool MeleeHero::PreProcessSkill2()
{

	return true;
}

bool MeleeHero::PreProcessSkill3()
{

	return true;
}

void MeleeHero::LevelUp()
{

	hitPoints;
	recoveryHitPointsRate;
	energyPoints;
	recoveryEnergyRate;

	attackDamage;
	attackSpeed;
	attackRange;

	unitSpeed;
	visionDistance;

}
