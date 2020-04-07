#ifndef __UI_SCROLLBAR_H__
#define __UI_SCROLLBAR_H__

#include "UI.h"
#include "Input.h"
#include "UIManager.h"
#include "UI_Button.h"


class UI_Scrollbar : public UI
{

public:

	UI_Scrollbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable);
	~UI_Scrollbar();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

private:

	void ScrollLimit();
	void HandleInput();

private:
	UI* scrollButton;


};

#endif // UI_BUTTON_H

