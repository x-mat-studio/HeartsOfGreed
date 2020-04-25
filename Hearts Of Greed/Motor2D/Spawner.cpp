#include "Spawner.h"
#include "EntityManager.h"


Spawner::Spawner(fMPoint position, ENTITY_TYPE spawnerType, Collider* col, int maxHitPoints, int currentHitPoints, float spawnRate) :
	
	Entity(position, ENTITY_TYPE::SPAWNER, ENTITY_ALIGNEMENT::NEUTRAL, col, maxHitPoints, currentHitPoints),
	spawnerType(spawnerType),
	spawnRate(spawnRate),
	entitysToSpawn(0),
	timer(0)
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


bool Spawner::PostUpdate(float dt)
{
	collider->active = false;

	if (entitysToSpawn > 0 && active)
	{
		Spawn(dt);
		collider->active = true;
	}

	return true;
}


void Spawner::SetNumberToSpawn(int number)
{
	entitysToSpawn = number;
}


void Spawner::SetSpawnRate(float ratio)
{
	spawnRate = ratio;
}


void Spawner::Spawn(float dt)
{
	timer += dt;

	if (timer > spawnRate)
	{
		app->entityManager->AddEntity(spawnerType, position.x, position.y + collider->rect.h, ENTITY_ALIGNEMENT::ENEMY);
		
		timer -= spawnRate;
		entitysToSpawn--;
	}
	
}


void Spawner::Activate()
{
	active = true;
}


void Spawner::Desactivate()
{
	active = false;
	entitysToSpawn = 0;
}



bool Spawner::GetActive()
{
	return active;
}