#ifndef __UI_HERO_H__
#define __UI_HERO_H__

#include "UI.h"

class UI_Hero : public UI
{

public:

	UI_Hero(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool draggable);
	~UI_Hero();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

private:

	void HandleInput();

private:



};

#endif // UI_HEALTHBAR_H
