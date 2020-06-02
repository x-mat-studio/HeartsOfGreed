#include <math.h>
#include "App.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "EventManager.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

ModuleFadeToBlack::ModuleFadeToBlack() :

	totalTime(0.0f),
	timeSpent(0.0f),
	screen({ 0,0,0,0 }),

	toEnable(nullptr),
	toDisable(nullptr),

	currentStep(FADE_STEP::NONE),
	currentAnim(FADE_ANIMATION::NONE)
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
	int w;
	int h;

	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	app->render->GetCameraMeasures(w, h);
	screen = { 0, 0, w, h };

	app->eventManager->EventRegister(EVENT_ENUM::GAME_WIN, this);
	app->eventManager->EventRegister(EVENT_ENUM::GAME_LOSE, this);

	return true;
}


bool ModuleFadeToBlack::CleanUp()
{
	app->eventManager->EventUnRegister(EVENT_ENUM::GAME_WIN, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GAME_LOSE, this);

	return true;
}


// Update: draw background
bool ModuleFadeToBlack::PostUpdate(float dt)
{
	CheckListener(this);

	if (currentStep == FADE_STEP::NONE)
		return true;

	timeSpent += app->necessaryDt;

	float normalized = MIN(1.0f, timeSpent / totalTime);

	switch (currentStep)
	{
	case FADE_STEP::FADE_TO_BLACK:
	{
		if (timeSpent >= totalTime)
		{
			toDisable->Disable();
			toEnable->Enable();
			timeSpent = 0.0f;
			currentStep = FADE_STEP::FADE_FROM_BLACK;

			if (currentAnim == FADE_ANIMATION::CURTAIN)
			{
				leftCurtainEasing.NewEasing(EASING_TYPE::EASE, 0, 0 - screen.w * 0.5, totalTime);
				rightCurtainEasing.NewEasing(EASING_TYPE::EASE, screen.w * 0.5, screen.w, totalTime);
			}

			app->gamePause = false;
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
	DrawAnim(normalized, app->necessaryDt);

	return true;
}


// Fade to black. At mid point deactivate one module, then activate the other
bool ModuleFadeToBlack::FadeToBlack(Module* module_off, Module* module_on, float time, FADE_ANIMATION anim)
{
	bool ret = false;

	if (currentStep == FADE_STEP::NONE)
	{
		currentStep = FADE_STEP::FADE_TO_BLACK;
		currentAnim = anim;

		if (currentAnim == FADE_ANIMATION::CURTAIN)
		{
			leftCurtainEasing.NewEasing(EASING_TYPE::EASE, 0 - screen.w * 0.5, 0, time);
			rightCurtainEasing.NewEasing(EASING_TYPE::EASE, screen.w, screen.w * 0.5, time);
		}

		timeSpent = 0.0f;
		totalTime = time;
		toEnable = module_on;
		toDisable = module_off;
		ret = true;
	}

	return ret;
}


void ModuleFadeToBlack::ExecuteEvent(EVENT_ENUM eventId)
{
	switch (eventId)
	{
	case EVENT_ENUM::GAME_WIN:
		app->fadeToBlack->FadeToBlack((Module*)app->testScene, (Module*)app->winScene);
		break;

	case EVENT_ENUM::GAME_LOSE:
		app->fadeToBlack->FadeToBlack((Module*)app->testScene, (Module*)app->loseScene);
		break;

	default:
		break;
	}

}


void ModuleFadeToBlack::DrawAnim(float normalized, float dt)
{
	SDL_Rect rightCurtain = { 0, 0, screen.w * 0.5, screen.h };
	SDL_Rect leftCurtain = { 0, 0, screen.w * 0.5, screen.h };

	switch (currentAnim)
	{
	case FADE_ANIMATION::NONE:
		break;

	case FADE_ANIMATION::FADE:
		SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
		SDL_RenderFillRect(app->render->renderer, &screen);
		break;

	case FADE_ANIMATION::CURTAIN:
		

		leftCurtain.x = leftCurtainEasing.UpdateEasingAddingTime(dt);
		rightCurtain.x = rightCurtainEasing.UpdateEasingAddingTime(dt);

		SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(255.0f));
		SDL_RenderFillRect(app->render->renderer, &leftCurtain);
		SDL_RenderFillRect(app->render->renderer, &rightCurtain);

		break;

	default:
		break;
	}
}