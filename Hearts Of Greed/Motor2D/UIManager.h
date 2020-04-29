#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "Module.h"
#include "Fonts.h"

#include <list>

struct SDL_Texture;
class UI_Group;
class UI;
enum class GROUP_TAG;
class Base;


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

	void LoadAtlas();

	bool MouseOnUI();

	void CheckFocusEntity();

private:
	void ExecuteEvent(EVENT_ENUM eventId);


	bool CheckGroupTag(GROUP_TAG tag);

	UI* SearchFocusUI() const;

	void DragElement();
	void CallElementDrag();
	

	void UnregisterEvents();

public:
	Base* lastShop;
	bool mouseOverUI;

private:

	std::vector<UI_Group*> uiGroupVector;
	SDL_Texture* atlas;

	UI* dragElement;
	iMPoint dragMouse;

	bool isMenuOn;
};

#endif //__UIMANAGER_H__
