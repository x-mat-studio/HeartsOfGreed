#include "MeleeHero.h"
#include "EntityManager.h"
#include "Map.h"

MeleeHero::MeleeHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown, int level, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int maxEnergyPoints, int energyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime,
	int skill1Dmg, SKILL_ID skill1Id, SKILL_TYPE skill1Type, ENTITY_ALIGNEMENT skill1Target) :
	
	Hero(position, ENTITY_TYPE::HERO_MELEE, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, level, maxHitPoints, currentHitPoints, recoveryHitPointsRate, maxEnergyPoints, energyPoints, recoveryEnergyRate,
		attackDamage, attackSpeed, attackRange, movementSpeed, vision, skill1ExecutionTime, skill2ExecutionTime,
		skill3ExecutionTime, skill1RecoverTime, skill2RecoverTime, skill3RecoverTime,
		skill1Dmg, skill1Id, skill1Type, skill1Target)
{}


MeleeHero::MeleeHero(fMPoint position, MeleeHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement)
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
		currAreaInfo = app->entityManager->RequestArea(skill1.id, &this->currAoE, this->origin);
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
		energyPoints -= skill1Cost;

		skillExecutionDelay = true;
		app->audio->PlayFx(app->entityManager->armored1Skill2, 0, 1, this->GetMyLoudness(), this->GetMyDirection());

		app->audio->PlayFx(app->entityManager->suitman1Skill, 0, 2, this->GetMyLoudness(), this->GetMyDirection());
		return skillExecutionDelay;
	}
	else
	{
		
		return app->entityManager->ExecuteSkill(skill1.dmg, this->origin, this->currAreaInfo, skill1.target, skill1.type);
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

	hitPointsMax += 15;
	hitPointsCurrent = hitPointsMax;		
	recoveryHitPointsRate += 1;
	energyPoints += 5;
	recoveryEnergyRate;

	attackDamage += 3;
	attackSpeed;
	attackRange;

	unitSpeed += 5;
	visionDistance;

	app->audio->PlayFx(app->entityManager->lvlup,0,-1,LOUDNESS::LOUD,DIRECTION::FRONT);
}

void MeleeHero::PlayGenericNoise()
{
	
	int random = rand() % 15 +1; 

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
