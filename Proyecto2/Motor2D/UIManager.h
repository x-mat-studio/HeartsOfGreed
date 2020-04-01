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

	UI* AddUIElement(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool dragable);
	void RemoveDeletedUI();

	bool CleanUp();

private:

	SDL_Rect RectConstructor(int x, int y, int w, int h);

	void ExecuteEvent(EVENT_ENUM eventId);

private:

	std::vector<UI*> uiVector;
	SDL_Texture* atlas;

};

#endif //__UIMANAGER_H__
