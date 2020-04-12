#include "Building.h"
#include "Render.h"
#include "Textures.h"
#include "EntityManager.h"

Building::Building(fMPoint position, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int xpOnDeath, int buildingCost, int transparency, Collider* collider,
	ENTITY_TYPE type, BUILDING_DECOR decor) :

	Entity(position, type, ENTITY_ALIGNEMENT::NEUTRAL, collider, maxHitPoints, currentHitPoints),

	recoveryHitPointsRate(recoveryHitPointsRate),
	xpOnDeath(xpOnDeath),
	buildingCost(buildingCost),
	transparencyValue(transparency),

	myBase(nullptr),

	transparent(false),
	selected(false),
	currentState(BUILDING_STATE::ST_UNKNOWN),
	myDecor(decor)
{}


Building::Building(fMPoint position, Building* copy, ENTITY_ALIGNEMENT alignement) :

	Entity(position, copy->type, alignement, copy->collider, copy->hitPointsMax, copy->hitPointsCurrent),

	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	xpOnDeath(copy->xpOnDeath),
	buildingCost(copy->buildingCost),
	transparencyValue(copy->transparencyValue),
	myDecor(copy->myDecor),

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
		app->render->Blit(texture, position.x, position.y, nullptr, false, true, transparencyValue);
	}
	 
	else
	{
		app->render->Blit(texture, position.x, position.y, nullptr, false, true);
	}
}

void Building::MinimapDraw(float scale, float halfWidth)
{
	float worldX = position.x;
	float worldY = position.y;

	worldX += app->render->currentCamX;
	worldY += app->render->currentCamY;

	SDL_Texture* auxTexture = nullptr;

	switch (GetDecor())
	{
	case BUILDING_DECOR::ST_01:
		auxTexture = app->tex->Load("maps/base01.png");
		break;
	case BUILDING_DECOR::ST_02:
		auxTexture = app->tex->Load("maps/base02.png");
		break;
	case BUILDING_DECOR::ST_03:
		auxTexture = app->tex->Load("maps/base03.png");
		break;

	}

	
	app->render->MinimapBlit(auxTexture, worldX + halfWidth, worldY, NULL, scale);

	app->tex->UnLoad(auxTexture);
}

void Building::ActivateTransparency()
{
	transparent = true;
}

BUILDING_DECOR Building::GetDecor()
{
	return myDecor;
}
