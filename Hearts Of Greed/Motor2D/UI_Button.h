#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "UI.h"
#include "Input.h"
#include "UIManager.h"


class UI_Button : public UI
{

public:

	UI_Button(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable);
	~UI_Button();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	bool OnAbove();
	
	void OnClick();
	void MovingIt(float dt);

private:

	void HandleInput();
	
private:

	fMPoint accuratedDrag;

};

#endif // UI_BUTTON_H
