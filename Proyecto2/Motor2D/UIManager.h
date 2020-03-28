#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "SDL/include/SDL.h"
#include "Module.h"
#include "UI.h"
#include <list>



class ModuleUIManager : public Module
{
public:

	ModuleUIManager();
	~ModuleUIManager();


	bool Awake(pugi::xml_node&);
	bool Start();


	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);


	bool CleanUp();

public:


private:

	SDL_Texture* atlas;

};

#endif //__UIMANAGER_H__
