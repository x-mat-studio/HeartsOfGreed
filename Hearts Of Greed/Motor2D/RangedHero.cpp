#include "RangedHero.h"
#include "Input.h"
#include "EntityManager.h"
#include "Render.h"
#include "Map.h"

RangedHero::RangedHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown, Animation& tileOnWalk,
	Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, 
	int level, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int maxEnergyPoints, int energyPoints, int recoveryEnergyRate,
	int attackDamage, int attackSpeed, int attackRange, int movementSpeed, int vision, float skill1ExecutionTime,
	float skill2ExecutionTime, float skill3ExecutionTime, float skill1RecoverTime, float skill2RecoverTime, float skill3RecoverTime,
	int skill1Dmg, SKILL_ID skill1Id, SKILL_TYPE skill1Type, ENTITY_ALIGNEMENT skill1Target) :

	Hero(position, ENTITY_TYPE::HERO_RANGED, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, 
		tileOnWalk, level, maxHitPoints, currentHitPoints, recoveryHitPointsRate, maxEnergyPoints, energyPoints, recoveryEnergyRate,
		attackDamage, attackSpeed, attackRange, movementSpeed, vision, skill1ExecutionTime, skill2ExecutionTime,
		skill3ExecutionTime, skill1RecoverTime, skill2RecoverTime, skill3RecoverTime,
		skill1Dmg, skill1Id, skill1Type, skill1Target),

	granadeArea(nullptr),

	currentVfx(nullptr),
	vfxExplosion(vfxExplosion),
	explosionRect{ 0,0,0,0 }
{}


RangedHero::RangedHero(fMPoint position, RangedHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement),

	granadeArea(nullptr),

	currentVfx(nullptr),
	vfxExplosion(copy->vfxExplosion),
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

		currAreaInfo = app->entityManager->RequestArea(skill1.id, &this->currAoE, this->origin);
	}

	iMPoint center = app->map->WorldToMap(position.x, position.y);
	granadePosLaunch = app->input->GetMousePosWorld();

	if (center.InsideCircle(app->map->WorldToMap(granadePosLaunch.x, granadePosLaunch.y), currAreaInfo->radius))
	{
		granadeArea = app->entityManager->RequestArea(SKILL_ID::RANGED_SKILL1_MOUSE, &this->suplAoE, { (int)granadePosLaunch.x, (int)granadePosLaunch.y });
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

	if (granadeArea)
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

			ret = app->entityManager->ExecuteSkill(skill1.dmg, { (int)granadePosLaunch.x, (int)granadePosLaunch.y }, this->granadeArea, skill1.target, skill1.type, true, (Entity*)this);

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
	
	hitPointsMax;
	hitPointsCurrent; 
	recoveryHitPointsRate;
	energyPoints;
	recoveryEnergyRate;

	attackDamage;
	attackSpeed;
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

		app->render->Blit(app->entityManager->explosionTexture, granadePosLaunch.x - currFrame.pivotPositionX, granadePosLaunch.y - currFrame.pivotPositionY, &currFrame.frame);
	}


	return false;
}

void RangedHero::BlitCommandVfx(Frame& currframe, int alphaValue)
{
	app->render->Blit(app->entityManager->moveCommandTileRng, movingTo.x - currframe.pivotPositionX, movingTo.y - currframe.pivotPositionY, &currframe.frame, false, true, alphaValue);
}