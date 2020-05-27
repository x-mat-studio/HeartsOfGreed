#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "Module.h"
#include "Fonts.h"
#include "EasingFunctions.h"

#include <list>

struct SDL_Texture;
struct UIFactory;

class UI_Group;
class Base;

class UI;
class Button;
class HeroesPortraitManager;
class HeroPortrait;

class Entity;
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
	UIFactory* ModuleUIManager::GetFactory() const;

	void AddUIGroup(UI_Group* element);

	bool DeleteUIGroup(GROUP_TAG tag);

	void LoadAtlas();

	bool MouseOnUI();

	void CheckFocusEntity(Entity* entity);

	void CheckDragElement(UI* element);

	void ExecuteButton(BUTTON_TAG tag, Button* button);
	void ExecuteHoverButton(BUTTON_TAG tag, Button* button);

	HeroesPortraitManager* GetPortraitManager();
	void SetPortraitManager(HeroesPortraitManager* portraitManager);

	void AddPortrait(Hero* portrait);
	void RemovePortrait(Hero* portrait);

	void PlayHoverSound();
	void PlayClickSound();

	//Pause Game volume logic

	void LowerVolumeOnPause();
	void RaiseVolumeOnUnpause();

	bool CheckGroupTag(GROUP_TAG tag);

private:

	void ExecuteEvent(EVENT_ENUM eventId);

	void CheckFocusedUI();

	UI* SearchFocusUI() const;

	void DragElement();

	void UnregisterEvents();

	void AddPendingPortraits();

	void AugmentValueByTenPercent(float* value);

	void BasicResourceManagement(EVENT_ENUM eventN, float* cost);
	void StatsUpgradeResourceManagement(EVENT_ENUM eventN, float* cost);
	void SkillResourceManagement();

	bool Save(pugi::xml_node& data) const;

	bool Load(pugi::xml_node& data);

public:
	Base* lastShop;
	bool mouseOverUI;
	UIFactory* factory;

private:
	std::vector<UI_Group*> uiGroupVector;
	std::vector<HeroPortrait*> portraitsToAdd;
	
	SDL_Texture* atlas;

	UI* dragElement;
	iMPoint dragMouse;

	// DO NOT USE THIS IF YOU AREN'T FERRAN >:3
	UI* hoverElement;

	HeroesPortraitManager* portraitManager;

	int hoverSound;
	int clickSound;
	int easyIn;
	int easyOut;

	bool isMenuOn;

	bool lastFramePauseEasingActive;
	bool goingToPause;
	Easing pauseAnimPosX;
	Easing pauseAnimPosY;
	Easing pauseAnimScale;
	Easing pauseAnimAlpha;
	Easing pauseAnimRectAlpha;
};

#endif //__UIMANAGER_H__
