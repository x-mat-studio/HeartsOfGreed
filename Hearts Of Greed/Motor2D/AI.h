#pragma once

#ifndef __AI_H__
#define __AI_H__

#define ENEMIES_PER_NIGHT 6

#include "Module.h"
#include "SDL/include/SDL_rect.h"

#include "vector"

class Base;
class Spawner;

class ModuleAI : public Module
{
public:

	ModuleAI();
	~ModuleAI();

	bool Awake(pugi::xml_node&);

	bool PostUpdate(float dt);

	void OnCollision(Collider*, Collider*);

	fMPoint* GetObjective();
	
	void PushBase(Base* building);
	void PushSpawner(Spawner* spawner);

private:
	void CreateSelectionCollider(Collider*);
	void ExecuteEvent(EVENT_ENUM eventId);

	int CheckBaseAligmentAttack();

	void CommandSpawners();
	Spawner* FindNearestSpawner();
	int CalculateEnemiesToSpawn();
	
private:
	fMPoint objectivePos;
	std::vector<Base*> baseVector;
	std::vector<Spawner*> spawnerVector;

};


#endif//__AI_H__
