#include "Spawner.h"
#include "EntityManager.h"
#include "Enemy.h"


Spawner::Spawner(Enemy* prototype) : prototype(prototype)
{}


Enemy* Spawner::spawnEnemy()
{
	app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, position.x, position.y);
	return prototype->Clone();
}


Enemy* Spawner::spawnEnemy(int x, int y)
{
	app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, x, y);
	return prototype->Clone(x, y);
}


void spawnEnemies(int x, int y, uint nEnemies)
{

	for (int i = 0; i < nEnemies; i++)
	{
		app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, x, y);
	}

}

