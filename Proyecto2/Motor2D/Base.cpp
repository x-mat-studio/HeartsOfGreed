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
	baseTurrets(baseTurrets),
	baseBarricades(baseBarricades),
	baseAreaAlarm(baseArea)
{}


Base::Base(Base * copy):

	maxTurrets(copy->maxTurrets),
	maxBarricades(copy->maxBarricades),
	baseCore(copy->baseCore),
	baseUpgradeCenter(copy->baseUpgradeCenter),
	baseTurrets(copy->baseTurrets),
	baseBarricades(copy->baseBarricades),
	baseAreaAlarm(copy->baseAreaAlarm)
{}


Base::~Base()
{}


bool Base::Start()
{
	bool ret = true;

	return ret;
}


bool Base::PreUpdate(float dt)
{
	bool ret = true;

	return ret;
}


bool Base::Update(float dt)
{
	bool ret = true;

	return ret;
}


bool Base::PostUpdate(float dt)
{
	bool ret = true;

	return ret;
}


bool Base::CleanUp()
{
	bool ret = true;

	return ret;
}

void Base::DisableTurrets()
{
	for (uint i = 0; i < this->baseTurrets.size(); i++) {

		//this->baseTurrets[i]->ChangeBuildingState(BUILDING_STATE::ST_NEUTRAL);

	}
}

void Base::AddBuilding(Building * building)
{
	building->myBase = this;
}

void Base::RemoveBuilding(Building * building)
{

}
