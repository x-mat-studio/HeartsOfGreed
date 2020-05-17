#include "Barricade.h"

#include "Base.h"

#include "Render.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "Collision.h"

Barricade::Barricade(fMPoint position, int maxHitPoints, int currenthitPoints, int recoveryHitPointsRate, int xpOnDeadth, int buildingCost, int transparency, Collider* collider, int barricadeLvl) :

	Building(position, maxHitPoints, currenthitPoints, recoveryHitPointsRate, xpOnDeadth, buildingCost, transparency, collider, ENTITY_TYPE::BLDG_BARRICADE),
	
	barricadeLvl(1),
	vertical(false)
{}

Barricade::Barricade(fMPoint position, Barricade* copy, ENTITY_ALIGNEMENT align) :

	Building(position, copy, align),

	barricadeLvl(1),
	vertical(copy->vertical)
{
}


Barricade::~Barricade()
{
}


void Barricade::DrawSelected()
{
	if (selectedByPlayer == true)
		app->render->Blit(app->entityManager->selectedTexture, this->collider->rect.x + this->collider->rect.w * 0.5f, this->collider->rect.y);
}


int Barricade::RecieveDamage(int damage)
{
	if (hitPointsCurrent > 0)
	{
		hitPointsCurrent -= damage;

		int randomCounter = rand() % 10;

		//TODO, CHANGE AUDIO
		if (randomCounter == 0)
			app->audio->PlayFx(app->entityManager->buildingGetsHit, 0, 3, this->GetMyLoudness(), this->GetMyDirection(), true);
		else if (randomCounter == 9)
			app->audio->PlayFx(app->entityManager->buildingGetsHit2, 0, 3, this->GetMyLoudness(), this->GetMyDirection(), true);

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
	//int w = collider->rect.w;
	//int h = collider->rect.h;

	//vertical = !vertical;

	//collider->rect.w = 
}


void Barricade::SetLevel(int level)
{

}