#pragma once

#ifndef __SPAWNER_H__
#define __SPAWNER_H_

#include "Entity.h"


class Spawner : public Entity
{
public:

	Spawner(fMPoint position, ENTITY_TYPE spawnerType, Collider* col, int maxHitPoints, int currentHitPoints, float spawnRate = 0);
	Spawner(fMPoint position, Spawner* copy);

	~Spawner();

	void Spawn(float dt);
	void SetNumberToSpawn(int number);

	bool PreUpdate(float dt);

private:

	ENTITY_TYPE spawnerType;
	int entitysToSpawn;
	float spawnRate;

	float timer;
};




#endif //__SPAWNER_H_
