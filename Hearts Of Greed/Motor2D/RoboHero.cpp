#include "RoboHero.h"
#include "EntityManager.h"
#include "Map.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "ParticleSystem.h"
#include "CameraShake.h"
#include "EventManager.h"

RoboHero::RoboHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown,
	Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown,
	Animation& tileOnWalk, HeroStats& stats, Skill& skill1, Skill& passiveSkill) :

	Hero(position, ENTITY_TYPE::HERO_ROBO, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown,
		tileOnWalk, stats, skill1),

	passiveSkill(passiveSkill),
	acumulations(0),
	timer(0),

	currentDamage(GetAttackDamage()),
	currentSpeed(GetSpeed())
{
}


RoboHero::RoboHero(fMPoint position, RoboHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement),

	passiveSkill(copy->passiveSkill),
	acumulations(0),
	timer(0),

	currentDamage(GetAttackDamage()),
	currentSpeed(GetSpeed())
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

	int ret = 0;

		UnleashParticlesSkill1((int)this->position.x, (int)this->position.y);
		

		ret = app->entityManager->ExecuteSkill(skill1, this->origin);
		app->cameraShake->StartCameraShake(1, 10);

	currAoE.clear();
	suplAoE.clear();
	currAreaInfo = nullptr;





	

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


void RoboHero::UpdatePasiveSkill(float dt)
{
	if (timer > 0)
		timer -= dt;

	else if (timer <= 0 && acumulations > 0)
	{
		ResetBuff();
		acumulations = 0;
	}
}


void RoboHero::Attack()
{
	int ret = -1;

	if (objective)
		ret = objective->RecieveDamage(stats.damage);

	if (ret > 0)
	{
		GetExperience(ret);

		if (acumulations < passiveSkill.attackRadius)
		{
			ResetBuff();
			acumulations++;
			ApplyBuff();
		}

		timer = passiveSkill.coolDown;
	}
}


void RoboHero::LevelUp()
{
	ResetBuff();

	//lvl up effect
	if (myParticleSystem != nullptr)
		myParticleSystem->Activate();

	else
	{
		myParticleSystem = (ParticleSystem*)app->entityManager->AddParticleSystem(TYPE_PARTICLE_SYSTEM::MAX, position.x, position.y);
	}

	stats.maxHP *= app->entityManager->robottoLifeUpgradeValue;
	stats.currHP = stats.maxHP;
	hitPointsCurrent = stats.maxHP;

	stats.maxEnergy *= (app->entityManager->robottoEnergyUpgradeValue);
	stats.currEnergy = stats.maxEnergy;

	stats.damage *= (app->entityManager->robottoDamageUpgradeValue);
	stats.atkSpeed *= (app->entityManager->robottoAtkSpeedUpgradeValue);

	app->entityManager->RequestHeroStats(stats, this->type, stats.heroLevel + 1);

	heroSkillPoints++;

	ApplyBuff();
}


void RoboHero::PlayGenericNoise(int probability)
{

}


void RoboHero::BlitCommandVfx(Frame& currframe, int alphaValue)
{
	iMPoint postoPrint = movingTo;

	if (objective != nullptr)
	{
		fMPoint enemyPos = objective->GetPosition();
		enemyPos = app->map->WorldToMap(enemyPos.x, enemyPos.y);
		enemyPos = app->map->MapToWorld(enemyPos.x, enemyPos.y);

		postoPrint = { (int)enemyPos.x, (int)enemyPos.y };
	}

	app->render->Blit(app->entityManager->moveCommandTileRobot, postoPrint.x, postoPrint.y, &currframe.frame, false, true, alphaValue, 255, 255, 255, 1.0f, currframe.pivotPositionX, currframe.pivotPositionY);
}

void RoboHero::UnleashParticlesSkill1(float posx, float posy)
{
	//fire spreading

	if (activeSkillsParticleSystem != nullptr)
	{
		activeSkillsParticleSystem->Activate();
		activeSkillsParticleSystem->Move(posx, posy);
	}
	else
	{
		activeSkillsParticleSystem = (ParticleSystem*)app->entityManager->AddParticleSystem(TYPE_PARTICLE_SYSTEM::SKILL_GATHERER, posx, posy);
																								//This is correct. Ask Adri
	}
}


//Call this before upgrading the passive skill
void RoboHero::ResetBuff()
{
	SetAttackDamage(currentDamage);
	SetSpeed(currentSpeed);
}


void RoboHero::ApplyBuff()
{
	currentDamage = GetAttackDamage();
	float dmg = currentDamage * passiveSkill.dmg * 0.01 * acumulations;		  //passiveSkill.dmg codifies the damage increment

	currentSpeed = GetSpeed();
	float spd = currentSpeed * passiveSkill.rangeRadius * 0.01 * acumulations;//passiveSkill.rangeRadius codifies the speed increment

	SetAttackDamage(currentDamage + dmg);
	SetSpeed(currentSpeed + spd);
}


Skill RoboHero::GetPassiveSkill() const
{
	return passiveSkill;
}


void RoboHero::ReplacePassiveSkill(Skill& skill)
{
	passiveSkill = skill;
}