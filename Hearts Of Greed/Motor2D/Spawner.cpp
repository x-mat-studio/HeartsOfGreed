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
	int x, y;
	x = position.x + copy->collider->rect.w * 0.5f;
	y = position.y - copy->collider->rect.h * 0.5f;

	collider->SetPos(x, y);
}


Spawner::~Spawner()
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
	app->entityManager->AddEntity(spawnerType, position.x, position.y + collider->rect.h, ENTITY_ALIGNEMENT::ENEMY);
}
