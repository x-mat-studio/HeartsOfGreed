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
	Animation& tileOnWalk, int level, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int maxEnergyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, Skill& skill1, int hpLevelUp, int damageLevelUp, int energyLevelUp, int atkSpeedLevelUp) :

	Hero(position, ENTITY_TYPE::HERO_ROBO, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, 
		tileOnWalk, level, maxHitPoints, currentHitPoints, recoveryHitPointsRate, maxEnergyPoints, recoveryEnergyRate,
		attackDamage, attackSpeed, attackRange, movementSpeed, vision, skill1, hpLevelUp, damageLevelUp, energyLevelUp, atkSpeedLevelUp)

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
	maxEnergyPoints += (energyLevelUpConstant * app->entityManager->gathererEnergyUpgradeValue);
	energyPoints = maxEnergyPoints; 
	recoveryEnergyRate;

	attackDamage += (damageLevelUpConstant * app->entityManager->robottoDamageUpgradeValue);
	attackSpeed += (attackSpeedLevelUpConstant * app->entityManager->robottoAtkSpeedUpgradeValue);
	attackRange;

	unitSpeed;
	visionDistance;

	heroSkillPoints++;
}

void RoboHero::PlayGenericNoise(int probability)
{

}
