#include "App.h"
#include "EventManager.h"

Event::Event()
{
	id;
	idTrigger;
}

Event::Event(EVENT_ENUM& idValue, EVENT_ENUM& idTriggerValue)
{
	id = idValue;
	idTrigger = idTriggerValue;
}

Event::~Event()
{
}

ModuleEventManager::ModuleEventManager()
{
	name.create("EventyManager");
}


// Destructor
ModuleEventManager::~ModuleEventManager()
{}


// Called before render is available
bool ModuleEventManager::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}


// Called before the first frame
bool ModuleEventManager::Start()
{
	bool ret = true;


	return ret;
}

//// Called before quitting
bool ModuleEventManager::CleanUp()
{

	return true;
}
