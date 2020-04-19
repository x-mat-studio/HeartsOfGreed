#ifndef __FADE_TO_BLACK_H__
#define __FADE_TO_BLACK_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"

class ModuleFadeToBlack : public Module
{
public:
	ModuleFadeToBlack();
	~ModuleFadeToBlack();

	bool Start();
	bool PostUpdate(float dt);
	bool CleanUp();

	bool FadeToBlack(Module* moduleOff, Module* moduleOn, float time = 2.0f);

private:
	void ExecuteEvent(EVENT_ENUM eventId);

private:

	enum class FADE_STEP
	{
		NONE,
		FADE_TO_BLACK,
		FADE_FROM_BLACK
	} currentStep = FADE_STEP::NONE;

	float timeSpent;
	float totalTime;
	SDL_Rect screen;
	Module* toEnable;
	Module* toDisable;
};

#endif //!__FADE_TO_BLACK_H__