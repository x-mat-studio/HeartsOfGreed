#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "Module.h"
#include <list>

struct SDL_Texture;
class UI;
enum class UI_TYPE;

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

	UI* AddUIElement(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool dragable, char* text = nullptr);
	SDL_Texture* GetAtlasTexture() const;
	void RemoveDeletedUI();

	bool CleanUp();

private:

	SDL_Rect RectConstructor(int x, int y, int w, int h);

	void ExecuteEvent(EVENT_ENUM eventId);
	void CreateBasicUI();

private:

	std::vector<UI*> uiVector;
	SDL_Texture* atlas;

};

#endif //__UIMANAGER_H__
