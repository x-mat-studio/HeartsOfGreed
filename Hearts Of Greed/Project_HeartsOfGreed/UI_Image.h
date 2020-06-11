#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

#include "UI.h"

class UI_Image : public UI
{
public:

	UI_Image(float x, float y, UI* parent, SDL_Rect rect, SDL_Texture* texture, bool dragable, bool interactable = false);
	~UI_Image();

};

#endif//__UI_IMAGE_H__