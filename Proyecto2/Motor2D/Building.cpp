#include "Building.h"

Building::Building(int hitPoints, int recoveryHitPointsRate, int xpOnDeath) :

	hitPointsMax(hitPoints),
	hitPointsCurrent(hitPoints),
	recoveryHitPointsRate(recoveryHitPointsRate),
	xpOnDeath(xpOnDeath)
{}


Building::Building(SDL_Point position, Building* copy) :

	Entity(position, copy->type, copy->texture),
	hitPointsMax(copy->hitPointsMax),
	hitPointsCurrent(copy->hitPointsMax),
	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	xpOnDeath(copy->xpOnDeath)
{}


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
