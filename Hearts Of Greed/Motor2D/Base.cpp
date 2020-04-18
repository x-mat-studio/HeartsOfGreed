#include "Base.h"
#include "Turret.h"
#include "Barricade.h"
#include "UpgradeCenter.h"
#include "Building.h"

#include "Collision.h"
#include "EntityManager.h"
#include "Player.h"


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
	baseAreaAlarm = app->coll->AddCollider(copy->baseAreaAlarm->rect, copy->baseAreaAlarm->type, copy->baseAreaAlarm->callback);

	int x = position.x;
	int y = position.y;

	x -= baseAreaAlarm->rect.w * 0.25;
	y -= baseAreaAlarm->rect.h * 0.25;

	baseAreaAlarm->SetPos(x, y);

}


Base::~Base()
{}


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
	
	return true;
}


void Base::DisableTurrets()
{
	for (uint i = 0; i < this->turretsVector.size(); i++) 
	{

		
	}
}


bool Base::AddTurret(Turret* turret)
{
	if (turretsVector.size() == maxTurrets)
		return false;


	else
	{
		turretsVector.push_back(turret);
		return true;
	}
}


bool Base::AddBarricade(Barricade* barricade)
{
	if (barricadesVector.size() == maxBarricades)
		return false;


	else
	{
		barricadesVector.push_back(barricade);
		return true;
	}
}


bool Base::AddUpgradeCenter(UpgradeCenter* upgradeCenter)
{
	//We should only have one upgrade center
	if (baseUpgradeCenter == nullptr)
	{
		baseUpgradeCenter = upgradeCenter;
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
			delete turretsVector[i];
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
			delete barricadesVector[i];
			barricadesVector.erase(barricadesVector.begin() + i);
		}
	}
}


void Base::RemoveUpgradeCenter()
{
	baseUpgradeCenter = nullptr;
}


void Base::ChangeAligment()
{
	ENTITY_ALIGNEMENT aligment;

	if (align == ENTITY_ALIGNEMENT::ENEMY)
	{
		aligment = ENTITY_ALIGNEMENT::PLAYER;
	}

	if (align == ENTITY_ALIGNEMENT::PLAYER)
	{
		aligment = ENTITY_ALIGNEMENT::ENEMY;
	}

	if (baseUpgradeCenter != nullptr)
	{
		baseUpgradeCenter->SetAlignment(aligment);
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


int Base::RecieveDamage(int damage)
{

	if (hitPointsCurrent > 0)
	{
		hitPointsCurrent -= damage;
		if (hitPointsCurrent <= 0)
		{
			Die();
		}
	}
	else
	{
		int randomCounter = rand() % 10;

		if (randomCounter == 0)
			app->audio->PlayFx(app->entityManager->buildingGetsHit, 0, 5, this->GetMyLoudness(), this->GetMyDirection(), true);
		else if (randomCounter == 9)
			app->audio->PlayFx(app->entityManager->buildingGetsHit2, 0, 5, this->GetMyLoudness(), this->GetMyDirection(), true);
	}

	return 0;
}


void Base::Die()
{
	hitPointsCurrent = hitPointsMax;

	ChangeAligment();
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
