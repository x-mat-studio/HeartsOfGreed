#pragma once

#ifndef __AI_H__
#define __AI_H__

#define ENEMIES_PER_NIGHT 5
#define SPAWNERS_TO_ACTIVATE 2

#include "Module.h"
#include "SDL/include/SDL_rect.h"

#include "vector"
#include "map"

class Base;
class Spawner;

class ModuleAI : public Module
{
public:

	ModuleAI();
	~ModuleAI();

	bool Start();

	bool PostUpdate(float dt);

	bool CleanUp();

	void OnCollision(Collider*, Collider*);

	fMPoint* GetObjective();
	
	void PushBase(Base* building);
	void PushSpawner(Spawner* spawner);

	void ResetAI();
private:
	void CreateSelectionCollider(Collider*);
	void ExecuteEvent(EVENT_ENUM eventId);

	int CheckBaseAligmentAttack();

	void CommandSpawners();
	void FindNearestSpawners(std::multimap<int, Spawner*>* spawners);
	int CalculateEnemiesToSpawn(int numberOfSpawners);
	int CalculateSpawnersToActivate();
	
private:
	fMPoint objectivePos;
	std::vector<Base*> baseVector;
	std::vector<Spawner*> spawnerVector;

};


#endif//__AI_H__
