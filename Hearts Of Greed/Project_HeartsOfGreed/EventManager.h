#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

#include "SDL/include/SDL.h"
#include "Module.h"
#include <map>
#include <list>

enum class EVENT_ENUM;

class Event 
{

public:

	Event();
	Event(EVENT_ENUM idValue, EVENT_ENUM idTriggerValue);
	~Event();

	EVENT_ENUM id;
	EVENT_ENUM idTrigger;
};

class ModuleEventManager : public Module
{
public:
	
	ModuleEventManager();
	virtual ~ModuleEventManager();

	void GenerateEvent(EVENT_ENUM eventId, EVENT_ENUM eventTriggerId);
	bool EventRegister(EVENT_ENUM event, Module* mod);
	bool EventUnRegister(EVENT_ENUM event, Module* mod);

	void CleanListenerMap();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool CleanUp();

private:
	void CreateEventOnMap(EVENT_ENUM event);
	void FireEvent(EVENT_ENUM eventId) const;
	EVENT_ENUM CheckEventTrigger(EVENT_ENUM eventTrigger);
	int FindListener(EVENT_ENUM event,Module*mod);
	std::vector<Module*>::iterator EraseListener(EVENT_ENUM event, Module* mod, int vecId);


private:
	std::map<EVENT_ENUM,std::vector<Module*>> eventListenersMap;
	std::vector<Event> eventVector;
};

#endif //__EVENTMANAGER_H__
