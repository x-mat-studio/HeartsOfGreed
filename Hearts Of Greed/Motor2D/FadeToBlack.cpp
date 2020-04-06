#include <math.h>
#include "App.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

ModuleFadeToBlack::ModuleFadeToBlack():
	totalTime(0.0f),
	timeSpent(0.0f),
	toEnable(nullptr),
	toDisable(nullptr),
	screen({0,0,0,0}),
	currentStep(FADE_STEP::NONE)
{
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{
	toEnable = nullptr;
	toDisable = nullptr;
}

// Load assets
bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	int w;
	int h;
	app->render->GetCameraMeasures(w, h);
	screen = { 0, 0, w, h };
	return true;
}

// Update: draw background
bool ModuleFadeToBlack::PostUpdate(float dt)
{
	if(currentStep == FADE_STEP::NONE)
		return true;

	timeSpent += dt;
	float normalized = MIN(1.0f, timeSpent / totalTime);

	switch(currentStep)
	{
		case FADE_STEP::FADE_TO_BLACK:
		{
			if(timeSpent >= totalTime)
			{
				toDisable->Disable();
				toEnable->Enable();
				timeSpent = 0.0f;
				currentStep = FADE_STEP::FADE_FROM_BLACK;
			}
		} break;

		case FADE_STEP::FADE_FROM_BLACK:
		{
			normalized = 1.0f - normalized;
			if (timeSpent >= totalTime)
			{
				currentStep = FADE_STEP::NONE;
				timeSpent = 0.0f;
				totalTime = 0.0f;
			}
		} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool ModuleFadeToBlack::FadeToBlack(Module* module_off, Module* module_on, float time)
{
	bool ret = false;

	if(currentStep == FADE_STEP::NONE)
	{
		currentStep = FADE_STEP::FADE_TO_BLACK;
		timeSpent = 0.0f;
		totalTime = time;
		toEnable = module_on;
		toDisable = module_off;
		ret = true;
	}

	return ret;
}