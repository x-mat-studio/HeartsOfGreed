#include "RangedHero.h"
#include "Input.h"
#include "EntityManager.h"
#include "Render.h"
#include "Map.h"
#include "ParticleSystem.h"
#include "Player.h"

#include "Enemy.h"

RangedHero::RangedHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown, 
	Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, Animation& tileOnWalk,
	HeroStats& stats, Skill& skill1, Skill& passiveSkill) :

	Hero(position, ENTITY_TYPE::HERO_RANGED, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, 
		tileOnWalk, stats, skill1),

	skill1Area(nullptr),
	currentVfx(nullptr),

	passiveSkill(passiveSkill),

	explosionRect{ 0,0,0,0 }
{}


RangedHero::RangedHero(fMPoint position, RangedHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement),

	skill1Area(nullptr),
	currentVfx(nullptr),

	passiveSkill(copy->passiveSkill),

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


		if (currAreaInfo != nullptr)
			app->entityManager->CreateDynamicArea(&this->currAoE, skill1.rangeRadius, origin, currAreaInfo);
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
				stats.currEnergy -= skill1.energyCost;


			skillExecutionDelay = true;
			ExecuteSFX(app->entityManager->suitman1Skill);
			return skillExecutionDelay;
		}
		else
		{

			int ret = 0;

			ExecuteSFX(app->entityManager->ranged1Skill);
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


void RangedHero::Attack()
{
	int ret = -1;
	ENTITY_TYPE type = objective->GetType();
	Enemy* enemy = nullptr;;

	if (objective)
		ret = objective->RecieveDamage(stats.damage);

	if (ret > 0)
	{
		GetExperience(ret);
	}

	else
	{
		if (type == ENTITY_TYPE::ENEMY || type == ENTITY_TYPE::ENEMY_GIGA || type == ENTITY_TYPE::ENEMY_NIGHT || type == ENTITY_TYPE::ENEMY_RANGED)
		{
			enemy = (Enemy*)objective;

			enemy->debuffs.AddNewEffect(passiveSkill.effect, passiveSkill.executionTime, passiveSkill.dmg);
		}
	}
}


void RangedHero::LevelUp()
{
	//lvl up effect
	if (myParticleSystem != nullptr)
	myParticleSystem->Activate();
	else {
		myParticleSystem = (ParticleSystem*)app->entityManager->AddParticleSystem(TYPE_PARTICLE_SYSTEM::MAX, position.x, position.y);
	}

	app->entityManager->RequestHeroStats(stats, this->type, stats.heroLevel + 1);

	stats.maxHP *= app->entityManager->rangedLifeUpgradeValue;
	stats.currHP = stats.maxHP;

	stats.maxEnergy *= (app->entityManager->rangedEnergyUpgradeValue);
	stats.currEnergy = stats.maxEnergy;

	stats.damage *= (app->entityManager->rangedDamageUpgradeValue);
	stats.atkSpeed *= (app->entityManager->rangedAtkSpeedUpgradeValue);

	heroSkillPoints++;
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
	iMPoint postoPrint = movingTo;

	if (objective != nullptr)
	{
		fMPoint enemyPos = objective->GetPosition();
		enemyPos = app->map->WorldToMap(enemyPos.x, enemyPos.y);
		enemyPos = app->map->MapToWorld(enemyPos.x, enemyPos.y);

		postoPrint = { (int)enemyPos.x, (int)enemyPos.y };
	}

	app->render->Blit(app->entityManager->moveCommandTileRng, postoPrint.x, postoPrint.y, &currframe.frame, false, true, alphaValue, 255, 255, 255, 1.0f, currframe.pivotPositionX, currframe.pivotPositionY);
}

void RangedHero::PlayGenericNoise(int probability)
{
	int random = rand() % ((100 * 4) / probability) + 1;

	switch (random)
	{
	case 1:
		ExecuteSFX(app->entityManager->noise1Ranged);
		break;
	case 2:
		ExecuteSFX(app->entityManager->noise2Ranged);
		break;
	case 3:
		ExecuteSFX(app->entityManager->noise3Ranged);
		break;
	case 4:
		ExecuteSFX(app->entityManager->noise4Ranged);
		break;

	default:
		break;
	}

}


void RangedHero::PlayOnHitSound()
{
	ExecuteSFX(app->entityManager->rangedGetsHit);
}


Skill RangedHero::GetPassiveSkill() const
{
	return passiveSkill;
}


void RangedHero::ReplacePassiveSkill(Skill& skill)
{
	passiveSkill = skill;
}