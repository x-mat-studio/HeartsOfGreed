#include "App.h"
#include "UIFactory.h"
#include "UI_Group.h"
#include "Window.h"

UIFactory::UIFactory() :
	portraitBackground{ 643, 145, 72, 56 },
	portraitLevelBackground{ 55, 210, 18, 18 },
	portraitHealthbarBackground{ 34, 22, 52, 10 },
	portraitHealthbarGreenImage{ 29, 79, 50, 8 },
	dataPageBackground{ 400, 435, 139, 67 },
	dataPageImageBackground{ 727, 203, 65, 51 },
	gathererPicture{ 351, 149, 68, 52 },
	gathererShopPicture{ 581, 24, 36, 27 },
	meleePicture{ 561, 149, 68, 52 },
	meleeShopPicture{ 658, 24, 36, 27 },
	rangedPicture{ 147, 149, 68, 52 },
	rangedShopPicture{ 619, 24, 36, 27 },
	baseDataPagePicture{ 634, 78, 68, 62 },
	turretDataPagePicture{ 561, 77, 68, 62 },
	wanamingoDataPagePicture{ 894, 116, 70, 79 },
	turretShopPicture{ 696, 12, 34, 40 },
	resourceIcon{ 18, 209, 11, 19 },
	creditsBackgroundImage{ 563, 237, 117, 122 },
	pauseMenuBackground{ 15, 271, 194, 231 },
	optionsMenuBackground{ 677, 369, 278, 153 },
	creditsMenuBackground{ 15, 271, 194, 231 },
	minimapBackground{ 221, 317, 162, 174 },
	shopBackground{ 15, 271, 194, 231 },
	resourcesBackground{ 415, 435, 65, 30 },

	menuButton{ 17, 12, 195, 36 },
	pauseButton{ 449, 24, 24, 24 },
	closeButton{ 424, 25, 23, 23 },
	fullscreenOnButton{ 739, 27, 23, 23 },
	fullscreenOffButton{ 763, 27, 23, 23 },
	shopButton{ 480, 62, 33, 33 },
	reviveButton{ 653, 54, 46, 14 }
{}

// Menu specific functions

UI_Group* UIFactory::CreateMainMenu()
{
	float x (app->win->width / app->win->GetUIScale() - menuButton.w - 20);
	float y (app->win->height / (app->win->GetUIScale() * 4));

	UI_Group* group = new UI_Group(GROUP_TAG::MAIN_MENU);

	UI* uiReturnPointer;

	//uiReturnPointer = CreateContinueGameButton(x, y, nullptr);
	//group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateNewGameButton(x, y + 40, nullptr);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateOptionsButton(x, y + 80, nullptr);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateCreditsButton(x, y + 120, nullptr);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateExitGameButton(x, y + 160, nullptr);
	group->AddUiElement(uiReturnPointer);

	//CreateText(x + 5, y + 5, nullptr, "C O N T I N U E    G A M E");
	
	CreateText(x + 35, y + 45, nullptr, "N E W  G A M E");
	
	CreateText(x + 40, y + 85, nullptr, "O P T I O N S");
	
	CreateText(x + 42, y + 125, nullptr, "C R E D I T S");
	
	CreateText(x + 30, y + 165, nullptr, "E X I T    G A M E");

	return group;
}

UI_Group* UIFactory::CreateOptionsMenu()
{
	float x((app->win->width / app->win->GetUIScale() / 2) - (optionsMenuBackground.w / 2));
	float y((app->win->height / app->win->GetUIScale() / 2) - (optionsMenuBackground.h / 2));

	UI_Group* group = new UI_Group(GROUP_TAG::MAIN_MENU);

	UI* uiReturnPointer;
	UI* background;
	
	background = CreateImage(x, y, nullptr, optionsMenuBackground);
	group->AddUiElement(background);

	uiReturnPointer = CreateFullscreenButton(x + 20, y + 100, background);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateCloseMenuButton(x + optionsMenuBackground.w - (3 * closeButton.w / 4), y - (1 * closeButton.h / 4), background);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateMusicScrollbar(x + 20, y + 60, background);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateSFXScrollbar(x + 140, y + 60, background);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 30, y, background, "Options");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 20, y + 25, background, "Music");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 140, y + 25, background, "SFX");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 20, y + 75, background, "Fullscreen mode");
	group->AddUiElement(uiReturnPointer);

	return group;
}

UI_Group* UIFactory::CreateCreditsMenu()
{
	float x((app->win->width / app->win->GetUIScale() / 2) - (pauseMenuBackground.w / 2));
	float y((app->win->height / app->win->GetUIScale() / 2) - (pauseMenuBackground.h / 2));
	
	UI_Group* group = new UI_Group(GROUP_TAG::MAIN_MENU);

	UI* uiReturnPointer;
	UI* background;

	background = CreateImage(x, y, nullptr, pauseMenuBackground);
	group->AddUiElement(background);

	uiReturnPointer = CreateCloseMenuButton(x + pauseMenuBackground.w - (3 * closeButton.w / 4), y - (1 * closeButton.h / 4), background);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateImage(x + 40, y + 50, background, creditsBackgroundImage);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 5, y, background, "Aaron Guerrero Cruz");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 15, y + 10, background, "Lead");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 5, y + 35, background, "Jose Luis Redondo Tello");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 15, y + 45, background, "Code");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 5, y + 70, background, "Ferran-Roger Basart i Bosch");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 15, y + 80, background, "Management + UI");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 5, y + 105, background, "Alex Melenchon Maza");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 15, y + 115, background, "Design");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 5, y + 140, background, "Adria Serrano Lopez");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 15, y + 150, background, "Audio + Art");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 5, y + 175, background, "Oscar Perez Martin");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateText(x + 15, y + 185, background, "QA");
	group->AddUiElement(uiReturnPointer);

	return group;
}

UI_Group* UIFactory::CreateBasicInGameUI()
{
	float x(app->win->width / app->win->GetUIScale());
	float y(app->win->height / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::MAIN_MENU);

	UI* uiReturnPointer;
	UI* background;

	char resources[10];
	//float screenResources = app->player->GetResources();

	//rect = RectConstructor(556, 35, 15, 14);
	//father = AddButton(fMPoint(w / app->win->GetUIScale() - 87, 35), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"PortraitHideButton", EVENT_ENUM::NULL_EVENT, false, false, true, false);
	//AddUIElement(fMPoint(w / app->win->GetUIScale() - 72, 35), nullptr, UI_TYPE::UI_PORTRAIT, rect, P2SString("portraitVector"), nullptr, DRAGGABLE::DRAG_OFF);
	//AddButton(fMPoint(162, h / app->win->GetUIScale() - 85), nullptr, UI_TYPE::UI_BUTTON, rect, P2SString("minimapHideButton"), EVENT_ENUM::NULL_EVENT, false, false, true, false);

	uiReturnPointer = CreateImage(0, y - minimapBackground.h - 20, nullptr, minimapBackground);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreatePauseGameButton(x - (1.25f) * pauseButton.w, ((1.25f) * pauseButton.w) - pauseButton.w, nullptr);
	group->AddUiElement(uiReturnPointer);

	background = CreateImage(x - 65, y - 97, nullptr, resourcesBackground);
	group->AddUiElement(background);

	uiReturnPointer = CreateImage(x - 59, y - 90, background, resourceIcon);
	group->AddUiElement(uiReturnPointer);

//	screenResources = app->player->GetResources();
//	sprintf_s(resources, 10, "%d", screenResources);
//	uiReturnPointer = CreateText(x - 41, y - 94, background, resources);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateImage(x - dataPageBackground.w, y - dataPageBackground.h, nullptr, dataPageBackground);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateImage(x - 2 * dataPageImageBackground.w + 12, y - dataPageImageBackground.h - 5, nullptr, dataPageImageBackground);
	group->AddUiElement(uiReturnPointer);

	return group;
}

UI_Group* UIFactory::CreateDataPageComponents()
{
	UI_Group* group = new UI_Group(GROUP_TAG::MAIN_MENU);

	UI* uiReturnPointer;
	UI* background;

	return group;
}

UI_Group* UIFactory::CreatePauseMenu()
{
	float x((app->win->width / app->win->GetUIScale() / 2) - (pauseMenuBackground.w / 2));
	float y((app->win->height / app->win->GetUIScale() / 2) - (pauseMenuBackground.h / 2));
	
	UI_Group* group = new UI_Group(GROUP_TAG::MAIN_MENU);

	UI* uiReturnPointer;
	UI* background;

	background = CreateImage(x, y, nullptr, pauseMenuBackground);
	group->AddUiElement(background);

	x = ((app->win->width / app->win->GetUIScale() / 2) - (menuButton.w / 2));

	uiReturnPointer = CreateResumeGameButton(x, y + 8, background);
	group->AddUiElement(background);

	//uiReturnPointer = CreateSaveGameButton(x, y + 52, background);
	//group->AddUiElement(background);

	//uiReturnPointer = CreateSaveGameButton(x, y + 97, background);
	//group->AddUiElement(background);

	uiReturnPointer = CreateOptionsButton(x, y + 142, background);
	group->AddUiElement(background);

	uiReturnPointer = CreateReturnToMainMenuButton(x, y + 187, background);
	group->AddUiElement(background);

	CreateText(x + 32, y + 13, nullptr, "R E S U M E  G A M E");

	CreateText(x + 43, y + 57, nullptr, "S A V E  G A M E");

	CreateText(x + 43, y + 102, nullptr, "L O A D  G A M E");

	CreateText(x + 58, y + 147, nullptr, "O P T I O N S");

	CreateText(x + 48, y + 192, nullptr, "M A I N  M E N U");

	return group;
}

UI_Group* UIFactory::CreateShopMenu()
{
	float x((app->win->width / app->win->GetUIScale() / 2) - (shopBackground.w / 2));
	float y((app->win->height / app->win->GetUIScale() / 2) - (shopBackground.h / 2));

	UI_Group* group = new UI_Group(GROUP_TAG::MAIN_MENU);

	UI* uiReturnPointer;
	UI* background;

	char cost[40];

	background = CreateImage(x, y, nullptr, shopBackground);
	group->AddUiElement(background);

	// Heroes
	uiReturnPointer = CreateText(x + 3, y + 5, background, "H E R O   R E S U R R E C T I O N");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateImage(x + 30, y + 35, background, gathererShopPicture);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateImage(x + 80, y + 35, background, rangedShopPicture);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateImage(x + 130, y + 35, background, meleeShopPicture);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateGathererReviveButton(x + 25, y + 65, background);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateRangedReviveButton(x + 75, y + 65, background);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateMeleeReviveButton(x + 125, y + 65, background);
	group->AddUiElement(uiReturnPointer);

	CreateText(x + 28, y + 57, nullptr, "Revive");

	CreateText(x + 78, y + 57, nullptr, "Revive");

	CreateText(x + 128, y + 57, nullptr, "Revive");

	// Turrets
	uiReturnPointer = CreateText(x + 10, y + 85, background, "T U R R E T   B U I L D I N G");
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateImage(x + 120, y + 135, background, turretShopPicture);
	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateBuyTurretButton(x + 40, y + 120, background);
	group->AddUiElement(uiReturnPointer);

	CreateText(x + 50, y + 112, nullptr, "Buy");

//	if (lastShop != nullptr)
//	{
//		sprintf_s(cost, 40, "Max. %i", lastShop->GetmaxTurrets());
//		uiReturnPointer = CreateText(x + 95, y + 112, background, cost);
//		group->AddUiElement(uiReturnPointer);
//	}
	
//	sprintf_s(cost, 40, "- %i", app->player->GetTurretCost());
//	uiReturnPointer = CreateText(x + 45, y + 130, background, cost);
//	group->AddUiElement(uiReturnPointer);

	uiReturnPointer = CreateUpgradeTurretButton(x + 40, y + 170, background);
	group->AddUiElement(uiReturnPointer);

	CreateText(x + 45, y + 162, nullptr, "Lvl up");

	uiReturnPointer = CreateCloseMenuButton(x - (closeButton.w / 2), y - (closeButton.h / 2), background);
	group->AddUiElement(uiReturnPointer);

	return group;
}

// Element specific functions

UI* UIFactory::CreateImage(float x, float y, UI* parent, SDL_Rect rect)
{
	return nullptr;
}

UI* UIFactory::CreateText(float x, float y, UI* parent, P2SString text)
{
	return nullptr;
}

UI* UIFactory::CreateNewGameButton(float x, float y, UI* parent)
{
	return nullptr;
}

UI* UIFactory::CreateOptionsButton(float x, float y, UI* parent)
{
	return nullptr;
}

UI* UIFactory::CreateFullscreenButton(float x, float y, UI* parent)
{
	// Check screen condition to know if you should create the button on ON or OFF

	return nullptr;
}

UI* UIFactory::CreateCreditsButton(float x, float y, UI* parent)
{
	return nullptr;
}

UI* UIFactory::CreateExitGameButton(float x, float y, UI* parent)
{
	return nullptr;
}


UI* UIFactory::CreatePauseGameButton(float x, float y, UI* parent)
{
	return nullptr;
}

UI* UIFactory::CreateResumeGameButton(float x, float y, UI* parent)
{
	return nullptr;
}

UI* UIFactory::CreateReturnToMainMenuButton(float x, float y, UI* parent)
{
	// The event triggered is UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU
	return nullptr;
}

UI* UIFactory::CreateCloseMenuButton(float x, float y, UI* parent)
{
	return nullptr;
}


UI* UIFactory::CreateShopButton(float x, float y, UI* parent)
{
	return nullptr;
}

UI* UIFactory::CreateGathererReviveButton(float x, float y, UI* parent)
{
	return nullptr;
}

UI* UIFactory::CreateMeleeReviveButton(float x, float y, UI* parent)
{
	return nullptr;
}

UI* UIFactory::CreateRangedReviveButton(float x, float y, UI* parent)
{
	return nullptr;
}

UI* UIFactory::CreateBuyTurretButton(float x, float y, UI* parent)
{
	return nullptr;
}

UI* UIFactory::CreateUpgradeTurretButton(float x, float y, UI* parent)
{
	return nullptr;
}

UI* UIFactory::CreateMusicScrollbar(float x, float y, UI* parent)
{
	// Triggers event music adjustment, at 128.0f

	return nullptr;
}

UI* UIFactory::CreateSFXScrollbar(float x, float y, UI* parent)
{
	// Triggers event sfx adjustment, at 455.0f

	return nullptr;
}

