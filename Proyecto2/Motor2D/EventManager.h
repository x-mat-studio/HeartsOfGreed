#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

#include "SDL/include/SDL.h"
#include "Module.h"
#include "vector"
#include <map>

enum class EVENT_ENUM
{
	AUDIO_INCOMES,
	KEYBOARD_INPUT,
	MOUSE_INPUT,
	PAUSE_GAME,
	NULL_EVENT
};

class Event {

public:

	Event();
	Event(EVENT_ENUM& idValue, EVENT_ENUM& idTriggerValue);
	~Event();

private:

	EVENT_ENUM id;
	EVENT_ENUM idTrigger;
};

class ModuleEventManager : public Module
{
public:
	
	ModuleEventManager();
	virtual ~ModuleEventManager();

	bool EventRegister(EVENT_ENUM event, Module* mod);

	bool Awake(pugi::xml_node&);
	bool Start();

	bool CleanUp();

private:
	void CreateEventOnMap(EVENT_ENUM event);
	bool FindListener(EVENT_ENUM event,Module*mod);

private:
	std::map<EVENT_ENUM,std::vector<Module*>> eventListenersMap;
};

#endif //__EVENTMANAGER_H__
