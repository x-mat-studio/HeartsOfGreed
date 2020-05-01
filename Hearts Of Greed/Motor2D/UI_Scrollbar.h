#ifndef __UI_SCROLLBAR_H__
#define __UI_SCROLLBAR_H__

#include "UI.h"

class UI_Image;

class UI_Scrollbar : public UI
{
public:

	UI_Scrollbar(float x, float y, UI* parent, SDL_Rect rect, SDL_Texture* texture, float maxValue);
	~UI_Scrollbar();

private:

	void HandleInput();
	void Move();
	float PositionToValue();
	float ValueToPosition(float value);

private:

	float previousX;
	float currentValue;
	float maxValue;
};

#endif//__UI_SCROLLBAR_H__
