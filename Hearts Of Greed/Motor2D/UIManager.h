#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "Module.h"

#include <list>

struct SDL_Texture;
class UI;
class UI_Portrait;
enum class UI_TYPE;

enum class DRAGGABLE
{
	DRAG_OFF = 0,
	DRAG_X = 1,
	DRAG_Y = 2,
	DRAG_XY = 3,

	DRAG_MAX = NULL
};

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

	UI* AddUIElement(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable = DRAGGABLE::DRAG_OFF, char* text = nullptr, SDL_Color color = { 255, 255, 255, 255 });
	UI* AddButton(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, EVENT_ENUM eventR, bool menuClosure = false, bool includeFather = false, DRAGGABLE draggable = DRAGGABLE::DRAG_OFF, EVENT_ENUM eventTrigger = EVENT_ENUM::NULL_EVENT);
	SDL_Texture* GetAtlasTexture() const;

	void CreateBasicInGameUI();
	void CreateMainMenu();

	SDL_Rect RectConstructor(int x, int y, int w, int h);

	void DeleteUI(UI* father, bool includeFather);

	bool CleanUp();

	void LoadAtlas();

private:

	UI* FindUIByName(char* name);

	void CreatePauseMenu();
	void ExecuteEvent(EVENT_ENUM eventId);

private:

	std::vector<UI*> uiVector;
	SDL_Texture* atlas;
	UI_Portrait* portraitPointer;

};

#endif //__UIMANAGER_H__
