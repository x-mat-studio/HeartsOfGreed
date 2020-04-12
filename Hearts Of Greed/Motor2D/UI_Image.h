#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

#include "UI.h"

class UI_Image : public UI
{

public:

	UI_Image(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable);
	~UI_Image();

	bool Update(float dt);
	bool PostUpdate(float dt);

private:

	void HandleInput();

};

#endif // UI_IMAGE_H