#include "GathererHero.h"
#include "EntityManager.h"
#include "Map.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

GathererHero::GathererHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown, Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown,
	Animation& tileOnWalk, int level, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int maxEnergyPoints, int energyPoints, int recoveryEnergyRate,
	int attackDamage, float attackSpeed, int attackRange, int movementSpeed, int vision, Skill& skill1, Animation& vfxExplosion, int hpLevelUp, int damageLevelUp, int energyLevelUp, int atkSpeedLevelUp) :

	Hero(position, ENTITY_TYPE::HERO_GATHERER, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown,
		tileOnWalk, level, maxHitPoints, currentHitPoints, recoveryHitPointsRate, maxEnergyPoints, energyPoints, recoveryEnergyRate,
		attackDamage, attackSpeed, attackRange, movementSpeed, vision, skill1, hpLevelUp, damageLevelUp, energyLevelUp, atkSpeedLevelUp),

	granadeArea(nullptr),

	vfxExplosion(vfxExplosion),
	currentVfx(nullptr),
	explosionRect{ 0,0,0,0 }
{}


GathererHero::GathererHero(fMPoint position, GathererHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement),
	granadeArea(nullptr),

	currentVfx(nullptr),
	vfxExplosion(copy->vfxExplosion),
	explosionRect{ 0,0,0,0 }
{}

GathererHero::~GathererHero()
{
	vfxExplosion = Animation();

	currentVfx = nullptr;

	granadeArea = nullptr;
	currentVfx = nullptr;
}

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
		origin = app->map->WorldToMap(round(position.x), round(position.y));
		origin = app->map->MapToWorld(origin.x, origin.y);

		currAreaInfo = app->entityManager->RequestAreaInfo(skill1.rangeRadius);

		if (currAreaInfo != nullptr)
			app->entityManager->CreateDynamicArea(&this->currAoE, skill1.rangeRadius, origin, currAreaInfo);
	}

	iMPoint center = app->map->WorldToMap(position.x, position.y);
	granadePosLaunch = app->input->GetMousePosWorld();

	if (center.InsideCircle(app->map->WorldToMap(granadePosLaunch.x, granadePosLaunch.y), skill1.rangeRadius))
	{
		granadeArea = app->entityManager->RequestAreaInfo(skill1.attackRadius);

		app->entityManager->CreateDynamicArea(&this->suplAoE, skill1.attackRadius, { (int)granadePosLaunch.x, (int)granadePosLaunch.y }, granadeArea);
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
	if (granadeArea != nullptr)
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
			currentVfx = &vfxExplosion;
			currentVfx->ResetAnimation();
			currentVfx->loop = false;

			app->audio->PlayFx(app->entityManager->suitman1Skill2, 0, -1, this->GetMyLoudness(), this->GetMyDirection());


			int ret = 0;

			ret = app->entityManager->ExecuteSkill(skill1, { (int)granadePosLaunch.x, (int)granadePosLaunch.y }, (Entity*)this);

			currAoE.clear();
			suplAoE.clear();
			currAreaInfo = nullptr;

			if (ret >= 0)
			{
				GetExperience(ret);
			}
			return true;

		}
	}
	else
		return false;

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

	hitPointsMax += (hpLevelUpConstant * app->entityManager->gathererLifeUpgradeValue);
	hitPointsCurrent = hitPointsMax;
	recoveryHitPointsRate;
	energyPoints += (energyLevelUpConstant * app->entityManager->gathererEnergyUpgradeValue);
	recoveryEnergyRate += 1;

	attackDamage += (damageLevelUpConstant * app->entityManager->gathererDamageUpgradeValue);
	attackSpeed += (attackSpeedLevelUpConstant * app->entityManager->gathererAtkSpeedUpgradeValue);
	attackRange;

	unitSpeed += 6;
	visionDistance;

}

void GathererHero::PlayGenericNoise(int probability)
{
	int random = rand() % ((100 * 4) / probability) + 1;

	switch (random)
	{
	case 1:
		app->audio->PlayFx(app->entityManager->noise1Suitman, 0, 4, this->GetMyLoudness(), this->GetMyDirection());
		break;
	case 2:
		app->audio->PlayFx(app->entityManager->noise2Suitman, 0, 4, this->GetMyLoudness(), this->GetMyDirection());
		break;
	case 3:
		app->audio->PlayFx(app->entityManager->noise3Suitman, 0, 4, this->GetMyLoudness(), this->GetMyDirection());
		break;
	case 4:
		app->audio->PlayFx(app->entityManager->noise4Suitman, 0, 4, this->GetMyLoudness(), this->GetMyDirection());
		break;

	default:
		break;
	}
}

bool GathererHero::DrawVfx(float dt)
{
	if (currentVfx == nullptr)
		return false;
	else
	{
		Frame currFrame = currentVfx->GetCurrentFrame(dt);

		if (currentVfx->GetCurrentFrameNum() == currentVfx->lastFrame - 1)
		{
			currentVfx = nullptr;
			drawingVfx = false;
		}

		app->render->Blit(app->entityManager->explosionTexture, granadePosLaunch.x, granadePosLaunch.y, &currFrame.frame, false, true, 0, 255, 255, 255, 1.0f, currFrame.pivotPositionX, currFrame.pivotPositionY);
	}


	return false;
}

void GathererHero::BlitCommandVfx(Frame& currframe, int alphaValue)
{
	app->render->Blit(app->entityManager->moveCommandTileGath, movingTo.x, movingTo.y, &currframe.frame, false, true, alphaValue, 255, 255, 255, 1.0f, currframe.pivotPositionX, currframe.pivotPositionY);
}