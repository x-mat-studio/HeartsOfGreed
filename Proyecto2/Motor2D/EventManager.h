#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

#include "SDL/include/SDL.h"
#include "Module.h"
#include "vector"
#include <map>

class ModuleEventManager : public Module
{
public:

	ModuleEventManager();
	virtual ~ModuleEventManager();


	bool Awake(pugi::xml_node&);
	bool Start();

	bool CleanUp();

private:
	//	std::map<> eventMap;
};

#endif //__EVENTMANAGER_H__
