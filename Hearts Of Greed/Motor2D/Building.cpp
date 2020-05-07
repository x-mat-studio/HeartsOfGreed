#include "Building.h"
#include "Render.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Brofiler/Brofiler/Brofiler.h"


Building::Building(fMPoint position, int maxHitPoints, int currentHitPoints, int recoveryHitPointsRate, int xpOnDeath, int buildingCost, int transparency, Collider* collider,
	ENTITY_TYPE type, BUILDING_DECOR decor) :

	Entity(position, type, ENTITY_ALIGNEMENT::NEUTRAL, collider, maxHitPoints, currentHitPoints),

	recoveryHitPointsRate(recoveryHitPointsRate),
	xpOnDeath(xpOnDeath),
	buildingCost(buildingCost),
	transparencyValue(transparency),

	myBase(nullptr),
	selectedTexture(nullptr),
	transparent(false),
	selected(false),
	currentState(BUILDING_STATE::ST_UNKNOWN),
	myDecor(decor)
{
}


Building::Building(fMPoint position, Building* copy, ENTITY_ALIGNEMENT alignement) :

	Entity(position, copy->type, alignement, copy->collider, copy->hitPointsMax, copy->hitPointsCurrent),

	recoveryHitPointsRate(copy->recoveryHitPointsRate),
	xpOnDeath(copy->xpOnDeath),
	buildingCost(copy->buildingCost),
	transparencyValue(copy->transparencyValue),
	myDecor(copy->myDecor),
	selectedTexture(copy->selectedTexture),

	myBase(nullptr),
	transparent(false),
	selected(false),
	currentState(BUILDING_STATE::ST_UNKNOWN)
{
}


Building::Building()
{
}


void Building::Destroy()
{
}


Building::~Building()
{
	myBase = nullptr;
	selectedTexture = nullptr;
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



void Building::Contruct()
{
}


void Building::Draw(float dt)
{
	BROFILER_CATEGORY("DRAW Static Buildings", Profiler::Color::DarkGoldenRod);

	fMPoint newPos = position + offset;

	if (selectedByPlayer)
	{
		if (transparent) 
		{
			app->render->Blit(selectedTexture, newPos.x, newPos.y, nullptr, false, true, transparencyValue);
		}
		else
		{
			app->render->Blit(selectedTexture, newPos.x, newPos.y, nullptr, false, true);
		}
	}
	else 
	{
		if (transparent)
		{
			app->render->Blit(texture, newPos.x, newPos.y, nullptr, false, true, transparencyValue);
		}
		else
		{
			app->render->Blit(texture, newPos.x, newPos.y, nullptr, false, true);
		}
	}
}

void Building::MinimapDraw(float scale, float halfWidth)
{
	float worldX = position.x+offset.x;
	float worldY = position.y+offset.y;

	worldX += app->render->currentCamX;
	worldY += app->render->currentCamY;

	SDL_Texture* auxTexture = nullptr;

	
	if (texture != nullptr)
	{
		app->render->MinimapBlit(texture, worldX + halfWidth, worldY, NULL, scale);
	}
	else
	{
		app->minimap->MinimapFoWNeedsUpdate();
	}

}

void Building::ActivateTransparency()
{
	transparent = true;
}

BUILDING_DECOR Building::GetDecor()
{
	return myDecor;
}
