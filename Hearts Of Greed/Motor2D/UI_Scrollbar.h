#ifndef __UI_SCROLLBAR_H__
#define __UI_SCROLLBAR_H__

#include "UI.h"
#include "Input.h"
#include "UIManager.h"
#include "UI_Button.h"


class UI_Scrollbar : public UI
{

public:

	UI_Scrollbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable, float maxValue = 100.00f);
	~UI_Scrollbar();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	float GetScrollValue();

private:

	bool GenerateScrollButton();

	void ScrollLimit();

private:
	UI* scrollButton;

	bool generatedButton;

	float maxValue;

};

#endif // UI_BUTTON_H

