#pragma once

#ifndef __AI_H__
#define __AI_H__

#include "Module.h"
#include "SDL/include/SDL_rect.h"

#include "vector"

class Building;

class ModuleAI : public Module
{
public:

	ModuleAI();
	~ModuleAI();

	bool Awake(pugi::xml_node&);

	void OnCollision(Collider*, Collider*);

	fMPoint* GetObjective();
	
	void PushBase(Building* building);

private:
	void CreateSelectionCollider(Collider*);
	void ExecuteEvent(EVENT_ENUM eventId);

	int CheckBaseAligmentAttack();
	
private:
	fMPoint objectivePos;
	std::vector<Building*> baseVector;

};


#endif//__AI_H__
