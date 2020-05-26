#pragma once

#ifndef __SPAWNER_H__
#define __SPAWNER_H_

#define MAX_Nº_SPAWNERS 20

#include "Entity.h"


class Spawner : public Entity
{
public:

	Spawner(fMPoint position, ENTITY_TYPE spawnerType, Collider* col, int enemiesPerWave = 1, float spawnRate = 0);
	Spawner(fMPoint position, Spawner* copy);

	~Spawner();

	void SetNumberToSpawn(int number);
	void SetSpawnRate(float ratio);
	void SetEnemiesPerWave(int entities);

	bool PostUpdate(float dt);

	void Activate();
	void Desactivate();

	bool GetActive();

	int GetNumberToSpawn();
	float GetSpawnRate();
	int GetEnemiesPerWave();


private:
	void Spawn(float dt);

private:

	ENTITY_TYPE spawnerType;
	int entitysToSpawn;

	int entitiesPerWave;
	float spawnRate;


	float timer;

	bool active;
};




#endif //__SPAWNER_H_
