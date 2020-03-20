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

class ModuleEventManager : public Module
{
public:
	
	ModuleEventManager();
	virtual ~ModuleEventManager();


	bool Awake(pugi::xml_node&);
	bool Start();

	bool CleanUp();

private:
	std::map<EVENT_ENUM,std::vector<Module*>> eventListenersMap;
};

#endif //__EVENTMANAGER_H__
