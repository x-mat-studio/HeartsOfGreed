#include "Spawner.h"
#include "EntityManager.h"
#include "Enemy.h"

Spawner::Spawner(Enemy* prototype) : prototype(prototype)
{}

Enemy* Spawner::spawnEnemy()
{
    app->entityManager->AddEntity(ENTITY_TYPE::ENEMY,position.x,position.y);
    return prototype->Clone();
}

Enemy* Spawner::spawnEnemy(fMPoint positionToBe)
{
    app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, positionToBe.x, positionToBe.y);
    return prototype->Clone(positionToBe);
}

Enemy* Spawner::spawnEnemy(int x, int y)
{
    app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, x, y);
    return prototype->Clone( x,  y);
}

