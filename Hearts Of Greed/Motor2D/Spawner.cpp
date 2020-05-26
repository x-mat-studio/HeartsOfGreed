#include "Spawner.h"
#include "EntityManager.h"


Spawner::Spawner(fMPoint position, Collider* col, int wannamingosPerWave, float wannamingosSpawnRate, int gigamingosPerWave, float gigamingosSpawnRate,
	int speedamingosPerWave, float speedamingosSpawnRate, int snipermingosPerWave, float snipermingosSpawnRate) :
	
	Entity(position, ENTITY_TYPE::SPAWNER, ENTITY_ALIGNEMENT::NEUTRAL, col,1,1),
	active(true),
	
//Wannamingo
	wannamingosToSpawn(0),
	wannamingosPerWave(wannamingosPerWave),

	wannamingosSpawnRate(wannamingosSpawnRate),
	wannamingoTimer(0.f),

//Gigamingo
	gigamingosToSpawn(0),
	gigamingosPerWave(gigamingosPerWave),

	gigamingosSpawnRate(gigamingosSpawnRate),
	gigamingoTimer(0.f),

//Speedamingo
	speedamingosToSpawn(0),
	speedamingosPerWave(speedamingosPerWave),

	speedamingosSpawnRate(speedamingosSpawnRate),
	speedamingoTimer(0.f),

//Snipermingo
	snipermingosToSpawn(0),
	snipermingosPerWave(snipermingosPerWave),

	snipermingosSpawnRate(snipermingosSpawnRate),
	snipermingoTimer(0.f)

{}


Spawner::Spawner(fMPoint position, Spawner* copy) :

	Entity(position, ENTITY_TYPE::SPAWNER, ENTITY_ALIGNEMENT::NEUTRAL, copy->collider, copy->hitPointsMax, copy->hitPointsCurrent),
	active(true),

	//Wannamingo
	wannamingosToSpawn(0),
	wannamingosPerWave(copy->wannamingosPerWave),

	wannamingosSpawnRate(copy->wannamingosSpawnRate),
	wannamingoTimer(0.f),

	//Gigamingo
	gigamingosToSpawn(0),
	gigamingosPerWave(copy->gigamingosPerWave),

	gigamingosSpawnRate(copy->gigamingosSpawnRate),
	gigamingoTimer(0.f),

	//Speedamingo
	speedamingosToSpawn(0),
	speedamingosPerWave(copy->speedamingosPerWave),

	speedamingosSpawnRate(copy->speedamingosSpawnRate),
	speedamingoTimer(0.f),

	//Snipermingo
	snipermingosToSpawn(0),
	snipermingosPerWave(copy->snipermingosPerWave),

	snipermingosSpawnRate(copy->snipermingosSpawnRate),
	snipermingoTimer(0.f)

	
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

	if (wannamingosToSpawn > 0 && active)
	{
		SpawnWanamingo(dt);
		collider->active = true;
	}

	if (gigamingosToSpawn > 0 && active)
	{
		SpawnGigamingo(dt);
		collider->active = true;
	}

	if (speedamingosToSpawn > 0 && active)
	{
		SpawnSpeedamingo(dt);
		collider->active = true;
	}

	if (snipermingosToSpawn > 0 && active)
	{
		SpawnSnipermingo(dt);
		collider->active = true;
	}

	return true;
}



void Spawner::SetWannamingosToSpawn(int number)
{
	wannamingosToSpawn = number;
	wannamingoTimer = wannamingosSpawnRate; //This is because when we give the order to spawn, the spawner spawns the first wave instantly
}

void Spawner::SetGigamingosToSpawn(int number)
{
	gigamingosToSpawn = number;
	gigamingoTimer = gigamingosSpawnRate;
}

void Spawner::SetSpeedamingosToSpawn(int number)
{
	speedamingosToSpawn = number;
	speedamingoTimer = speedamingosSpawnRate;
}

void Spawner::SetSnipermingosToSpawn(int number)
{
	snipermingosToSpawn = number;
	snipermingoTimer = snipermingosSpawnRate;
}



void Spawner::SetWannamingoSpawnRate(float ratio)
{
	wannamingosSpawnRate = ratio;
}

void Spawner::SetGigamingoSpawnRate(float ratio)
{
	gigamingosSpawnRate = ratio;
}

void Spawner::SetSpeedamingoSpawnRate(float ratio)
{
	speedamingosSpawnRate = ratio;
}

void Spawner::SetSnipermingosSpawnRate(float ratio)
{
	snipermingosSpawnRate = ratio;
}



void Spawner::SetWannamingosPerWave(int entities)
{
	wannamingosPerWave = entities;
}

void Spawner::SetGigamingosPerWave(int entities)
{
	gigamingosPerWave = entities;
}

void Spawner::SetSpeedamingosPerWave(int entities)
{
	speedamingosPerWave = entities;
}

void Spawner::SetSnipermingosPerWave(int entities)
{
	snipermingosPerWave = entities;
}



void Spawner::SpawnWanamingo(float dt)
{
	wannamingoTimer += dt;

	if (wannamingoTimer > wannamingosSpawnRate)
	{
		for (int i = 0; i < wannamingosPerWave && wannamingosToSpawn > 0; i++)
		{
			app->entityManager->AddEntity(ENTITY_TYPE::ENEMY, position.x, position.y + collider->rect.h, ENTITY_ALIGNEMENT::ENEMY);
			wannamingosToSpawn--;
		}
		
		
		wannamingoTimer -= wannamingosSpawnRate;
	}
}

void Spawner::SpawnGigamingo(float dt)
{
	gigamingoTimer += dt;

	if (gigamingoTimer > gigamingosSpawnRate)
	{
		for (int i = 0; i < gigamingosPerWave && gigamingosToSpawn > 0; i++)
		{
			app->entityManager->AddEntity(ENTITY_TYPE::ENEMY_GIGA, position.x, position.y + collider->rect.h, ENTITY_ALIGNEMENT::ENEMY);
			gigamingosToSpawn--;
		}


		gigamingoTimer -= gigamingosSpawnRate;
	}
}

void Spawner::SpawnSpeedamingo(float dt)
{
	speedamingoTimer += dt;

	if (speedamingoTimer > speedamingosSpawnRate)
	{
		for (int i = 0; i < speedamingosPerWave && speedamingosToSpawn > 0; i++)
		{
			app->entityManager->AddEntity(ENTITY_TYPE::ENEMY_NIGHT, position.x, position.y + collider->rect.h, ENTITY_ALIGNEMENT::ENEMY);
			speedamingosToSpawn--;
		}


		speedamingoTimer -= speedamingosSpawnRate;
	}
}

void Spawner::SpawnSnipermingo(float dt)
{
	snipermingoTimer += dt;

	if (snipermingoTimer > snipermingosSpawnRate)
	{
		for (int i = 0; i < snipermingosPerWave && snipermingosToSpawn > 0; i++)
		{
			app->entityManager->AddEntity(ENTITY_TYPE::ENEMY_RANGED, position.x, position.y + collider->rect.h, ENTITY_ALIGNEMENT::ENEMY);
			snipermingosToSpawn--;
		}


		snipermingoTimer -= snipermingosSpawnRate;
	}
}



void Spawner::Activate()
{
	active = true;
}


void Spawner::Desactivate()
{
	active = false;
	wannamingosToSpawn = 0;
}



bool Spawner::GetActive()
{
	return active;
}


int Spawner::GetWannamingosToSpawn()
{
	return wannamingosToSpawn;
}

int Spawner::GetGigamingosToSpawn()
{
	return gigamingosToSpawn;
}

int Spawner::GetSpeedamingosToSpawn()
{
	return speedamingosToSpawn;
}

int Spawner::GetSnipermingosToSpawn()
{
	return snipermingosToSpawn;
}



float Spawner::GetWannamingosSpawnRate()
{
	return wannamingosSpawnRate;
}

float Spawner::GetGigamingosSpawnRate()
{
	return gigamingosSpawnRate;
}

float Spawner::GetSpeedamingosSpawnRate()
{
	return speedamingosSpawnRate;
}

float Spawner::GetSnipermingosSpawnRate()
{
	return snipermingosSpawnRate;
}



int Spawner::GetWannamingosPerWave()
{
	return wannamingosPerWave;
}

int Spawner::GetGigamingosPerWave()
{
	return gigamingosPerWave;
}

int Spawner::GetSpeedamingosPerWave()
{
	return speedamingosPerWave;
}

int Spawner::GetSnipermingosPerWave()
{
	return snipermingosPerWave;
}