#ifndef __FRAME_H__
#define __FRAME_H__

#include "Module.h"
#include "SDL/include/SDL_rect.h"

class Frame
{
public:
	float maxFrames = 0.f;

	SDL_Rect frame = {0,0,0,0};

	int pivotPositionX = 0;
	int pivotPositionY = 0;

private:
	int lastCollider = 0;

public:


};

#endif
