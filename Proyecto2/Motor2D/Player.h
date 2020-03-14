#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_mouse.h"
#include "Module.h"
#include "vector"

class Hero;
class Entity;

class ModulePlayer : public Module
{
public:
	ModulePlayer();

	//Destructor
	~ModulePlayer();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

private:
	//Handles Player Input
	bool Select() { return true; };
	bool Click();
	bool HandleInput();

private:
	std::vector <Hero*> heroesVector;
	Entity* focusedEntity;
};


#endif __PLAYER_H__