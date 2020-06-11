#include "MeleeHero.h"
#include "EntityManager.h"
#include "Map.h"
#include "Map.h"
#include "Render.h"
#include "ParticleSystem.h"
#include "CameraShake.h"
#include "EventManager.h"

MeleeHero::MeleeHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown,
	Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, Animation& tileOnWalk,
	HeroStats& stats,  Skill& skill1, Skill& passiveSkill) :
	
	Hero(position, ENTITY_TYPE::HERO_MELEE, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, 
		tileOnWalk, stats,  skill1),

	passiveSkill(passiveSkill),
	passiveSkillCollider(nullptr),

	armorGained(0)

{}


MeleeHero::MeleeHero(fMPoint position, MeleeHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement),

	passiveSkill(copy->passiveSkill),
	passiveSkillCollider(nullptr),

	armorGained(0)
{}


MeleeHero::~MeleeHero()
{
	if (passiveSkillCollider != nullptr)
	{
		passiveSkillCollider->to_delete = true;
		passiveSkillCollider->thisEntity = nullptr;
		passiveSkillCollider = nullptr;
	}
}


bool MeleeHero::Start(SDL_Texture* texture)
{
	this->texture = texture;
	if (collider != nullptr)
	{
		collider = new Collider(collider->rect, collider->type, collider->callback, this);
		collider->thisEntity = this;
		app->coll->AddColliderEntity(collider);

		collider->SetPos(position.x, position.y);

		offset.x = -((float)collider->rect.w * 0.5f);

		offset.y = -((float)collider->rect.h * 0.82f);

		center.x = (float)collider->rect.w * 0.5f;
		center.y = (float)collider->rect.h * 0.5f;

		CollisionPosUpdate();
	}

	passiveSkillCollider = new Collider(SDL_Rect{ 0, 0, passiveSkill.rangeRadius, passiveSkill.rangeRadius }, COLLIDER_PASSIVE_MELEE, app->entityManager, this);
	app->coll->AddColliderEntity(passiveSkillCollider);

	started = true;

	return true;
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

		ExecuteSFX(app->entityManager->suitman1Skill);

		return skillExecutionDelay;
	}
	else
	{
	
		int ret = 0;

		ret =  app->entityManager->ExecuteSkill(skill1, this->origin);

		if (skill1.hurtYourself == true)
		{
			this->RecieveDamage(skill1.dmg, true);
		}

		ExecuteSFX(app->entityManager->armored1Skill2);
		app->cameraShake->StartCameraShake(0.8, 8);
		UnleashParticlesSkill1(position.x, position.y);

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


void MeleeHero::UpdatePasiveSkill(float dt)
{
	if (started == true)
	{
		if (gettingAttacked == false)
		{
			RecoverHealth(dt * passiveSkill.coolDown); //Cooldown refers to extra passive regeneration
		}

		bonusArmor = armorGained;
		armorGained = 0;
		passiveSkillCollider->SetPos(position.x - passiveSkill.rangeRadius * 0.5, position.y - passiveSkill.rangeRadius * 0.5);
	}
	
}


void MeleeHero::LevelUp()
{
	//lvl up effect
	if (myParticleSystem != nullptr)
		myParticleSystem->Activate();

	else 
	{
		myParticleSystem = (ParticleSystem*)app->entityManager->AddParticleSystem(TYPE_PARTICLE_SYSTEM::MAX, position.x, position.y);
	}

	app->entityManager->RequestHeroStats(stats, this->type, stats.heroLevel + 1);

	stats.maxHP *= app->entityManager->meleeLifeUpgradeValue;
	stats.currHP = stats.maxHP;
	hitPointsCurrent = stats.maxHP;
	
	stats.maxEnergy *= (app->entityManager->meleeEnergyUpgradeValue);
	stats.currEnergy = stats.maxEnergy;

	stats.damage *= (app->entityManager->meleeDamageUpgradeValue);
	stats.atkSpeed *= (app->entityManager->meleeAtkSpeedUpgradeValue);

	heroSkillPoints++;
	app->eventManager->GenerateEvent(EVENT_ENUM::HERO_LEVELED_UP, EVENT_ENUM::NULL_EVENT);
}


void MeleeHero::OnCollision(Collider* collider)
{
	if (collider->type == COLLIDER_ENEMY)	
	{
		if (armorGained < passiveSkill.energyCost) //max accumulable armor
		{
			armorGained += passiveSkill.dmg; //armor per enemy near
		}
		
	}
}


void MeleeHero::PlayGenericNoise(int probability)
{
	int random = rand() % ((100 * 4) / probability) + 1;

	switch (random)
	{
	case 1:
		ExecuteSFX(app->entityManager->noise1Armored);
		break;
	case 2:
		ExecuteSFX(app->entityManager->noise2Armored);
		break;
	case 3:
		ExecuteSFX(app->entityManager->noise3Armored);
		break;
	case 4:
		ExecuteSFX(app->entityManager->noise4Armored);
		break;

	default:
		break;
	}

}

void MeleeHero::BlitCommandVfx(Frame& currframe, int alphaValue)
{
	iMPoint postoPrint = movingTo;

	if (objective != nullptr)
	{
		fMPoint enemyPos = objective->GetPosition();
		enemyPos = app->map->WorldToMap(enemyPos.x, enemyPos.y);
		enemyPos = app->map->MapToWorld(enemyPos.x, enemyPos.y);

		postoPrint = { (int)enemyPos.x, (int)enemyPos.y };
	}

	app->render->Blit(app->entityManager->moveCommandTileMelee, postoPrint.x, postoPrint.y, &currframe.frame, false, true, alphaValue, 255, 255, 255, 1.0f, currframe.pivotPositionX, currframe.pivotPositionY);
}


Skill MeleeHero::GetPassiveSkill() const
{
	return passiveSkill;
}


void MeleeHero::ReplacePassiveSkill(Skill& skill)
{
	passiveSkill = skill;
}

void MeleeHero::UnleashParticlesSkill1(float posx, float posy)
{
	if (activeSkillsParticleSystem != nullptr)
	{
		activeSkillsParticleSystem->Activate();
		activeSkillsParticleSystem->Move(posx, posy);
	}
	else
	{
		activeSkillsParticleSystem = (ParticleSystem*)app->entityManager->AddParticleSystem(TYPE_PARTICLE_SYSTEM::SKILL_MELEE, this->position.x, this->position.y);
		//This is correct. Ask Adri
	}
}
