#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "UI.h"
#include "Input.h"
#include "UIManager.h"


class UI_Button : public UI
{

public:

	UI_Button(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, EVENT_ENUM eventR, bool menuClosure = false, bool includeFather = false,
		bool hidingVar = false, bool hoverMove = false, DRAGGABLE draggable = DRAGGABLE::DRAG_OFF, EVENT_ENUM eventTrigger = EVENT_ENUM::NULL_EVENT);
	~UI_Button();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	
	void OnClick(float dt);
	void HoverFeedback();
	void MovingIt(float dt);
	

private:

	void HandleInput();
	void CloseMenu();
	
private:

	bool hoverSound;

	ButtonProperties properties;
	fMPoint accuratedDrag;
	EVENT_ENUM eventRecieved;
	EVENT_ENUM eventTriggerer;

};

#endif // UI_BUTTON_H
