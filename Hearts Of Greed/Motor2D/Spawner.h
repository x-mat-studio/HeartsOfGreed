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

	void SetNumberToSpawn(int number);
	void SetSpawnRate(float ratio);

	bool PostUpdate(float dt);

	void Activate();
	void Desactivate();

	bool GetActive();

private:
	void Spawn(float dt);

private:

	ENTITY_TYPE spawnerType;
	int entitysToSpawn;
	float spawnRate;

	float timer;

	bool active;
};




#endif //__SPAWNER_H_
