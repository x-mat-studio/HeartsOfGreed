#include "Spawner.h"
#include "EntityManager.h"


Spawner::Spawner(fMPoint position, ENTITY_TYPE spawnerType, Collider* col, int maxHitPoints, int currentHitPoints) :
	
	Entity(position, ENTITY_TYPE::SPAWNER, ENTITY_ALIGNEMENT::NEUTRAL, col, maxHitPoints, currentHitPoints),
	spawnerType(spawnerType),
	entitysToSpawn(0)
{}


Spawner::Spawner(fMPoint position, Spawner* copy) :

	Entity(position, ENTITY_TYPE::SPAWNER, ENTITY_ALIGNEMENT::NEUTRAL, copy->collider, copy->hitPointsMax, copy->hitPointsCurrent),
	spawnerType(copy->spawnerType),
	entitysToSpawn(0)
{
	collider->SetPos(position.x, position.y);
}


Spawner::~Spawner()
{}


bool Spawner::PreUpdate(float dt)
{
	collider->active = false;

	if (entitysToSpawn > 0)
	{
		Spawn();

		collider->active = true;
		entitysToSpawn--;
	}

	return true;
}


void Spawner::SetNumberToSpawn(int number)
{
	entitysToSpawn = number;
}


void Spawner::Spawn()
{
	app->entityManager->AddEntity(spawnerType, position.x, position.y, ENTITY_ALIGNEMENT::ENEMY);
}
