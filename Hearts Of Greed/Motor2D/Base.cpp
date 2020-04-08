#include "Base.h"
#include "Turret.h"
#include "Barricade.h"
#include "UpgradeCenter.h"
#include "Building.h"

#include "Collision.h"
#include "EntityManager.h"
#include "Player.h"


Base::Base(fMPoint position, Collider* collider, int maxTurrets, int maxBarricades, UpgradeCenter* baseUpgradeCenter, std::vector <Turret*> baseTurrets,  
	       std::vector <Barricade*> baseBarricades,Collider* baseArea, int resourcesProduced, float resourcesRate, int hitPoints, 
	       int recoveryHitPointsRate, int transparency) :

	Building(position, hitPoints, recoveryHitPointsRate, 0, 0, transparency, collider, ENTITY_TYPE::BLDG_BASE),

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
	int hitPoints, int recoveryHitPointsRate, int transparency):

	Building(position, hitPoints, recoveryHitPointsRate, 0, 0, transparency, collider, ENTITY_TYPE::BLDG_BASE),

	maxTurrets(maxTurrets),
	maxBarricades(maxBarricades),

	resourcesProduced(resourcesProduced),
	resourcesRate(resourcesRate),
	resourcesCooldown(0),

	baseUpgradeCenter(baseUpgradeCenter),

	baseAreaAlarm(baseArea)

{}


Base::Base(fMPoint position, Base* copy, ENTITY_ALIGNEMENT alignement) :

	Building(copy->position, copy, alignement),

	maxTurrets(copy->maxTurrets),
	maxBarricades(copy->maxBarricades),
	
	resourcesProduced(copy->resourcesProduced),
	resourcesRate(copy->resourcesRate),
	resourcesCooldown(0),

	baseUpgradeCenter(copy->baseUpgradeCenter),
	turretsVector(copy->turretsVector),
	barricadesVector(copy->barricadesVector),

	baseAreaAlarm(copy->baseAreaAlarm)
{}


Base::~Base()
{}


bool Base::Update(float dt)
{
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


void Base::ChangeAligment(ENTITY_ALIGNEMENT aligment)
{
	baseUpgradeCenter->SetAlignment(aligment);

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