#include "UIFactory.h"

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

UI* UIFactory::CreateReviveButton(float x, float y, UI* parent)
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
	return nullptr;
}

UI* UIFactory::CreateSFXScrollbar(float x, float y, UI* parent)
{
	return nullptr;
}

