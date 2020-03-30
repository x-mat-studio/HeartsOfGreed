#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "Module.h"
#include "UI.h"
#include <list>

class UI;

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

	UI* AddUIElement(fMPoint positionValue, UI* father, UI_TYPE uiType, P2SString uiName);
	void RemoveDeletedUI();

	bool CleanUp();

public:


private:

	std::vector<UI*> uiVector;
	SDL_Texture* atlas;

};

#endif //__UIMANAGER_H__
