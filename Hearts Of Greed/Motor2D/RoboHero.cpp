#include "RoboHero.h"
#include "EntityManager.h"
#include "Map.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

RoboHero::RoboHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown, 
	Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, 
	Animation& tileOnWalk, int level, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int maxEnergyPoints, int energyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime,
	int skill1Dmg, SKILL_ID skill1Id, SKILL_TYPE skill1Type, ENTITY_ALIGNEMENT skill1Target, SKILL_EFFECT skill1Effect, int hpLevelUp, int damageLevelUp, int energyLevelUp, int atkSpeedLevelUp) :

	Hero(position, ENTITY_TYPE::HERO_ROBO, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, 
		tileOnWalk, level, maxHitPoints, currentHitPoints, recoveryHitPointsRate, maxEnergyPoints, energyPoints, recoveryEnergyRate,
		attackDamage, attackSpeed, attackRange, movementSpeed, vision, skill1ExecutionTime, skill2ExecutionTime,
		skill3ExecutionTime, skill1RecoverTime, skill2RecoverTime, skill3RecoverTime,
		skill1Dmg, skill1Id, skill1Type, skill1Target,skill1Effect, hpLevelUp, damageLevelUp, energyLevelUp, atkSpeedLevelUp)

{}


RoboHero::RoboHero(fMPoint position, RoboHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement)
{}


RoboHero::~RoboHero()
{
}


bool RoboHero::ActivateSkill1(fMPoint clickPosition)
{

	return true;
}

bool RoboHero::ActivateSkill2()
{

	return true;
}

bool RoboHero::ActivateSkill3()
{

	return true;
}

bool RoboHero::PreProcessSkill1()
{

	return true;
}

bool RoboHero::PreProcessSkill2()
{

	return true;
}

bool RoboHero::PreProcessSkill3()
{

	return true;
}

bool RoboHero::ExecuteSkill1()
{

	return true;
}

bool RoboHero::ExecuteSkill2()
{

	return true;
}

bool RoboHero::ExecuteSkill3()
{

	return true;
}

void RoboHero::LevelUp()
{

	hitPointsMax += (hpLevelUpConstant * app->entityManager->robottoLifeUpgradeValue);
	hitPointsCurrent = hitPointsMax;
	//recoveryHitPointsRate;
	energyPoints += (energyLevelUpConstant * app->entityManager->robottoEnergyUpgradeValue);
	recoveryEnergyRate;

	attackDamage += (damageLevelUpConstant * app->entityManager->robottoDamageUpgradeValue);
	attackSpeed += (attackSpeedLevelUpConstant * app->entityManager->robottoAtkSpeedUpgradeValue);
	attackRange;

	unitSpeed;
	visionDistance;

}

void RoboHero::PlayGenericNoise(int probability)
{

}
