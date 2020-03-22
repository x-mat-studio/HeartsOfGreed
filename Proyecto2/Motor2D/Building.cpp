#include "Building.h"

Building::Building(int hitPoints, int recoveryHitPointsRate, int xpOnDeath, int buildingCost) :

	hitPointsMax(hitPoints),
	hitPointsCurrent(hitPoints),
	recoveryHitPointsRate(recoveryHitPointsRate),
	xpOnDeath(xpOnDeath),
	buildingCost(buildingCost)
{}


Building::Building(iMPoint position, Building* copy) :

	Entity(position, copy->type),
	hitPointsMax(copy->hitPointsMax),
	hitPointsCurrent(copy->hitPointsMax),
	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	xpOnDeath(copy->xpOnDeath),
	buildingCost(copy->buildingCost)
{}

Building::Building()
{}

void Building::Destroy()
{
}


Building::~Building()
{}

bool Building::Start()
{
	bool ret = true;

	return ret;
}

bool Building::PreUpdate(float dt)
{
	bool ret = true;

	return ret;
}

bool Building::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool Building::PostUpdate(float dt)
{
	bool ret = true;

	return ret;
}

bool Building::CleanUp()
{
	bool ret = true;

	return ret;
}

bool Building::Load(pugi::xml_node &)
{
	bool ret = true;

	return ret;
}

bool Building::Save(pugi::xml_node &) const
{
	bool ret = true;

	return ret;
}

void Building::ChangeBuildingState(BUILDING_STATE state)
{
	this->currentState = state;
}

void Building::BeingRepaired()
{
}

void Building::RecoverHealth()
{
	this->hitPointsCurrent += this->recoveryHitPointsRate;
}

void Building::Contruct()
{
}


