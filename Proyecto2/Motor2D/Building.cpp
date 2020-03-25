#include "Building.h"
#include "Render.h"
#include "Textures.h"

Building::Building(int hitPoints, int recoveryHitPointsRate, int xpOnDeath, int buildingCost, int transparency) :

	Entity(position, ENTITY_TYPE::BUILDING),

	hitPointsMax(hitPoints),
	hitPointsCurrent(hitPoints),
	recoveryHitPointsRate(recoveryHitPointsRate),
	xpOnDeath(xpOnDeath),
	buildingCost(buildingCost),
	transparencyValue(transparency),

	myBase(nullptr),
	texture(nullptr),
	transparent(false),
	selected(false),
	currentState(BUILDING_STATE::ST_UNKNOWN)
{}


Building::Building(fMPoint position, Building* copy) :

	Entity(position, copy->type),

	hitPointsMax(copy->hitPointsMax),
	hitPointsCurrent(copy->hitPointsMax),
	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	xpOnDeath(copy->xpOnDeath),
	buildingCost(copy->buildingCost),
	transparencyValue(copy->transparencyValue),

	myBase(nullptr),
	texture(nullptr),
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


bool Building::Start()
{
	
	return true;
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
	Draw();
	return true;
}


bool Building::CleanUp()
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


bool Building::Load(pugi::xml_node &)
{

	return true;
}


bool Building::Save(pugi::xml_node &) const
{

	return true;
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


void Building::Draw()
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