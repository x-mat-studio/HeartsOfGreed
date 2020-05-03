#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "Module.h"
#include "Fonts.h"

#include <list>

struct SDL_Texture;
struct UIFactory;

class UI_Group;
class Base;

class UI;
class Button;
class HeroesPortraitManager;
class HeroPortrait;

class Hero;

enum class GROUP_TAG;
enum class BUTTON_TAG;


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

	bool CleanUp();

	SDL_Texture* GetAtlasTexture() const;

	void AddUIGroup(UI_Group* element);

	bool DeleteUIGroup(GROUP_TAG tag);

	void LoadAtlas();

	bool MouseOnUI();

	void CheckFocusEntity();

	void CheckDragElement(UI* element);

	void ExecuteButton(BUTTON_TAG tag, Button* button);

	HeroesPortraitManager* GetPortraitManager();
	void SetPortraitManager(HeroesPortraitManager* portraitManager);

	void AddPortrait(Hero* portrait);
	void RemovePortrait(Hero* portrait);

private:
	void ExecuteEvent(EVENT_ENUM eventId);

	bool CheckGroupTag(GROUP_TAG tag);

	UI* SearchFocusUI() const;

	void DragElement();

	void UnregisterEvents();


public:
	Base* lastShop;
	bool mouseOverUI;

private:

	std::vector<UI_Group*> uiGroupVector;
	SDL_Texture* atlas;

	UI* dragElement;
	iMPoint dragMouse;

	UIFactory* factory;

	HeroesPortraitManager* portraitManager;

	bool isMenuOn;
};

#endif //__UIMANAGER_H__
