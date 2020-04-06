#include "Spawner.h"
#include "EntityManager.h"


Spawner::Spawner(fMPoint position, ENTITY_TYPE spawnerType) :
	
	Entity(position, ENTITY_TYPE::SPAWNER, ENTITY_ALIGNEMENT::NEUTRAL, nullptr),
	spawnerType(spawnerType),
	entitysToSpawn(0)
{}

Spawner::Spawner(fMPoint position, Spawner* copy) :

	Entity(position, ENTITY_TYPE::SPAWNER, ENTITY_ALIGNEMENT::NEUTRAL, nullptr),
	spawnerType(copy->spawnerType),
	entitysToSpawn(0)
{}

bool Spawner::PreUpdate(float dt)
{
	if (entitysToSpawn > 0)
	{
		Spawn();
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
