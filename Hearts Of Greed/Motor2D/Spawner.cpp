#include "Spawner.h"
#include "EntityManager.h"


Spawner::Spawner(fMPoint position, ENTITY_TYPE spawnerType, Collider* col, int enemiesPerWave, float spawnRate) :
	
	Entity(position, ENTITY_TYPE::SPAWNER, ENTITY_ALIGNEMENT::NEUTRAL, col,1,1),

	spawnerType(spawnerType),
	spawnRate(spawnRate),
	entitiesPerWave(enemiesPerWave),

	entitysToSpawn(0),
	timer(0),

	active(true)
{}


Spawner::Spawner(fMPoint position, Spawner* copy) :

	Entity(position, ENTITY_TYPE::SPAWNER, ENTITY_ALIGNEMENT::NEUTRAL, copy->collider, copy->hitPointsMax, copy->hitPointsCurrent),

	spawnerType(copy->spawnerType),
	spawnRate(copy->spawnRate),
	entitiesPerWave(copy->entitiesPerWave),

	entitysToSpawn(0),
	timer(0),

	active(true)
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
	timer = spawnRate; //This is because when we give the order to spawn, the spawner spawns the first wave instantly
}


void Spawner::SetSpawnRate(float ratio)
{
	spawnRate = ratio;
}


void Spawner::SetEnemiesPerWave(int entities)
{
	entitiesPerWave = entities;
}


void Spawner::Spawn(float dt)
{
	timer += dt;

	if (timer > spawnRate)
	{
		for (int i = 0; i < entitiesPerWave && entitysToSpawn > 0; i++)
		{
			app->entityManager->AddEntity(spawnerType, position.x, position.y + collider->rect.h, ENTITY_ALIGNEMENT::ENEMY);
			entitysToSpawn--;
		}
		
		
		timer -= spawnRate;
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


int Spawner::GetNumberToSpawn()
{
	return entitysToSpawn;
}


float Spawner::GetSpawnRate()
{
	return spawnRate;
}


int Spawner::GetEnemiesPerWave()
{
	return entitiesPerWave;
}