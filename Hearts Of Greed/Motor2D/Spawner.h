#pragma once

#ifndef __SPAWNER_H__
#define __SPAWNER_H_

#include "Entity.h"


class Spawner : public Entity
{
public:

	Spawner(fMPoint position, ENTITY_TYPE spawnerType, Collider* col, int maxHitPoints, int currentHitPoints);
	Spawner(fMPoint position, Spawner* copy);

	void Spawn();
	void SetNumberToSpawn(int number);

	bool PreUpdate(float dt);

private:

	ENTITY_TYPE spawnerType;
	int entitysToSpawn;

};




#endif //__SPAWNER_H_
