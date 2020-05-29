#include "Base.h"
#include "Turret.h"
#include "Barricade.h"
#include "UpgradeCenter.h"
#include "Building.h"

#include "Collision.h"
#include "EntityManager.h"
#include "Player.h"
#include "FoWManager.h"
#include "EventManager.h"
#include "App.h"


Base::Base(fMPoint position, Collider* collider, int maxTurrets, int maxBarricades, UpgradeCenter* baseUpgradeCenter, std::vector <Turret*> baseTurrets,  
	       std::vector <Barricade*> baseBarricades,Collider* baseArea, int resourcesProduced, float resourcesRate, int maxHitPoints, int currentHitPoints,
	       int recoveryHitPointsRate, int transparency) :

	Building(position, maxHitPoints, currentHitPoints, recoveryHitPointsRate, 0, 0, transparency, collider, ENTITY_TYPE::BLDG_BASE),

	maxTurrets(maxTurrets),
	maxBarricades(maxBarricades),

	resourcesProduced(resourcesProduced),
	resourcesRate(resourcesRate),
	resourcesCooldown(0),

	baseUpgradeCenter(baseUpgradeCenter),
	turretsVector(baseTurrets),
	barricadesVector(baseBarricades),

	baseAreaAlarm(baseArea)
	
{}


Base::Base(fMPoint position, Collider* collider, int maxTurrets, int maxBarricades, UpgradeCenter* baseUpgradeCenter, Collider* baseArea, int resourcesProduced, float resourcesRate,
	int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int transparency):

	Building(position, maxHitPoints, currentHitPoints, recoveryHitPointsRate, 0, 0, transparency, collider, ENTITY_TYPE::BLDG_BASE),

	maxTurrets(maxTurrets),
	maxBarricades(maxBarricades),

	resourcesProduced(resourcesProduced),
	resourcesRate(resourcesRate),
	resourcesCooldown(0),

	baseUpgradeCenter(baseUpgradeCenter),

	baseAreaAlarm(baseArea)

{}


Base::Base(fMPoint position, Base* copy, ENTITY_ALIGNEMENT alignement) :

	Building(position, copy, alignement),

	maxTurrets(copy->maxTurrets),
	maxBarricades(copy->maxBarricades),
	
	resourcesProduced(copy->resourcesProduced),
	resourcesRate(copy->resourcesRate),
	resourcesCooldown(0),

	baseUpgradeCenter(copy->baseUpgradeCenter),
	turretsVector(copy->turretsVector),
	barricadesVector(copy->barricadesVector)
{
	baseAreaAlarm = app->coll->AddCollider(copy->baseAreaAlarm->rect, copy->baseAreaAlarm->type, copy->baseAreaAlarm->callback, this);

	int x = position.x - baseAreaAlarm->rect.w *0.5f;
	int y = position.y - baseAreaAlarm->rect.h * 0.6f;

	baseAreaAlarm->SetPos(x, y);
	radiusSize = 5;

	//FoW Related
	if (alignement == ENTITY_ALIGNEMENT::PLAYER || alignement == ENTITY_ALIGNEMENT::NEUTRAL)
	{
		visionEntity = app->fowManager->CreateFoWEntity(position, true, 15,5);//TODO that 15 needs to be passed as a parameter
	}
	else
	{
		visionEntity = app->fowManager->CreateFoWEntity(position, false, 15,5);
	}
}


Base::~Base()
{
	baseAreaAlarm = nullptr;

	baseUpgradeCenter = nullptr;


	turretsVector.clear();
	barricadesVector.clear();
}


bool Base::Update(float dt)
{
	if (align == ENTITY_ALIGNEMENT::ENEMY)
	{
		baseAreaAlarm->active = true;
	}
	
	GainResources(dt);

	return true;
}


bool Base::PostUpdate(float dt)
{
	DebugDraw();
	return true;
}


void Base::DisableTurrets()
{
	for (uint i = 0; i < this->turretsVector.size(); i++) 
	{
		//TODO: someone left this for empty
		
	}
}


bool Base::AddTurret(Turret* turret)
{
	if (turretsVector.size() == maxTurrets)
		return false;

	else
	{
		if (baseUpgradeCenter != nullptr)
			turret->SetLevel(baseUpgradeCenter->GetTurretLevel());

		turretsVector.push_back(turret);
		turret->myBase = this;
		return true;
	}
}


bool Base::TurretCapacityExceed()
{
	if (turretsVector.size() >= maxTurrets)
	{
		return false;
	}
	return true;
}


bool Base::BarricadeCapacityExceed()
{
	if (barricadesVector.size() == maxBarricades)
	{
		return false;
	}
	return true;
}


bool Base::UpgradeCenterCapacityExceed()
{
	if (baseUpgradeCenter != nullptr)
	{
		return false;
	}
	return true;
}


bool Base::AddBarricade(Barricade* barricade)
{
	if (barricadesVector.size() == maxBarricades)
		return false;

	else
	{
		if (baseUpgradeCenter != nullptr)
			barricade->SetLevel(baseUpgradeCenter->GetBarricadeLevel());

		barricadesVector.push_back(barricade);
		barricade->myBase = this;
		return true;
	}
}


bool Base::AddUpgradeCenter(UpgradeCenter* upgradeCenter)
{
	//We should only have one upgrade center
	if (baseUpgradeCenter == nullptr)
	{
		baseUpgradeCenter = upgradeCenter;
		upgradeCenter->myBase = this;
		return true;
	}


	else
		return false;
	
}


void Base::RemoveTurret(Turret* turret)
{
	int numTurrets = turretsVector.size();

	for (int i = 0; i < numTurrets; i++)
	{
		if (turretsVector[i] == turret)
		{
			turretsVector.erase(turretsVector.begin() + i);
		}
	}

}


void Base::RemoveBarricade(Barricade* barricade)
{
	int numBarricades = barricadesVector.size();

	for (int i = 0; i < numBarricades; i++)
	{
		if (barricadesVector[i] == barricade)
		{
			barricadesVector.erase(barricadesVector.begin() + i);
		}
	}
}


void Base::RemoveUpgradeCenter()
{
	baseUpgradeCenter = nullptr;
}


void Base::LevelUpTurrets(int lvl)
{
	int numberTurrets = turretsVector.size();

	for (int i = 0; i < numberTurrets; i++)
	{
		if (turretsVector[i]->GetLvl() < lvl)
		{
			turretsVector[i]->LevelUp();
		}

		if (baseUpgradeCenter != nullptr)
		{

		}
	}
}


void Base::LevelUpBarricades(int lvl)
{
	int numberBarricades = barricadesVector.size();

	for (int i = 0; i < numberBarricades; i++)
	{
		if (barricadesVector[i]->GetLevel() < lvl)
		{
			barricadesVector[i]->LevelUp();
		}
	}
}


void Base::ChangeAligment()
{
	ENTITY_ALIGNEMENT aligment= ENTITY_ALIGNEMENT::UNKNOWN;

	if (align == ENTITY_ALIGNEMENT::ENEMY)
	{
		aligment = ENTITY_ALIGNEMENT::PLAYER;
		app->eventManager->GenerateEvent(EVENT_ENUM::PLAYER_CONQUERED_A_BASE, EVENT_ENUM::NULL_EVENT);

		if (visionEntity != nullptr)
		{
			visionEntity->SetEntityProvideVision(true);
			
		}


		if(minimapIcon!=nullptr)
		{
			minimapIcon->type = MINIMAP_ICONS::BASE;
		}
	}

	if (align == ENTITY_ALIGNEMENT::PLAYER)
	{
		aligment = ENTITY_ALIGNEMENT::ENEMY;
		app->eventManager->GenerateEvent(EVENT_ENUM::ENEMY_CONQUERED_A_BASE, EVENT_ENUM::NULL_EVENT);

		if (visionEntity != nullptr)
		{
			visionEntity->SetEntityProvideVision(false);
		}

		if (minimapIcon != nullptr)
		{
			minimapIcon->type = MINIMAP_ICONS::ENEMY_BASE;
		}

	}

	if (baseUpgradeCenter != nullptr)
	{
		baseUpgradeCenter->SetAlignment(aligment);
		baseUpgradeCenter->ChangeTextures();
	}

	int numTurrets = turretsVector.size();

	for (int i = 0; i < numTurrets; i++)
	{
		turretsVector[i]->SetAlignment(aligment);
	}


	int numBarricades = barricadesVector.size();

	for (int i = 0; i < numBarricades; i++)
	{
		barricadesVector[i]->SetAlignment(aligment);
	}

	align = aligment;
}


bool Base::RessurectHero(ENTITY_TYPE heroType)
{
	if (heroType == ENTITY_TYPE::HERO_GATHERER)
	{
		app->entityManager->AddEntity(heroType, position.x, position.y);
		return true;
	}

	else if (heroType == ENTITY_TYPE::HERO_MELEE)
	{
		app->entityManager->AddEntity(heroType, position.x, position.y);
		return true;
	}

	else if (heroType == ENTITY_TYPE::HERO_RANGED)
	{
		app->entityManager->AddEntity(heroType, position.x, position.y);
		return true;
	}


	return false;
}


void Base::GainResources(float dt)
{

	if (align == ENTITY_ALIGNEMENT::PLAYER)
	{
		resourcesCooldown += dt;

		if (resourcesCooldown > resourcesRate)
		{
			app->player->AddResources(resourcesProduced);
			resourcesCooldown -= resourcesRate;
		}
		
	}
}


int Base::RecieveDamage(float damage)
{

	if (hitPointsCurrent > 0)
	{
		hitPointsCurrent -= damage;

		int randomCounter = rand() % 10;

		if (randomCounter == 0)
			app->audio->PlayFx(app->entityManager->buildingGetsHit, 0, 1, this->GetMyLoudness(), this->GetMyDirection(), true);
		else if (randomCounter == 9)
			app->audio->PlayFx(app->entityManager->buildingGetsHit2, 0, 1, this->GetMyLoudness(), this->GetMyDirection(), true);

		if (hitPointsCurrent <= 0)
		{
			Die();
		}
	}

	return 0;
}


void Base::Die()
{
	hitPointsCurrent = hitPointsMax;

	ChangeTexturesOnDeath();

	ChangeAligment();
}

void Base::ChangeTexturesOnDeath()
{
	switch (GetAlignment()) { //change texture

	case ENTITY_ALIGNEMENT::ENEMY:
		texture = app->entityManager->base2Texture;
		selectedTexture = app->entityManager->base2TextureSelected;
		break;

	case ENTITY_ALIGNEMENT::PLAYER:
		texture = app->entityManager->base2TextureEnemy;
		selectedTexture = app->entityManager->base2TextureSelectedEnemy;
		break;

	default:
		break;

	}
}

int Base::GetHP()
{
	return this->hitPointsCurrent;
}

int Base::GetMaxHP()
{
	return this->hitPointsMax;
}

int Base::GetRsrc()
{
	return resourcesProduced;
}

int Base::GetcurrentTurrets()
{
	return turretsVector.size();
}

int Base::GetmaxTurrets()
{
	return maxTurrets;
}


std::vector<Turret*>* Base::GetTurretVector()
{
	return &turretsVector;
}


std::vector<Barricade*>* Base::GetBarricadeVector()
{
	return &barricadesVector;
}


UpgradeCenter* Base::GetUpgradeCenter()
{
	return baseUpgradeCenter;
}