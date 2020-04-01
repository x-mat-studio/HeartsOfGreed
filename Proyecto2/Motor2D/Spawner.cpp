#include "Spawner.h"
#include "Enemy.h"

Spawner::Spawner(Enemy* prototype) : prototype(prototype)
{}

Enemy* Spawner::spawnEnemy()
{
    return prototype->Clone();
}

