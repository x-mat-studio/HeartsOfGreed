#ifndef __FADE_TO_BLACK_H__
#define __FADE_TO_BLACK_H__

#define CURTAIN_DELAY 0.4 //Should not lower this value, ask Jose

#include "Module.h"
#include "EasingFunctions.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Texture;

enum class FADE_ANIMATION : int
{
	NONE,

	FADE,
	CURTAIN
};

class ModuleFadeToBlack : public Module
{
public:
	ModuleFadeToBlack();
	~ModuleFadeToBlack();

	bool Start();
	bool PostUpdate(float dt);
	bool CleanUp();

	bool FadeToBlack(Module* moduleOff, Module* moduleOn, float time = 2.0f, FADE_ANIMATION anim = FADE_ANIMATION::FADE);

private:
	void ExecuteEvent(EVENT_ENUM eventId);

	void DrawAnim(float normalized, float dt);

private:

	enum class FADE_STEP
	{
		NONE,

		FADE_TO_BLACK,
		FADE_FROM_BLACK
	};

	FADE_STEP currentStep;
	FADE_ANIMATION currentAnim;

	float timeSpent;
	float totalTime;
	bool animStarted;

	SDL_Rect screen;
	Module* toEnable;
	Module* toDisable;

	Easing rightCurtainEasing;
	Easing leftCurtainEasing;

	SDL_Texture* texture;
	SDL_Rect rightRect;
	SDL_Rect leftRect;

	int vaultDoorSound;

};

#endif //!__FADE_TO_BLACK_H__