#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "UI.h"
#include "UIManager.h"

class UI_Button : public UI
{

public:

	UI_Button(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool draggable);
	~UI_Button();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

private:

	void HandleInput();


};

#endif // UI_BUTTON_H
