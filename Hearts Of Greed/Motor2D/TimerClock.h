#ifndef __TIMER_CLOCK_H__
#define __TIMER_CLOCK_H__

#include "UI.h"

class TimerClock : public UI
{
public:
	TimerClock(float x, float y, UI* parent);
	~TimerClock();

	void HandleInput();

private:
	void ChangeTexture();

private:
	int minutes;
	int seconds;

};


#endif//__TIMER_CLOCK_H__