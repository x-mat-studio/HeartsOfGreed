#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Frame.h"
#include "SDL/include/SDL_rect.h"
#include "j1App.h"

#define MAX_FRAMES 55

class Animation
{
public:
	bool loop = true;
	Frame frames[MAX_FRAMES];

private:
	float frames_passed = 0;
	int current_frame = 0;
	int last_frame = 0;
	int loops = 0;

public:

	Animation PushAnimation(pugi::xml_node node, p2SString name) {
		
		Animation aux;

		pugi::xml_node iterator = node.child("Animations").first_child();
	
		for (iterator; iterator; iterator = iterator.next_sibling())
		{
			if (name == iterator.attribute("name").as_string())
			{
				pugi::xml_node iteratorFrames = iterator.first_child();
				for (int y = 0; y < iterator.attribute("frames").as_int(); y++)
				{
					SDL_Rect rect;
					rect.x = iteratorFrames.attribute("rposX").as_int();
					rect.y = iteratorFrames.attribute("rposY").as_int();
					rect.w = iteratorFrames.attribute("rwidth").as_int();
					rect.h = iteratorFrames.attribute("rheight").as_int();

					aux.PushBack(rect, iteratorFrames.attribute("time").as_int(),  iteratorFrames.attribute("pivotX").as_int(), iteratorFrames.attribute("pivotY").as_int());
					
					iteratorFrames = iteratorFrames.next_sibling();
				}
				break;
			}
		}
		aux.current_frame = 0;
		return aux;
	}

	
	void PushBack(const SDL_Rect& rect, const int maxFrames,int pivotPositionX, int pivotPositionY) {
		
		frames[last_frame].frame = rect;
		frames[last_frame].maxFrames = maxFrames;
		frames[last_frame].pivotPositionX = pivotPositionX;
		frames[last_frame].pivotPositionY = pivotPositionY;


		last_frame++;
	}

	Frame& GetCurrentFrame(float dt)
	{
		if (frames_passed * dt < frames[current_frame].maxFrames * dt) {
			frames_passed += dt;
		}
		else {
			frames_passed = 0;
			current_frame ++;
		}

		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[current_frame];
	}

	SDL_Rect& GetCurrentFrameBox(float dt)
	{
		return GetCurrentFrame(dt).frame;
	}

	SDL_Rect& GetRect()
	{
		return this->frames[0].frame;
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void ResetAnimation()
	{
		frames_passed = 0;
		last_frame = 0;
		current_frame = 0;
	}
};

#endif