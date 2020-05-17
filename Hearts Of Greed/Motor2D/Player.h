#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_mouse.h"
#include "Module.h"

#include "Entity.h"


class Hero;
class Base;
struct skillArea;

class ModulePlayer : public Module
{
public:
	ModulePlayer();

	~ModulePlayer();

	bool Awake(pugi::xml_node&);

	bool Start();
	bool CleanUp();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void AddResources(int gain);
	void AddResourcesSkill(int gain);
	void AddResourcesBoost(int gain);
	bool UseResources(int cost);
	bool UseResourcesSkill(int cost);
	bool UseResourcesBoost(int cost);

	bool ActivateBuildMode(ENTITY_TYPE building, Base* contrBase);
	void DesactivateBuildMode();

	void RemoveHeroFromVector(Hero* hero);
	void CheckFocusedEntity(Entity* entity);

	iMPoint GetClickPosition();

	int GetResources() const;
	int GetResourcesSkill() const;
	int GetResourcesBoost() const;

	int GetTurretCost() const;

	bool IsBuilding() const;

	Entity* GetFocusedEntity();

	bool SetMenuState(bool menuState);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:
	
	bool HandleInput();

	void Select();
	bool Click();
	void LeftClick();
	void RightClick();

	void CommandSkill();
	void PrepareHeroSkills();
	void DoHeroSkills();

	bool BuildClick();
	void SubstractBuildResources();

	void ExecuteEvent(EVENT_ENUM eventId);

	void DrawSelectQuad();

	bool CheckFocusedHero();

public:
	Entity* focusedEntity; //other modules need this for portrait generation
	bool doingAction;

private:
	std::vector <Hero*> heroesVector;
	
	ENTITY_TYPE buildingToBuild;

	iMPoint clickPosition;

	SDL_Rect selectRect;

	bool selectUnits;
	bool entityComand;
	bool entityInteraction;
	
	int focusedHero;

	bool skill1;
	bool skill2;
	bool skill3;
	bool prepareSkill;

	bool doSkill;

	bool hasClicked;
	bool buildMode;
	bool UIMenuOn;

	int resources;
	int resourcesSkill;
	int resourcesBoost;

	skillArea* constrAreaInfo;
	std::vector <iMPoint> constrArea;
	fMPoint baseDrawCenter;
	iMPoint buildingPrevPosition;
	Base* baseInBuild;


	int turretCost;

	int buildAreaRadius;
};


#endif //__PLAYER_H__
