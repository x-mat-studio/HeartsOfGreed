#include "Core.h"



Core::Core(int resourceProductionRate, fMPoint position, Collider* collider, int hitPoints , int recoveryHitPointsRate , int xpOnDeath , int buildingCost , int transparency ) :

	Building(position, hitPoints, recoveryHitPointsRate, xpOnDeath, buildingCost, transparency, collider),
	resourceProductionRate(resourceProductionRate)
{}

Core::Core(Core * copy):

	Building(copy->position, copy),
	resourceProductionRate(copy->resourceProductionRate)
{}


Core::~Core()
{
}

bool Core::Start()
{
	return true;
}


bool Core::PreUpdate(float dt)
{
	return true;
}


bool Core::Update(float dt)
{
	return true;
}


bool Core::PostUpdate(float dt)
{
	return true;
}

void Core::RessurectHero(Hero * hero)
{
}

void Core::GainResources()
{
}
