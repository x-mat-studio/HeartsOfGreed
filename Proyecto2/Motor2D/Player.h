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

private:
	std::vector <Hero*> heroesVector;
	Entity* focusedEntity;

	SDL_Point clickPosition;

};

class PlayerListener : public Listener
{
private:

	void onNotify(const Module& module, Evento event);

};

class PlayerSpeaker : public Speaker
{
private:
	Listener* listeners_[MAX_LISTENERS]; //Array, we're not expecting to add/remove a lot

	int numListeners;

};

#endif __PLAYER_H__