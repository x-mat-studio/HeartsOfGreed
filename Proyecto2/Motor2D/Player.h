#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_mouse.h"
#include "Module.h"

class Hero;
class Entity;

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

private:
	
	void Select();
	bool Click();
	void RightClick();
	bool HandleInput();
	void ExecuteEvent(EVENT_ENUM& eventId);

	void DrawSelectQuad();

private:
	std::vector <Hero*> heroesVector;
	Entity* focusedEntity;

	iMPoint clickPosition;

	SDL_Rect selectRect;

	bool selectUnits=false;
	bool entityComand=false;
	bool entityInteraction=false;
};



#endif __PLAYER_H__