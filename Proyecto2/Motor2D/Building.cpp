#include "Building.h"

Building::Building(int hitPoints, int recoveryHitPointsRate, int maxTurrets, int maxBarricades, int turretsLevel, int resourceOutput, int xpOnDeath) :

	hitPoints(hitPoints),
	recoveryHitPointsRate(recoveryHitPointsRate),
	maxTurrets(maxTurrets),
	maxBarricades(maxBarricades),
	turretsLevel(turretsLevel),
	resourceOutput(resourceOutput),
	xpOnDeath(xpOnDeath)
{}


Building::Building(SDL_Point position, Building* copy) :

	Entity(position, copy->type, copy->texture),
	hitPoints(copy->hitPoints),
	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	xpOnDeath(copy->xpOnDeath),
	maxTurrets(copy->maxTurrets),
	maxBarricades(copy->maxBarricades),
	turretsLevel(copy->turretsLevel),
	resourceOutput(copy->resourceOutput)
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