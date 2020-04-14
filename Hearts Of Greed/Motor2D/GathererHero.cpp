#include "GathererHero.h"
#include "EntityManager.h"
#include "Map.h"
#include "Input.h"

GathererHero::GathererHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown, int level, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int energyPoints, int recoveryEnergyRate,
	int attackDamage, float attackSpeed, int attackRange, int movementSpeed, int vision, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime,
	int skill1Dmg, SKILL_ID skill1Id, SKILL_TYPE skill1Type, ENTITY_ALIGNEMENT skill1Target) :

	Hero(position, ENTITY_TYPE::HERO_GATHERER, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, level, maxHitPoints, currentHitPoints, recoveryHitPointsRate, energyPoints, recoveryEnergyRate,
		attackDamage, attackSpeed, attackRange, movementSpeed, vision, skill1ExecutionTime, skill2ExecutionTime,
		skill3ExecutionTime, skill1RecoverTime, skill2RecoverTime, skill3RecoverTime,
		skill1Dmg, skill1Id, skill1Type, skill1Target)
{}


GathererHero::GathererHero(fMPoint position, GathererHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement)
{}


bool GathererHero::ActivateSkill1(fMPoint mouseClick)
{
	bool ret = false;
	//Hola soy Marc :)

	if (currAreaInfo)
	{
		inputs.push_back(IN_SKILL1);
		ret = true;
	}

	return ret;
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
	if (currAoE.size() == 0)
	{
		//app->audio->PlayFx(app->entityManager->suitman1Skill2, 0, 7, this->GetMyLoudness(), this->GetMyDirection());
		

		origin = app->map->WorldToMap(round(position.x), round(position.y));
		origin = app->map->MapToWorld(origin.x, origin.y);

		currAreaInfo = app->entityManager->RequestArea(skill1.id, &this->currAoE, this->origin);
	}

	iMPoint center = app->map->WorldToMap(position.x, position.y);
	granadePosLaunch = app->input->GetMouseWorld();

	if (center.InsideCircle(app->map->WorldToMap(granadePosLaunch.x, granadePosLaunch.y), currAreaInfo->radius))
	{
		granadeArea = app->entityManager->RequestArea(SKILL_ID::GATHERER_SKILL1_MOUSE, &this->suplAoE, { (int)granadePosLaunch.x, (int)granadePosLaunch.y });
	}

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

bool GathererHero::ExecuteSkill1()
{

	if (granadeArea) 
	{
		app->audio->PlayFx(app->entityManager->suitman1Skill, 0, 6, this->GetMyLoudness(), this->GetMyDirection());
		app->audio->PlayFx(app->entityManager->suitman1Skill2, 0, 7, this->GetMyLoudness(), this->GetMyDirection());
		return app->entityManager->ExecuteSkill(skill1.dmg, { (int)granadePosLaunch.x, (int)granadePosLaunch.y }, this->granadeArea, skill1.target, skill1.type, true, (Entity*)this);
	}	
	else
		return false;
}

bool GathererHero::ExecuteSkill2()
{

	return true;
}

bool GathererHero::ExecuteSkill3()
{

	return true;
}

void GathererHero::LevelUp()
{

	hitPointsMax;
	hitPointsCurrent;		// As it has been comented on both ranged and melee heroes previously, whichever health may be				Ferran, with love, from Ayowa, United States of the Great Country of America! Except I don't like american culture that much, so fuck America, long live Japan, bring me nekos and waifus, and not necessarily in that order >:D
	recoveryHitPointsRate;
	energyPoints;
	recoveryEnergyRate;

	attackDamage;
	attackSpeed;
	attackRange;

	unitSpeed;
	visionDistance;

}
