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


//returns true if the event has been registered or if the new listener has been added, else returns false
bool ModuleEventManager::EventRegister(EVENT_ENUM event, Module* mod)
{
	bool ret = false;

	//if event isn't on the map create a new entry on the map
	if (!eventListenersMap.count(event))
	{
		CreateEventOnMap(event);
		eventListenersMap[event].push_back(mod);
		ret = true;
	}
	//if listener has already been added don't add
	//else add a new listener
	else if (!FindListener(event, mod))
	{
		eventListenersMap[event].push_back(mod);
		ret = true;
	}


	return ret;
}


void ModuleEventManager::CreateEventOnMap(EVENT_ENUM event)
{
	//further testing in this function is needed TODO
	std::vector<Module*> vec;
	eventListenersMap[event] = vec;
}


//returns true if a listener is already in the vector
bool ModuleEventManager::FindListener(EVENT_ENUM event, Module* mod)
{
	bool ret = false;


	for (int i = 0; i < eventListenersMap[event].size(); i++)
	{


		if (eventListenersMap[event].at(i) == mod)
		{
			ret = true;
		}


	}


	return ret;
}
