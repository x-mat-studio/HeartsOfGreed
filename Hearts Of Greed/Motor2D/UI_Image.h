#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

#include "UI.h"

class UI_Image : public UI
{
public:

	UI_Image(float x, float y, UI* parent, SDL_Rect rect, SDL_Texture* texture, bool dragable);
	~UI_Image();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	void CleanUp();

};

#endif//__UI_IMAGE_H__