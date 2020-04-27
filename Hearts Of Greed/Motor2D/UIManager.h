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
class UI_Text;
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

	bool CleanUp();

	SDL_Texture* GetAtlasTexture() const;

	void AddUIElement(UI* element);

	bool MouseOnUI(iMPoint& mouse);

	void LoadAtlas();

private:
	void ExecuteEvent(EVENT_ENUM eventId);
	void CheckFocusEntity();

	void UnregisterEvents();

public:
	Base* lastShop;

private:

	std::vector<UI*> uiVector;
	SDL_Texture* atlas;

	bool isMenuOn;
};

#endif //__UIMANAGER_H__
