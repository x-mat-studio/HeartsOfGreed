#include "Barricade.h"

#include "Base.h"

#include "Render.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "Collision.h"

#include "Pathfinding.h"
#include "Map.h"

Barricade::Barricade(fMPoint position, int maxHitPoints, int currenthitPoints, int recoveryHitPointsRate, int xpOnDeadth, int buildingCost, int transparency, 
					 Collider* collider, SDL_Rect& verticalRect, SDL_Rect& horizontalRect, float hpIncrease) :

	Building(position, maxHitPoints, currenthitPoints, recoveryHitPointsRate, xpOnDeadth, buildingCost, transparency, collider, ENTITY_TYPE::BLDG_BARRICADE),
	
	barricadeLvl(1),
	direction(DIRECTION_BARRICADE::VERTICAL),

	verticalRect(verticalRect),
	horizontalRect(horizontalRect),

	hpIncrease(hpIncrease),
	
	currentRect(&this->verticalRect)
{}


Barricade::Barricade(fMPoint position, Barricade* copy, ENTITY_ALIGNEMENT align) :

	Building(position, copy, align),

	barricadeLvl(1),
	direction(copy->direction),

	verticalRect(copy->verticalRect),
	horizontalRect(copy->horizontalRect),

	hpIncrease(copy->hpIncrease),

	currentRect(nullptr)
{
	if (direction == DIRECTION_BARRICADE::VERTICAL) 
	{
		//app->pathfinding->SetWalkabilityMap(false, app->map->WorldToMap(position.x - 90, position.y), 3, 1);
		currentRect = &verticalRect;
	}
	else if (direction == DIRECTION_BARRICADE::HORIZONTAL) 
	{
		//app->pathfinding->SetWalkabilityMap(false, app->map->WorldToMap(position.x - 10, position.y), 1, 3);	
		currentRect = &horizontalRect;
	}
	
}


Barricade::~Barricade()
{
	barricadeLvl = -1;
	direction = DIRECTION_BARRICADE::NONE;

	currentRect = nullptr;
}


void Barricade::Draw(float dt)
{
	if (transparent)
	{
		app->render->Blit(texture, position.x, position.y, currentRect, false, true, transparencyValue, 255, 255, 255, 1.0f, -offset.x, -offset.y);
	}
	else
		app->render->Blit(texture, position.x, position.y, currentRect, false, true, 0, 255, 255, 255, 1.0f, -offset.x, -offset.y);
}


void Barricade::DrawSelected()
{
	if (selectedByPlayer == true)
		app->render->Blit(app->entityManager->selectedTexture, this->position.x - 5, this->position.y - this->collider->rect.h);
}


int Barricade::RecieveDamage(float damage, bool ignoreArmor)
{
	if (hitPointsCurrent > 0)
	{
		hitPointsCurrent -= damage;

		int randomCounter = rand() % 10;

		//TODO, CHANGE AUDIO
		if (randomCounter == 0)
			app->audio->PlayFx(app->entityManager->buildingGetsHit, 0, -1, this->GetMyLoudness(), this->GetMyDirection(), true);
		else if (randomCounter == 9)
			app->audio->PlayFx(app->entityManager->buildingGetsHit2, 0, -1, this->GetMyLoudness(), this->GetMyDirection(), true);

		if (hitPointsCurrent <= 0)
		{
			Die();
		}
	}

	return 0;
}


void Barricade::Die()
{
	app->eventManager->GenerateEvent(EVENT_ENUM::ENTITY_DEAD, EVENT_ENUM::NULL_EVENT);
	toDelete = true;


	//if (direction == DIRECTION_BARRICADE::VERTICAL)
		//app->pathfinding->SetWalkabilityMap(true, app->map->WorldToMap(position.x - 90, position.y), 3, 1);
	//else if (direction == DIRECTION_BARRICADE::HORIZONTAL)
		//app->pathfinding->SetWalkabilityMap(true, app->map->WorldToMap(position.x - 10, position.y), 1, 3);

	if (minimapIcon != nullptr)
	{
		minimapIcon->toDelete = true;
		minimapIcon->minimapPos = nullptr;
	}

	if (visionEntity != nullptr)
	{
		visionEntity->deleteEntity = true;
		visionEntity = nullptr;
	}

	if (myBase != nullptr)
	{
		myBase->RemoveBarricade(this);
	}
}


void Barricade::Flip()
{
	if (direction == DIRECTION_BARRICADE::VERTICAL)
	{
		direction = DIRECTION_BARRICADE::HORIZONTAL;
		
		currentRect = &horizontalRect;
	}

	else if (direction == DIRECTION_BARRICADE::HORIZONTAL)
	{
		direction = DIRECTION_BARRICADE::VERTICAL;

		currentRect = &verticalRect;
	}

	else
		assert("Barricade has problem");
}


int Barricade::GetLevel() const
{
	return barricadeLvl;
}


void Barricade::SetLevel(int lvl)
{
	for (int i = 1; i < lvl; i++)
	{
		LevelUp();
	}
}


void Barricade::LevelUp()
{
	hitPointsMax += hpIncrease;
	hitPointsCurrent = hitPointsMax;

	barricadeLvl++;
}