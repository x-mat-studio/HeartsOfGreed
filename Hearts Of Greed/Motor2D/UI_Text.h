#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "UI.h"
#include "Fonts.h"

class UI_Text : public UI

{
public:

	UI_Text(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable, char* text, SDL_Color color);
	~UI_Text();

	bool PostUpdate(float dt);

private:

	void HandleInput();

};

#endif // UI_TEXT_H
