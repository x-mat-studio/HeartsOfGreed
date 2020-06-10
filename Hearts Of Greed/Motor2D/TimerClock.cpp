#include "TimerClock.h"
#include "Textures.h"
#include "Fonts.h"
#include "TestScene.h"
#include "EventManager.h"

TimerClock::TimerClock(float x, float y, UI* parent) :

	UI({ x, y }, parent, UI_TYPE::TIMER_CLOCK, { 0, 0, 0, 0 }, false, false, nullptr),
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

		if (app->testScene->IsNight() == false)
		{
			if (min == 0 && sec <= 30)
			{
				if ((sec % 2) == 1)
				{
					SetNumbersColor(255, 128, 128, 255);
				}
			}
		}
	}

	if (min == 0 && sec == 30)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::START_DAY_NIGHT_TRANSITION, EVENT_ENUM::NULL_EVENT);
	}




}

void TimerClock::SetNumbersColor(uint r, uint g, uint b, uint a)
{
	uint newR = MIN(r, 255);
	newR = MAX(r, 0);
	uint newG = MIN(g, 255);
	newG = MAX(g, 0);
	uint newB = MIN(b, 255);
	newB = MAX(b, 0);
	uint newA = MIN(a, 255);
	newA = MAX(a, 0);

	SDL_SetTextureColorMod(texture, newR, newG, newB);
	SDL_SetTextureAlphaMod(texture, newA);
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