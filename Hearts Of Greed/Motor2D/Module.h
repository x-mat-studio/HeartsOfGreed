// ----------------------------------------------------
// Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __MODULE_H__
#define __MODULE_H__

#define MAX_COLLIDERS_PER_FRAME 50

#include "p2SString.h"
#include <vector>
#include "PugiXml\src\pugixml.hpp"
#include "p2Point.h"

struct Collider;

class App;

enum class EVENT_ENUM
{
	AUDIO_INCOMES,
	KEYBOARD_INPUT,
	MOUSE_INPUT,
	SELECT_UNITS,
	STOP_SELECTING_UNITS,
	ENTITY_COMMAND, //aka right click
	ENTITY_INTERACTION, //aka left click
	HERO_CHANGE_FOCUS,

	SKILL1,
	SKILL2,
	SKILL3,

	ENTITY_DEAD,

	// Debug events
	DEBUG_DAY,
	DEBUG_NIGHT,

	GIVE_RESOURCES,

	KILL_ALL_ENEMIES,
	ACTIVATE_GODMODE_HEROES,
	DESACTIVATE_GODMODE_HEROES,

	SPAWN_GATHERER_HERO,
	SPAWN_RANGED_HERO,
	SPAWN_MELEE_HERO,
	SPAWN_ENEMY,
	SPAWN_BASE,
	SPAWN_TURRET,
	SPAWN_BUILDING,

	// Hero creation /revival
	HERO_MELEE_CREATED,
	HERO_GATHERER_CREATED,
	HERO_RANGED_CREATED,

	// Heroe dies
	HERO_MELEE_OUT,
	HERO_GATHERER_OUT,
	HERO_RANGED_OUT,

	// Hero level up
	GATHERER_RESURRECT,
	RANGED_RESURRECT,
	MELEE_RESURRECT,

	//Turret Shop Purchase
	TURRET_PURCHASED,

	NIGHT_START,
	DAY_START,
	

	PLAYER_CONQUERED_A_BASE,
	ENEMY_CONQUERED_A_BASE,

	// Camera related events
	CAMERA_UP,
	STOP_CAMERA_UP,
	CAMERA_DOWN,
	STOP_CAMERA_DOWN,
	CAMERA_RIGHT,
	STOP_CAMERA_RIGHT,
	CAMERA_LEFT,
	STOP_CAMERA_LEFT,
	CAMERA_SPRINT,
	STOP_CAMERA_SPRINT,

	START_GAME,
	START_GAME_FROM_CONTINUE,
	PAUSE_GAME,
	UNPAUSE_GAME,
	UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU,
	SAVE_GAME,
	LOAD_GAME,
	EXIT_GAME,

	OPTION_MENU,
	FULLSCREEN_INPUT,
	MUSIC_ADJUSTMENT,
	SFX_ADJUSTMENT,
	CREDIT_MENU,

	GAME_SCENE_STARTED,
	RETURN_TO_MAIN_MENU,

	//Portrait related
	ENTITY_ON_CLICK,
	CREATE_SHOP,

	NULL_EVENT
};

class Module
{

public:

	Module();

	void Init();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&);

	// Called before the first frame
	virtual bool Start();

	// Called each loop iteration
	virtual bool PreUpdate(float dt);

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called each loop iteration
	virtual bool PostUpdate(float dt);

	// Called before quitting
	virtual bool CleanUp();

	virtual bool Load(pugi::xml_node&);

	virtual bool Save(pugi::xml_node&) const;

	// Module activation 
	bool IsEnabled() const;

	void Enable();

	void Disable();


	virtual void OnCollision(Collider*, Collider*);

	// Event managing: saves an event into the listener vector

	void AddEvent(EVENT_ENUM eventId);

	void RemoveEvent(EVENT_ENUM eventId);

	bool CheckListener(Module* mod);

	virtual void ExecuteEvent(EVENT_ENUM eventId);

public:

	P2SString				name;
	std::vector<EVENT_ENUM>	listener;

protected:
	bool					enabled;


};

#endif // __MODULE_H__