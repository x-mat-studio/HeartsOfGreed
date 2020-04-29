#ifndef __UIFACTORY_H__
#define __UIFACTORY_H__

#include "p2SString.h"
#include "SDL/include/SDL_rect.h"

class UI;
class UI_Group;

struct UIFactory
{
public:

	UIFactory();

	// Menu creation functions

	UI_Group* CreateMainMenu();
	UI_Group* CreateOptionsMenu();
	UI_Group* CreateCreditsMenu();

	UI_Group* CreateBasicInGameUI();
	UI_Group* CreateDataPageComponents();
	UI_Group* CreatePauseMenu();
	UI_Group* CreateShopMenu();

	// Image creation functions

	UI* CreateImage(float x, float y, UI* parent, SDL_Rect rect, UI_Group* group, bool dragable = false);

	// Text creation functions

	UI* CreateText(float x, float y, UI* parent, char* text, UI_Group* group, bool interactable = false);

	// Button creation functions

	UI* CreateNewGameButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateOptionsButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateFullscreenButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateCreditsButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateExitGameButton(float x, float y, UI* parent, UI_Group* group);

	UI* CreatePauseGameButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateResumeGameButton(float x, float y, UI* parent, UI_Group* group);
//	UI* CreateSaveGameButton(float x, float y, UI* parent, UI_Group* group);
//	UI* CreateLoadGameButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateReturnToMainMenuButton(float x, float y, UI* parent, UI_Group* group);

//	UI* CreateHideButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateCloseMenuButton(float x, float y, UI* parent, UI_Group* group);

	UI* CreateShopButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateGathererReviveButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateMeleeReviveButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateRangedReviveButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateBuyTurretButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateUpgradeTurretButton(float x, float y, UI* parent, UI_Group* group);

	// Scrollbar creation functions

	UI* CreateMusicScrollbar(float x, float y, UI* parent, UI_Group* group);
	UI* CreateSFXScrollbar(float x, float y, UI* parent, UI_Group* group);

private:
	// Image SDL_Rect

	SDL_Rect portraitBackground;
	SDL_Rect portraitLevelBackground;
	SDL_Rect portraitHealthbarBackground;
	SDL_Rect portraitHealthbarGreenImage;

//	SDL_Rect dataPageHealthbarGreenImage;
//	SDL_Rect dataPageHealthbarBlueImage;
	SDL_Rect dataPageBackground;
	SDL_Rect dataPageImageBackground;

	SDL_Rect gathererPicture;
	SDL_Rect gathererShopPicture;
	SDL_Rect meleePicture;
	SDL_Rect meleeShopPicture;
	SDL_Rect rangedPicture;
	SDL_Rect rangedShopPicture;
	SDL_Rect baseDataPagePicture;
	SDL_Rect turretDataPagePicture;
	SDL_Rect wanamingoDataPagePicture;
	SDL_Rect turretShopPicture;

	SDL_Rect resourceIcon;
	SDL_Rect creditsBackgroundImage;

	SDL_Rect pauseMenuBackground;
	SDL_Rect optionsMenuBackground;
	SDL_Rect creditsMenuBackground;
	SDL_Rect minimapBackground;
	SDL_Rect shopBackground;
	SDL_Rect resourcesBackground;

	// Button SDL_Rect

	SDL_Rect menuButton;
	SDL_Rect pauseButton;
	SDL_Rect closeButton;
//	SDL_Rect hideLeftButton;
//	SDL_Rect hideRightButton;
	SDL_Rect fullscreenOnButton;
	SDL_Rect fullscreenOffButton;
	SDL_Rect shopButton;
	SDL_Rect reviveButton;

	// Scrollbar SDL_Rect

//	SDL_Rect musicScrollbar;
//	SDL_Rect SFXScrollbar;

};

#endif __UIFACTORY_H__