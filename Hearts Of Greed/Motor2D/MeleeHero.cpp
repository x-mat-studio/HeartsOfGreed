#include "MeleeHero.h"
#include "EntityManager.h"
#include "Map.h"
#include "Map.h"
#include "Render.h"
#include "ParticleSystem.h"

MeleeHero::MeleeHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown,
	Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, Animation& tileOnWalk,
	HeroStats& stats,  Skill& skill1) :
	
	Hero(position, ENTITY_TYPE::HERO_MELEE, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, 
		tileOnWalk, stats,  skill1)

{}


MeleeHero::MeleeHero(fMPoint position, MeleeHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement)
{}


MeleeHero::~MeleeHero()
{
}


bool MeleeHero::ActivateSkill1(fMPoint clickPosition)
{

	inputs.push_back(IN_SKILL1);

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
	if(currAoE.size() == 0)
	{
		origin = app->map->WorldToMap(round(position.x), round(position.y));
		origin = app->map->MapToWorld(origin.x, origin.y);
		currAreaInfo = app->entityManager->RequestAreaInfo(skill1.rangeRadius);


		if (currAreaInfo != nullptr)
			app->entityManager->CreateDynamicArea(&this->currAoE, skill1.rangeRadius, origin, currAreaInfo);
	}

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

bool MeleeHero::ExecuteSkill1()
{

	if (!skillExecutionDelay)
	{
		if (!godMode)
			stats.currEnergy -= skill1.energyCost;

		skillExecutionDelay = true;

		app->audio->PlayFx(app->entityManager->suitman1Skill, 0, -1, this->GetMyLoudness(), this->GetMyDirection());

		return skillExecutionDelay;
	}
	else
	{
	
		int ret = 0;

		ret =  app->entityManager->ExecuteSkill(skill1, this->origin);
		app->audio->PlayFx(app->entityManager->armored1Skill2, 0, -1, this->GetMyLoudness(), this->GetMyDirection());

		currAoE.clear();
		suplAoE.clear();
		currAreaInfo = nullptr;

		if (ret > 0)
		{
			GetExperience(ret);
		}

		return true;
	}

}

bool MeleeHero::ExecuteSkill2()
{

	return true;
}

bool MeleeHero::ExecuteSkill3()
{

	return true;
}

void MeleeHero::LevelUp()
{
	//lvl up effect
	if (myParticleSystem != nullptr)
	myParticleSystem->Activate();
	else {
		myParticleSystem = (ParticleSystem*)app->entityManager->AddParticleSystem(TYPE_PARTICLE_SYSTEM::MAX, position.x, position.y);
	}
	hitPointsMax += (hpLevelUpConstant * app->entityManager->meleeLifeUpgradeValue);
	hitPointsCurrent = hitPointsMax;	
	recoveryHitPointsRate += 1;
	maxEnergyPoints += (energyLevelUpConstant * app->entityManager->gathererEnergyUpgradeValue);
	energyPoints = maxEnergyPoints;
	recoveryEnergyRate;

	app->entityManager->RequestHeroStats(stats, this->type, stats.heroLevel + 1);


	stats.maxHP *= app->entityManager->meleeLifeUpgradeValue;
	stats.maxEnergy *= (app->entityManager->meleeEnergyUpgradeValue);

	stats.damage *= (app->entityManager->meleeDamageUpgradeValue);
	stats.atkSpeed *= (app->entityManager->meleeAtkSpeedUpgradeValue);

	heroSkillPoints++;
}

void MeleeHero::PlayGenericNoise(int probability)
{
	int random = rand() % ((100 * 4) / probability) + 1;

	switch (random)
	{
	case 1:
		app->audio->PlayFx(app->entityManager->noise1Armored, 0, 5, this->GetMyLoudness(), this->GetMyDirection());
		break;
	case 2:
		app->audio->PlayFx(app->entityManager->noise2Armored, 0, 5, this->GetMyLoudness(), this->GetMyDirection());
		break;
	case 3:
		app->audio->PlayFx(app->entityManager->noise3Armored, 0, 5, this->GetMyLoudness(), this->GetMyDirection());
		break;
	case 4:
		app->audio->PlayFx(app->entityManager->noise4Armored, 0, 5, this->GetMyLoudness(), this->GetMyDirection());
		break;

	default:
		break;
	}

}

void MeleeHero::BlitCommandVfx(Frame& currframe, int alphaValue)
{
	app->render->Blit(app->entityManager->moveCommandTileMelee, movingTo.x, movingTo.y, &currframe.frame, false, true, alphaValue, 255, 255, 255, 1.0f, currframe.pivotPositionX, currframe.pivotPositionY);
}