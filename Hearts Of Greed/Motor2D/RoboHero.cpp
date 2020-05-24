#include "RoboHero.h"
#include "EntityManager.h"
#include "Map.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "ParticleSystem.h"

RoboHero::RoboHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown, 
	Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, 
	Animation& tileOnWalk, HeroStats& stats, Skill& skill1) :

	Hero(position, ENTITY_TYPE::HERO_ROBO, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, 
		tileOnWalk, stats, skill1)

{}


RoboHero::RoboHero(fMPoint position, RoboHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement)
{}


RoboHero::~RoboHero()
{
}


bool RoboHero::ActivateSkill1(fMPoint clickPosition)
{

	inputs.push_back(IN_SKILL1);

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

	if (currAoE.size() == 0)
	{
		origin = app->map->WorldToMap(round(position.x), round(position.y));
		origin = app->map->MapToWorld(origin.x, origin.y);
		currAreaInfo = app->entityManager->RequestAreaInfo(skill1.rangeRadius);


		if (currAreaInfo != nullptr)
			app->entityManager->CreateDynamicArea(&this->currAoE, skill1.rangeRadius, origin, currAreaInfo);
	}

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

	if (!skillExecutionDelay)
	{
		if (!godMode)
			stats.currEnergy -= skill1.energyCost;

		skillExecutionDelay = true;



		return skillExecutionDelay;
	}
	else
	{

		int ret = 0;

		ret = app->entityManager->ExecuteSkill(skill1, this->origin);

		currAoE.clear();
		suplAoE.clear();
		currAreaInfo = nullptr;

	
		Die();

		return true;
	}

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
	//lvl up effect
	if (myParticleSystem != nullptr)
	myParticleSystem->Activate();
	else {
		myParticleSystem = (ParticleSystem*)app->entityManager->AddParticleSystem(TYPE_PARTICLE_SYSTEM::MAX, position.x, position.y);
	}
	
	app->entityManager->RequestHeroStats(stats, this->type, stats.heroLevel + 1);


	stats.maxHP *= app->entityManager->robottoLifeUpgradeValue;

	stats.maxEnergy *= (app->entityManager->robottoEnergyUpgradeValue);

	stats.damage *= (app->entityManager->robottoDamageUpgradeValue);
	stats.atkSpeed *= (app->entityManager->robottoAtkSpeedUpgradeValue);

	heroSkillPoints++;
}

void RoboHero::PlayGenericNoise(int probability)
{

}
