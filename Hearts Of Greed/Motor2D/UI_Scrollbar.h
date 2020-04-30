#ifndef __UI_SCROLLBAR_H__
#define __UI_SCROLLBAR_H__

#include "UI.h"

class UI_Image;

class UI_Scrollbar : public UI
{
public:

	UI_Scrollbar(float x, float y, UI* parent, SDL_Rect rect, SDL_Texture* texture, int maxValue);
	~UI_Scrollbar();

private:

	void Move();

private:

	int maxValue;
};

#endif//__UI_SCROLLBAR_H__
