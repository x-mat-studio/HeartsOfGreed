#include "Base.h"
#include "Turret.h"
#include "Barricade.h"
#include "Core.h"
#include "UpgradeCenter.h"
#include "Building.h"
#include "Collision.h"


Base::Base(int maxTurrets, int maxBarricades, Core* baseCore, UpgradeCenter* baseUpgradeCenter, std::vector <Turret*> baseTurrets, std::vector <Barricade*> baseBarricades, Collider* baseArea) :

	maxTurrets(maxTurrets),
	maxBarricades(maxBarricades),
	baseCore(baseCore),
	baseUpgradeCenter(baseUpgradeCenter),
	turretsVector(baseTurrets),
	barricadesVector(baseBarricades),
	baseAreaAlarm(baseArea)
{}


Base::Base(Base * copy):

	maxTurrets(copy->maxTurrets),
	maxBarricades(copy->maxBarricades),
	baseCore(copy->baseCore),
	baseUpgradeCenter(copy->baseUpgradeCenter),
	turretsVector(copy->turretsVector),
	barricadesVector(copy->barricadesVector),
	baseAreaAlarm(copy->baseAreaAlarm)
{}


Base::~Base()
{}


void Base::DisableTurrets()
{
	for (uint i = 0; i < this->turretsVector.size(); i++) 
	{

		
	}
}


void Base::AddTurret(Turret* turret)
{
	turretsVector.push_back(turret);
}


void Base::AddBarricade(Barricade* barricade)
{
	barricadesVector.push_back(barricade);
}


void Base::AddUpgradeCenter(UpgradeCenter* upgradeCenter)
{
	//We should only have one upgrade center
	assert(baseUpgradeCenter != nullptr);

	baseUpgradeCenter = upgradeCenter;
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

}


void Base::ChangeAligment(ENTITY_ALIGNEMENT aligment)
{

	baseCore->SetAlignment(aligment);
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

	baseAligment = aligment;
}