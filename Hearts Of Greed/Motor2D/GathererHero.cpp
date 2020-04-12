#include "GathererHero.h"
#include "EntityManager.h"

GathererHero::GathererHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown, int level, int hitPoints, int recoveryHitPointsRate, int energyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime,
	int skill1Dmg, SKILL_ID skill1Id, SKILL_TYPE skill1Type, ENTITY_ALIGNEMENT skill1Target) :

	Hero(position, ENTITY_TYPE::HERO_GATHERER, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, level, hitPoints, recoveryHitPointsRate, energyPoints, recoveryEnergyRate,
		attackDamage, attackSpeed, attackRange, movementSpeed, vision, skill1ExecutionTime, skill2ExecutionTime,
		skill3ExecutionTime, skill1RecoverTime, skill2RecoverTime, skill3RecoverTime,
		skill1Dmg, skill1Id, skill1Type, skill1Target)
{}


GathererHero::GathererHero(fMPoint position, GathererHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement)
{}


bool GathererHero::ActivateSkill1(iMPoint mouseClick)
{
	bool ret = false;
	//Hola soy Marc :)

	//if(mouseClick) is anywhere

	inputs.push_back(IN_SKILL1);

	return true;
}

bool GathererHero::ActivateSkill2()
{

	return true;
}

bool GathererHero::ActivateSkill3()
{

	return true;
}

bool GathererHero::PreProcessSkill1()
{
	if(currAoE.size() == 0)
		currAreaInfo = app->entityManager->RequestArea(skill1.id, &this->currAoE, this->origin);

	return true;
}

bool GathererHero::PreProcessSkill2()
{

	return true;
}

bool GathererHero::PreProcessSkill3()
{

	return true;
}

void GathererHero::LevelUp()
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
