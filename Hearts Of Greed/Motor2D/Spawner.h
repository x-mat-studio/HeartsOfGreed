#pragma once

#ifndef __SPAWNER_H__
#define __SPAWNER_H_

#include "Entity.h"


class Spawner : public Entity
{
public:

	Spawner(fMPoint position, Collider* col, int wannamingosPerWave = 1, float wannamingosSpawnRate = 0, int gigamingosPerWave = 1, float gigamingosSpawnRate = 0, 
			int speedamingosPerWave = 1, float speedamingosSpawnRate = 0, int sniperPerWave = 1, float sniperSpawnRate = 0);
	Spawner(fMPoint position, Spawner* copy);

	~Spawner();

	void SetWannamingosToSpawn(int number);
	void SetGigamingosToSpawn(int number);
	void SetSpeedamingosToSpawn(int number);
	void SetSnipermingosToSpawn(int number);

	void SetWannamingoSpawnRate(float ratio);
	void SetGigamingoSpawnRate(float ratio);
	void SetSpeedamingoSpawnRate(float ratio);
	void SetSnipermingosSpawnRate(float ratio);

	void SetWannamingosPerWave(int entities);
	void SetGigamingosPerWave(int entities);
	void SetSpeedamingosPerWave(int entities);
	void SetSnipermingosPerWave(int entities);

	bool PostUpdate(float dt);

	void Activate();
	void Desactivate();

	bool GetActive();

	int GetWannamingosToSpawn();
	int GetGigamingosToSpawn();
	int GetSpeedamingosToSpawn();
	int GetSnipermingosToSpawn();

	float GetWannamingosSpawnRate();
	float GetGigamingosSpawnRate();
	float GetSpeedamingosSpawnRate();
	float GetSnipermingosSpawnRate();

	int GetWannamingosPerWave();
	int GetGigamingosPerWave();
	int GetSpeedamingosPerWave();
	int GetSnipermingosPerWave();


private:
	void SpawnWanamingo(float dt);
	void SpawnGigamingo(float dt);
	void SpawnSpeedamingo(float dt);
	void SpawnSnipermingo(float dt);

private:

	//Wannamingo
	int wannamingosToSpawn;
	int wannamingosPerWave;

	float wannamingosSpawnRate;
	float wannamingoTimer;

	//Gigamingo
	int gigamingosToSpawn;
	int gigamingosPerWave;

	float gigamingosSpawnRate;
	float gigamingoTimer;

	//Speedamingo
	int speedamingosToSpawn;
	int speedamingosPerWave;

	float speedamingosSpawnRate;
	float speedamingoTimer;

	//Snipermingo
	int snipermingosToSpawn;
	int snipermingosPerWave;

	float snipermingosSpawnRate;
	float snipermingoTimer;
	

	bool active;
};




#endif //__SPAWNER_H_
