#pragma once

#ifndef __AI_H__
#define __AI_H__


//PLACEHOLDER SHIT
#define ENEMIES_PER_NIGHT 5
#define SPAWNERS_TO_ACTIVATE 2


#define Nº_TYPE_OF_ENEMIES 4

#include "Module.h"
#include "SDL/include/SDL_rect.h"

#include "vector"
#include "map"

class Base;
class Spawner;

enum class ENTITY_ALIGNEMENT;

class ModuleAI : public Module
{
public:

	ModuleAI();
	~ModuleAI();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PostUpdate(float dt);

	bool CleanUp();

	void OnCollision(Collider*, Collider*);

	fMPoint* GetObjective(fMPoint& pos);
	
	void PushBase(Base* building);
	void PushSpawner(Spawner* spawner);

	void ResetAI();
private:
	void CreateSelectionCollider(Collider*);
	void ExecuteEvent(EVENT_ENUM eventId);

	int CheckBaseAligmentAttack();

	void CommandSpawners();
	void FindNearestSpawners(std::multimap<int, Spawner*>* spawners);

	int CalculateWanamingoToSpawn();
	int CalculateGigamingoToSpawn();
	int CalculateSnipermingoToSpawn();
	int CalculateSpeedomingoToSpawn();

	void AssignWana(std::multimap<int, Spawner*> spawners, std::multimap<int, Spawner*>::iterator iterator, int tospawn, int spawnersaviable);
	void AssignGiga(std::multimap<int, Spawner*> spawners, std::multimap<int, Spawner*>::iterator iterator, int tospawn, int spawnersaviable);
	void AssignSpeed(std::multimap<int, Spawner*> spawners, std::multimap<int, Spawner*>::iterator iterator, int tospawn, int spawnersaviable);
	void AssignSniper(std::multimap<int, Spawner*> spawners, std::multimap<int, Spawner*>::iterator iterator, int tospawn, int spawnersaviable);

	int CalculateSpawnersToActivate();

	void CheckEndGame(ENTITY_ALIGNEMENT align);
	
private:
	fMPoint objectivePos;
	std::vector<fMPoint> alarmObjectivePos;

	std::vector<Base*> baseVector;
	std::vector<Spawner*> spawnerVector;

	std::vector<std::vector<int>> nightEnemyInfo;
};


#endif//__AI_H__
