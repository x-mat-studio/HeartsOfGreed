#ifndef __UI_PORTRAIT_H__
#define __UI_PORTRAIT_H__

#include "UI.h"

class UI_Portrait : public UI
{

public:

	UI_Portrait(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool draggable);
	~UI_Portrait();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

private:

	void HandleInput();

private:



};

#endif // UI_HEALTHBAR_H
