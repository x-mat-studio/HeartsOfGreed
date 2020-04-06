#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "Module.h"

#include <list>

struct SDL_Texture;
class UI;
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
