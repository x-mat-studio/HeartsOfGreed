#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "UI.h"

enum class BUTTON_TAG : int
{
	NULL_TAG = -1,

	CLOSE_OPTIONS_MENU,
	CLOSE_CREDITS_MENU,

	NEW_GAME,
	CONTINUE_GAME,
	OPTIONS,
	CREDITS,
	EXIT_GAME,

	FULLSCREEN_ON,
	FULLSCREEN_OFF,

	HIDE,
	SHOW,

	PAUSE,
	RESUME,
	MAIN_MENU,
	SAVE,
	LOAD,

	SHOP,
	REVIVE_GATHERER,
	REVIVE_RANGED,
	REVIVE_MELEE,
	BUY_TURRET,
	LEVEL_UP_TURRET,

	 GATHERER_PORTRAIT,
	 MELEE_PORTRAIT,
	 RANGED_PORTRAIT

};

class Button : public UI
{
public:
	Button(fMPoint positionValue, UI* father, SDL_Rect rect, bool dragable, SDL_Texture* texture, BUTTON_TAG buttonTag);
	~Button();

	BUTTON_TAG GetTag();
	void SetTag(BUTTON_TAG tag);

private:
	void HandleInput();
	void Draw(float dt);


private:
	bool clicked;
	bool focusedFirstFrame;
	BUTTON_TAG buttonTag;
};

#endif //__BUTTON_H__