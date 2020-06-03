#include "TimerClock.h"
#include "Textures.h"
#include "Fonts.h"
#include "TestScene.h"
#include "EventManager.h"

TimerClock::TimerClock(float x, float y, UI* parent) :

	UI({ x, y }, parent, UI_TYPE::TIMER_CLOCK, {0, 0, 0, 0}, false, false, nullptr),
	minutes(0),
	seconds(0)
{
	app->testScene->GetTimer(minutes, seconds);
	ChangeTexture();
}


TimerClock::~TimerClock()
{
	app->tex->UnLoad(texture);
	minutes = 0;
	seconds = 0;
}


void TimerClock::HandleInput()
{
	int min = 0;
	int sec = 0;
	app->testScene->GetTimer(min, sec);
	
	if (minutes != min || seconds != sec)
	{
		minutes = min;
		seconds = sec;
		ChangeTexture();
	}

	if (min == 0 && sec == 30)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::START_DAY_NIGHT_TRANSITION, EVENT_ENUM::NULL_EVENT);
	}

	
}


void TimerClock::ChangeTexture()
{
	app->tex->UnLoad(texture);

	char string[8];

	if (seconds < 10)
		sprintf_s(string, 8, "%i : 0%i", minutes, seconds);

	else
		sprintf_s(string, 8, "%i : %i", minutes, seconds);

	texture = app->fonts->Print(string, { 255, 255, 255 }, app->fonts->fonts[0], 230);
}