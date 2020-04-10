#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_mouse.h"
#include "Module.h"

#include "Entity.h"

class Hero;

class ModulePlayer : public Module
{
public:
	ModulePlayer();


	~ModulePlayer();


	bool Awake(pugi::xml_node&);


	bool Start();


	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void AddResources(int gain);
	bool UseResources(int cost);

	bool ActivateBuildMode(ENTITY_TYPE building);
	void DesactivateBuildMode();

	void RemoveHeroFromVector(Hero* hero);

private:
	
	bool HandleInput();

	void Select();
	bool Click();
	void RightClick();

	void CommandSkill();
	void PrepareHeroSkills();
	void DoHeroSkills();

	bool BuildClick();

	void ExecuteEvent(EVENT_ENUM eventId);

	void DrawSelectQuad();

private:
	std::vector <Hero*> heroesVector;
	Entity* focusedEntity;

	ENTITY_TYPE buildingToBuild;

	iMPoint clickPosition;

	SDL_Rect selectRect;

	bool selectUnits;
	bool entityComand;
	bool entityInteraction;

	bool skill1;
	bool skill2;
	bool skill3;
	bool prepareSkill;

	bool doSkill;

	bool buildMode;

	int resources;
};


#endif //__PLAYER_H__
