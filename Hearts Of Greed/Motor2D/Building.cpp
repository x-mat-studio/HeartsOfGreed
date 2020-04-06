#include "Building.h"
#include "Render.h"
#include "Textures.h"

Building::Building(fMPoint position, int hitPoints, int recoveryHitPointsRate, int xpOnDeath, int buildingCost, int transparency, Collider* collider) :

	Entity(position, ENTITY_TYPE::BUILDING, ENTITY_ALIGNEMENT::NEUTRAL, collider),

	hitPointsMax(hitPoints),
	hitPointsCurrent(hitPoints),
	recoveryHitPointsRate(recoveryHitPointsRate),
	xpOnDeath(xpOnDeath),
	buildingCost(buildingCost),
	transparencyValue(transparency),

	myBase(nullptr),

	transparent(false),
	selected(false),
	currentState(BUILDING_STATE::ST_UNKNOWN)
{}


Building::Building(fMPoint position, Building* copy, ENTITY_ALIGNEMENT alignement) :

	Entity(position, copy->type, alignement, copy->collider),

	hitPointsMax(copy->hitPointsMax),
	hitPointsCurrent(copy->hitPointsMax),
	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	xpOnDeath(copy->xpOnDeath),
	buildingCost(copy->buildingCost),
	transparencyValue(copy->transparencyValue),

	myBase(nullptr),

	transparent(false),
	selected(false),
	currentState(BUILDING_STATE::ST_UNKNOWN)
{}


Building::Building()
{
}


void Building::Destroy()
{
}


Building::~Building()
{
}


bool Building::PreUpdate(float dt)
{
	transparent = false;

	return true;
}


bool Building::Update(float dt)
{
	
	return true;
}


bool Building::PostUpdate(float dt)
{
	return true;
}


void Building::OnCollision(Collider* collider)
{
	if (collider->type == COLLIDER_HERO)
	{
		transparent = true;
	}
}


void Building::ChangeBuildingState(BUILDING_STATE state)
{
	currentState = state;
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


void Building::Draw(float dt)
{
	if (transparent)
	{
		app->render->Blit(texture, position.x, position.y, NULL, transparencyValue);
	}

	else
	{
		app->render->Blit(texture, position.x, position.y);
	}
}