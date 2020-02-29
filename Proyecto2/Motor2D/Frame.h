#ifndef __FRAME_H__
#define __FRAME_H__

#include "Module.h"
#include "SDL/include/SDL_rect.h"

enum class COLLIDER_TYPE;

class Frame
{
public:
	float maxFrames;

	SDL_Rect frame;

	int pivotPositionX;
	int pivotPositionY;

private:
	int lastCollider = 0;

public:


};

#endif
