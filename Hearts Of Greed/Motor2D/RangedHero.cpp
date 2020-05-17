#include "RangedHero.h"
#include "Input.h"
#include "EntityManager.h"
#include "Render.h"
#include "Map.h"

RangedHero::RangedHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown, 
	Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, Animation& tileOnWalk,
	int level, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int maxEnergyPoints, int energyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, Skill& skill1, int hpLevelUp, int damageLevelUp, int energyLevelUp, int atkSpeedLevelUp) :

	Hero(position, ENTITY_TYPE::HERO_RANGED, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, 
		tileOnWalk, level, maxHitPoints, currentHitPoints, recoveryHitPointsRate, maxEnergyPoints, energyPoints, recoveryEnergyRate,
		attackDamage, attackSpeed, attackRange, movementSpeed, vision, skill1, hpLevelUp, damageLevelUp, energyLevelUp, atkSpeedLevelUp),

	skill1Area(nullptr),

	currentVfx(nullptr),
	explosionRect{ 0,0,0,0 }
{}


RangedHero::RangedHero(fMPoint position, RangedHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement),

	skill1Area(nullptr),

	currentVfx(nullptr),
	explosionRect{ 0,0,0,0 }
{}


bool RangedHero::ActivateSkill1(fMPoint clickPosition)
{

	inputs.push_back(IN_SKILL1);

	return true;
}

bool RangedHero::ActivateSkill2()
{

	return true;
}

bool RangedHero::ActivateSkill3()
{

	return true;
}

bool RangedHero::PreProcessSkill1()
{
	if (currAoE.size() == 0)
	{
		origin = app->map->WorldToMap(round(position.x), round(position.y));
		origin = app->map->MapToWorld(origin.x, origin.y);

		currAreaInfo = app->entityManager->RequestAreaInfo(skill1.rangeRadius);
	}

	iMPoint center = app->map->WorldToMap(position.x, position.y);
	skill1PosLaunch = app->input->GetMousePosWorld();

	if (center.InsideCircle(app->map->WorldToMap(skill1PosLaunch.x, skill1PosLaunch.y), skill1.rangeRadius))
	{
		skill1Area = app->entityManager->RequestAreaInfo(skill1.attackRadius);

		app->entityManager->CreateDynamicArea(&this->suplAoE, skill1.attackRadius, { (int)skill1PosLaunch.x, (int)skill1PosLaunch.y }, skill1Area);
	}

	return true;
}

bool RangedHero::PreProcessSkill2()
{

	return true;
}

bool RangedHero::PreProcessSkill3()
{

	return true;
}

bool RangedHero::ExecuteSkill1()
{

	if (skill1Area)
	{
		if (!skillExecutionDelay)
		{
			if (!godMode)
				energyPoints -= skill1Cost;


			skillExecutionDelay = true;
			app->audio->PlayFx(app->entityManager->suitman1Skill, 0, -1, this->GetMyLoudness(), this->GetMyDirection());
			return skillExecutionDelay;
		}
		else
		{

			int ret = 0;

			app->audio->PlayFx(app->entityManager->ranged1Skill, 0, -1, this->GetMyLoudness(), this->GetMyDirection());
			ret = app->entityManager->ExecuteSkill(skill1, { (int)skill1PosLaunch.x, (int)skill1PosLaunch.y });

			currAoE.clear();
			suplAoE.clear();
			currAreaInfo = nullptr;

			if (ret >= 0)
			{
				GetExperience(ret);
				return true;
			}
		}
	}
	else
		return false;

}

bool RangedHero::ExecuteSkill2()
{

	return true;
}

bool RangedHero::ExecuteSkill3()
{

	return true;
}

void RangedHero::LevelUp()
{
	
	hitPointsMax += (hpLevelUpConstant * app->entityManager->rangedLifeUpgradeValue);		// Those variables that serve as values are on XML, because maths were supposed to avoid me going to the XML, but I had to anyway, but I'm too prideful to not make use of my maths, so I'm going to the XML, but less, which is a plus
	hitPointsCurrent = hitPointsMax; 
	recoveryHitPointsRate;
	energyPoints += (energyLevelUpConstant * app->entityManager->rangedEnergyUpgradeValue);
	recoveryEnergyRate;

	attackDamage += (damageLevelUpConstant * app->entityManager->rangedDamageUpgradeValue);
	attackSpeed += (attackSpeedLevelUpConstant * app->entityManager->rangedAtkSpeedUpgradeValue);
	attackRange;

	unitSpeed;
	visionDistance;

}


bool RangedHero::DrawVfx(float dt)
{
	if (currentVfx == nullptr)
		return false;
	else
	{
		Frame currFrame = currentVfx->GetCurrentFrame(dt);
		if (currentVfx->GetCurrentFrameNum() == currFrame.maxFrames)
			currentVfx = false;

		app->render->Blit(app->entityManager->explosionTexture, skill1PosLaunch.x, skill1PosLaunch.y, &currFrame.frame, false, true, 0, 255, 255 ,255, 1.0f, currFrame.pivotPositionX, currFrame.pivotPositionY);
	}


	return false;
}

void RangedHero::BlitCommandVfx(Frame& currframe, int alphaValue)
{
	app->render->Blit(app->entityManager->moveCommandTileRng, movingTo.x, movingTo.y, &currframe.frame, false, true, alphaValue, 255, 255, 255, 1.0f, currframe.pivotPositionX, currframe.pivotPositionY);
}

void RangedHero::PlayGenericNoise(int probability)
{
	int random = rand() % ((100 * 4) / probability) + 1;

	switch (random)
	{
	case 1:
		app->audio->PlayFx(app->entityManager->noise1Ranged, 0, 5, this->GetMyLoudness(), this->GetMyDirection());
		break;
	case 2:
		app->audio->PlayFx(app->entityManager->noise2Ranged, 0, 5, this->GetMyLoudness(), this->GetMyDirection());
		break;
	case 3:
		app->audio->PlayFx(app->entityManager->noise3Ranged, 0, 5, this->GetMyLoudness(), this->GetMyDirection());
		break;
	case 4:
		app->audio->PlayFx(app->entityManager->noise4Ranged, 0, 5, this->GetMyLoudness(), this->GetMyDirection());
		break;

	default:
		break;
	}

}

void RangedHero::PlayOnHitSound()
{
	app->audio->PlayFx(app->entityManager->rangedGetsHit, 0, -1, this->GetMyLoudness(), this->GetMyDirection(), true);
}