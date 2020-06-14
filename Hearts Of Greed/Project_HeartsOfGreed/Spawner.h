#pragma once

#ifndef __SPAWNER_H__
#define __SPAWNER_H_

#define MAX_Nº_SPAWNERS 5

#include "Entity.h"


class Spawner : public Entity
{
public:

	Spawner(fMPoint position, Collider* col, int wannamingosPerWave = 1, float wannamingosSpawnRate = 0, int gigamingosPerWave = 1, float gigamingosSpawnRate = 0, 
			int speedamingosPerWave = 1, float speedamingosSpawnRate = 0, int sniperPerWave = 1, float sniperSpawnRate = 0);
	Spawner(fMPoint position, Spawner* copy);

	~Spawner();

	bool PostUpdate(float dt);

	void Activate();
	void Desactivate();

	bool GetActive();

	void SetWannamingosToSpawn(int number);
	void AddAWanamingo();
	void SetGigamingosToSpawn(int number);
	void AddAGigamingo();
	void SetSpeedamingosToSpawn(int number);
	void AddASpeedamingo();
	void SetSnipermingosToSpawn(int number);
	void AddASnipermingo();

	//Getters and setters

	void SetWannamingoSpawnRate(float ratio);
	void SetGigamingoSpawnRate(float ratio);
	void SetSpeedamingoSpawnRate(float ratio);
	void SetSnipermingosSpawnRate(float ratio);

	void SetWannamingosPerWave(int entities);
	void SetGigamingosPerWave(int entities);
	void SetSpeedamingosPerWave(int entities);
	void SetSnipermingosPerWave(int entities);


	int GetWannamingosToSpawn() const;
	int GetGigamingosToSpawn() const;
	int GetSpeedamingosToSpawn() const;
	int GetSnipermingosToSpawn() const;

	float GetWannamingosSpawnRate() const;
	float GetGigamingosSpawnRate() const;
	float GetSpeedamingosSpawnRate() const;
	float GetSnipermingosSpawnRate() const;

	int GetWannamingosPerWave() const;
	int GetGigamingosPerWave() const;
	int GetSpeedamingosPerWave() const;
	int GetSnipermingosPerWave() const;


private:
	void SpawnWanamingo(float dt);
	void SpawnGigamingo(float dt);
	void SpawnSpeedamingo(float dt);
	void SpawnSnipermingo(float dt);

public:
	int wannamingosToSpawn;
	int gigamingosToSpawn;
	int speedamingosToSpawn;
	int snipermingosToSpawn;
private:

	//Wannamingo
	
	int wannamingosPerWave;

	float wannamingosSpawnRate;
	float wannamingoTimer;

	//Gigamingo
	
	int gigamingosPerWave;

	float gigamingosSpawnRate;
	float gigamingoTimer;

	//Speedamingo
	
	int speedamingosPerWave;

	float speedamingosSpawnRate;
	float speedamingoTimer;

	//Snipermingo
	
	int snipermingosPerWave;

	float snipermingosSpawnRate;
	float snipermingoTimer;
	

	bool active;


};




#endif //__SPAWNER_H_
