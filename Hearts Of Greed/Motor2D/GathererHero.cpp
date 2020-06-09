#include "GathererHero.h"

#include "EntityManager.h"
#include "Map.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "ParticleSystem.h"
#include "Player.h"
#include "Collision.h"
#include "CameraShake.h"
#include "Turret.h"

GathererHero::GathererHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
	Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
	Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
	Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
	Animation& skill1LeftUp, Animation& skill1LeftDown, Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown,
	Animation& tileOnWalk, HeroStats& stats, Skill& skill1, Skill& passiveSkill) :

	Hero(position, ENTITY_TYPE::HERO_GATHERER, col, walkLeft, walkLeftUp, walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightDown,
		idleRightUp, idleLeft, idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp,
		punchRightDown, punchRight, skill1Right, skill1RightUp, skill1RightDown, skill1Left,
		skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown,
		tileOnWalk, stats, skill1),

	granadeArea(nullptr),
	passiveSkillCollider(nullptr),
	myTurret(nullptr),

	passiveSkill(passiveSkill)

{}


GathererHero::GathererHero(fMPoint position, GathererHero* copy, ENTITY_ALIGNEMENT alignement) :

	Hero(position, copy, alignement),

	granadeArea(nullptr),
	passiveSkillCollider(nullptr),
	myTurret(nullptr),

	passiveSkill(copy->passiveSkill)

{}

GathererHero::~GathererHero()
{
	granadeArea = nullptr;

	if (passiveSkillCollider != nullptr)
	{
		passiveSkillCollider->to_delete = true;
		passiveSkillCollider->thisEntity = nullptr;
		passiveSkillCollider = nullptr;
	}
}


bool GathererHero::Start(SDL_Texture* texture)
{
	this->texture = texture;
	if (collider != nullptr)
	{
		collider = new Collider(collider->rect, collider->type, collider->callback, this);
		collider->thisEntity = this;
		app->coll->AddColliderEntity(collider);

		collider->SetPos(position.x, position.y);

		offset.x = -((float)collider->rect.w * 0.5f);

		offset.y = -((float)collider->rect.h * 0.66f);

		center.x = (float)collider->rect.w * 0.5f;
		center.y = (float)collider->rect.h * 0.5f;

		CollisionPosUpdate();
	}

	passiveSkillCollider = new Collider(SDL_Rect{ 0, 0, passiveSkill.rangeRadius, passiveSkill.rangeRadius }, COLLIDER_PASSIVE_GATHERER, app->entityManager, this);
	app->coll->AddColliderEntity(passiveSkillCollider);

	started = true;

	return true;
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
				stats.currEnergy -= skill1.energyCost;


			skillExecutionDelay = true;
			ExecuteSFX(app->entityManager->suitman1Skill);

			return skillExecutionDelay;
		}
		else
		{

			ExecuteSFX(app->entityManager->suitman1Skill2);


			int ret = 0;

			//ret = app->entityManager->ExecuteSkill(skill1, { (int)granadePosLaunch.x, (int)granadePosLaunch.y }, (Entity*)this);
			//UnleashParticlesSkill1((int)granadePosLaunch.x, (int)granadePosLaunch.y);
			//app->cameraShake->StartCameraShake(1.2, 7);

			if (myTurret != nullptr)
			{
				myTurret->SetTemporalTimer(0.01f);
				myTurret = nullptr;
			}

			myTurret = (Turret*)app->entityManager->AddEntity(ENTITY_TYPE::BLDG_TURRET, granadePosLaunch.x, granadePosLaunch.y, ENTITY_ALIGNEMENT::PLAYER);

			//Here we can do funny stuff to the turret
			myTurret->SetLevel(skill1.lvl + 1);
			myTurret->SetTemporalTimer(skill1.dmg);

			currAoE.clear();
			suplAoE.clear();
			currAreaInfo = nullptr;

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


void GathererHero::UpdatePasiveSkill(float dt)
{
	if (started == true)
	{
		passiveSkillCollider->SetPos(position.x - passiveSkill.rangeRadius * 0.5, position.y - passiveSkill.rangeRadius * 0.5);
	}
}


void GathererHero::Attack()
{
	int ret = -1;

	if (objective)
		ret = objective->RecieveDamage(stats.damage);

	if (ret > 0)
	{
		GetExperience(ret);

		if (this->type == ENTITY_TYPE::HERO_GATHERER && app->player != nullptr) {

			app->player->AddResources(ret * 0.5f);
			app->player->AddResources(passiveSkill.dmg); //dmg codifies the extra resources gained when killing an alien
		}
		true;
	}
}

void GathererHero::LevelUp()
{
	//lvl up effect
	if (myParticleSystem != nullptr)
		myParticleSystem->Activate();
	else
	{
		myParticleSystem = (ParticleSystem*)app->entityManager->AddParticleSystem(TYPE_PARTICLE_SYSTEM::MAX, position.x, position.y);
	}

	app->entityManager->RequestHeroStats(stats, this->type, stats.heroLevel + 1);

	stats.maxHP *= app->entityManager->gathererLifeUpgradeValue;
	stats.currHP = stats.maxHP;
	hitPointsCurrent = stats.maxHP;

	stats.maxEnergy *= (app->entityManager->gathererEnergyUpgradeValue);
	stats.currEnergy = stats.maxEnergy;

	stats.damage *= (app->entityManager->gathererDamageUpgradeValue);
	stats.atkSpeed *= (app->entityManager->gathererAtkSpeedUpgradeValue);

	heroSkillPoints++;
}

void GathererHero::UnleashParticlesSkill1(float posx, float posy)
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
	}
}

void GathererHero::OnCollision(Collider* collider)
{
	if (collider->type == COLLIDER_HERO)
	{
		if (collider->thisEntity != nullptr)
		{
			if (collider->thisEntity->GetType() == ENTITY_TYPE::HERO_ROBO)
			{
				Hero* hero = (Hero*)collider->thisEntity;

				hero->bonusArmor = passiveSkill.executionTime; //Execution time codifies as the bonuses the robotic units receibe
				hero->bonusAttack = passiveSkill.executionTime;
			}
		}
	}

	else if (collider->type == COLLIDER_VISIBILITY)
	{
		if (collider->thisEntity != nullptr)
		{
			if (collider->thisEntity->GetType() == ENTITY_TYPE::BLDG_TURRET)
			{
				Turret* turret = (Turret*)collider->thisEntity;

				turret->bonusArmor = passiveSkill.executionTime;
				turret->bonusAttack = passiveSkill.executionTime;
			}
		}
	}
}


void GathererHero::PlayGenericNoise(int probability)
{
	int random = rand() % ((100 * 4) / probability) + 1;

	switch (random)
	{
	case 1:
		ExecuteSFX(app->entityManager->noise1Suitman);
		break;
	case 2:
		ExecuteSFX(app->entityManager->noise2Suitman);
		break;
	case 3:
		ExecuteSFX(app->entityManager->noise3Suitman);
		break;
	case 4:
		ExecuteSFX(app->entityManager->noise4Suitman);
		break;

	default:
		break;
	}
}

void GathererHero::BlitCommandVfx(Frame& currframe, int alphaValue)
{
	iMPoint postoPrint = movingTo;

	if (objective != nullptr)
	{
		fMPoint enemyPos = objective->GetPosition();
		enemyPos = app->map->WorldToMap(enemyPos.x, enemyPos.y);
		enemyPos = app->map->MapToWorld(enemyPos.x, enemyPos.y);

		postoPrint = { (int)enemyPos.x, (int)enemyPos.y };
	}

	app->render->Blit(app->entityManager->moveCommandTileGath, postoPrint.x, postoPrint.y, &currframe.frame, false, true, alphaValue, 255, 255, 255, 1.0f, currframe.pivotPositionX, currframe.pivotPositionY);
}


Skill GathererHero::GetPassiveSkill() const
{
	return passiveSkill;
}


void GathererHero::ReplacePassiveSkill(Skill& skill)
{
	passiveSkill = skill;
}

void GathererHero::CheckObjective(Entity* deleted)
{
	if (objective == deleted)
	{
		path.clear();
		objective = nullptr;
		SearchForNewObjective();

		inputs.push_back(HERO_INPUTS::IN_MOVE);
	}

	if (myTurret == deleted)
	{
		myTurret = nullptr;
		cooldownHability1 += TIME_TRIGGER;
		skill1Charged = false;
	}

}


