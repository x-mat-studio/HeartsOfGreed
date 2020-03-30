#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "UI.h"

class UI_Text : public UI

{

public:

	UI_Text(fMPoint positionValue, UI* father, UI_TYPE uiType, P2SString uiName, bool draggable);
	~UI_Text();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt, SDL_Texture* texture);

private:

	void HandleInput();

};

#endif // UI_TEXT_H
