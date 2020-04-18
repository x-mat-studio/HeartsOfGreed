#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "Module.h"
#include "Fonts.h"

#include <list>

struct SDL_Texture;
class UI;
class UI_Portrait;
class Entity;
class UI_Healthbar;
class Base;
enum class UI_TYPE;
class Entity;

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

	UI* AddUIElement(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, Entity* entity = nullptr, DRAGGABLE draggable = DRAGGABLE::DRAG_OFF,
		char* text = nullptr, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = nullptr);
	UI* AddButton(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, EVENT_ENUM eventR, bool menuClosure = false, bool includeFather = false,
		bool hiding = false, bool hoverMove = false, DRAGGABLE draggable = DRAGGABLE::DRAG_OFF, EVENT_ENUM eventTrigger = EVENT_ENUM::NULL_EVENT);
	UI* AddScrollbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, EVENT_ENUM eventR, int maxValue = 100.00f,
		EVENT_ENUM eventTrigger = EVENT_ENUM::NULL_EVENT, DRAGGABLE draggable = DRAGGABLE::DRAG_OFF);
	SDL_Texture* GetAtlasTexture() const;

	void CreateBasicInGameUI();
	void CreateMainMenu();
	void CreateOptionsMenu();
	void CreateEntityPortrait();
	void CreateEntityPortraitChilds();
	void CreateShopMenu();
	void StopAll(UI* element, bool reposition, bool hidden, bool hidden_unhiding);
	void UpdateFocusPortrait();

	SDL_Rect RectConstructor(int x, int y, int w, int h);

	void DeleteUIChilds(UI* father, bool includeFather, UI_TYPE typeToDelete);
	void HideElements(UI* father, float dt);

	bool CleanUp();
	bool MouseOnUI(iMPoint& mouse);

	void LoadAtlas();
	UI* FindUIByName(char* name);

private:



	void CreatePauseMenu();
	void ExecuteEvent(EVENT_ENUM eventId);

	void DisableHealthBars();
	void CheckFocusEntity();

public:

	int hoverSound;
	int clickSound;

	Base* lastShop;

private:

	std::vector<UI*> uiVector;
	SDL_Texture* atlas;
	UI_Portrait* portraitPointer;
	Entity* focusedEnt;
	UI* focusedPortrait;
	

};

#endif //__UIMANAGER_H__
