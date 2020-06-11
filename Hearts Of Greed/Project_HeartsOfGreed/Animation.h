#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Frame.h"
#include "SDL/include/SDL_rect.h"
#include "App.h"

#define MAX_FRAMES 55

class Animation
{
public:
	bool loop = true;
	Frame frames[MAX_FRAMES];
	int lastFrame = 0;

private:
	float framesPassed = 0;
	int currentFrame = 0;
	int loops = 0;

public:

	Animation PushAnimation(pugi::xml_node node, P2SString name) {
		
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

					aux.PushBack(rect, iteratorFrames.attribute("time").as_float(),  iteratorFrames.attribute("pivotX").as_int(), iteratorFrames.attribute("pivotY").as_int());
					
					iteratorFrames = iteratorFrames.next_sibling();
				}


				break;
			}


		}


		aux.currentFrame = 0;
		return aux;
	}

	
	void PushBack(const SDL_Rect& rect, const float maxFrames,int pivotPositionX, int pivotPositionY) {
		
		frames[lastFrame].frame = rect;
		frames[lastFrame].maxFrames = maxFrames;
		frames[lastFrame].pivotPositionX = pivotPositionX;
		frames[lastFrame].pivotPositionY = pivotPositionY;


		lastFrame++;
	}

	bool GetAnimationFrame(int searchFrame) 
	{
		if (searchFrame == currentFrame)
			return true;

		return false;
	}

	Frame& GetCurrentFrame(float dt)
	{
		if (dt != 0)
		{
			if (framesPassed * dt < frames[currentFrame].maxFrames * dt)
				framesPassed += dt;
			else
			{
				framesPassed = 0;
				currentFrame++;
			}


			if (currentFrame >= lastFrame)
			{
				currentFrame = (loop) ? 0 : lastFrame - 1;
				loops++;
			}
		}

		return frames[currentFrame];
	}

	Frame& GetCurrentFrame()
	{
		return frames[currentFrame];
	}

	int GetCurrentFrameNum()
	{
		return currentFrame;
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
		framesPassed = 0;
		//lastFrame = 0;
		currentFrame = 0;
	}


};

#endif