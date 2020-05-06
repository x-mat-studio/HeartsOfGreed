#include "App.h"
#include "UIFactory.h"
#include "UI_Group.h"

#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Scrollbar.h"
#include "Button.h"
#include "ResourcesPortrait.h"
#include "DataPages.h"
#include "HeroesPortraitManager.h"
#include "HeroPortrait.h"

#include "Base.h"
#include "Turret.h"
#include "Enemy.h"
#include "Hero.h"

#include "Window.h"
#include "Input.h"
#include "Minimap.h"
#include "Player.h"
#include "Hero.h"

UIFactory::UIFactory() :
	portraitBackground{ 643, 145, 72, 56 },
	portraitLevelBackground{ 55, 210, 18, 18 },
	portraitHealthbarBackground{ 34, 22, 52, 10 },
	portraitHealthbarGreenImage{ 29, 79, 50, 8 },
	dataPageBackground{ 678, 369, 190, 67 },
	dataPageImageBackground{ 721, 202, 65, 51 },
	gathererPicture{ 352, 149, 59, 45 },
	gathererShopPicture{ 581, 24, 36, 27 },
	meleePicture{ 566, 149, 59, 45 },
	meleeShopPicture{ 658, 24, 36, 27 },
	rangedPicture{ 150, 149, 59, 45 },
	rangedShopPicture{ 619, 24, 36, 27 },
	baseDataPagePicture{ 634, 90, 59, 45 },
	turretDataPagePicture{ 561, 77, 60, 45 },
	wanamingoDataPagePicture{ 885, 152, 59, 45 },
	turretShopPicture{ 696, 12, 34, 40 },
	resourceIcon{ 18, 209, 11, 19 },
	creditsBackgroundImage{ 563, 237, 117, 122 },
	pauseMenuBackground{ 15, 271, 194, 231 },
	optionsMenuBackground{ 677, 369, 278, 153 },
	creditsMenuBackground{ 15, 271, 194, 231 },
	minimapBackground{ 221, 317, 162, 150 },
	shopBackground{ 15, 271, 194, 231 },
	resourcesBackground{ 415, 435, 65, 30 },

	menuButton{ 17, 12, 195, 36 },
	pauseButton{ 449, 24, 24, 24 },
	closeButton{ 424, 25, 23, 23 },
	fullscreenOnButton{ 739, 27, 23, 23 },
	fullscreenOffButton{ 763, 27, 23, 23 },
	shopButton{ 480, 62, 33, 33 },
	reviveButton{ 653, 54, 46, 14 },

	scrollbarBar{ 272, 45, 90, 4 },
	scrollbarButton{ 257, 15, 13, 34 },

	dataPageHealthbarGreenImage{ 313, 81, 59, 4 },
	dataPageHealthbarBlueImage{ 375, 81, 59, 4 },
	healthBarContainer{ 251, 86, 61, 8 },

	heroPortrait{ 478, 328, 33, 40 },
	gathererHeroIcon{ 178, 529, 13, 16 },
	meleHeroIcon{ 134, 528, 20, 17 },
	rangedHeroIcon{ 157, 526, 18, 19 },

	littleHealthBarContainer{ 251, 72, 31, 7 },
	littleHealthbarGreenImage{ 319, 73, 29, 4 },
	littleHealthbarBlueImage{ 352, 73, 29, 4 },

	reviveHoverBackground{ 20, 300, 150, 50 }
{}


UIFactory::~UIFactory()
{
}

// Menu specific functions

UI_Group* UIFactory::CreateMainMenu()
{
	float x (app->win->width / app->win->GetUIScale() - menuButton.w - 20);
	float y (app->win->height / (app->win->GetUIScale() * 4));

	UI_Group* group = new UI_Group(GROUP_TAG::MAIN_MENU);

	//CreateContinueGameButton(x, y, nullptr, group);

	CreateNewGameButton(x, y + 40, nullptr, group);

	CreateOptionsButton(x, y + 80, nullptr, group);

	CreateCreditsButton(x, y + 120, nullptr, group);

	CreateExitGameButton(x, y + 160, nullptr, group);

	//CreateText(x + 5, y + 5, nullptr, "C O N T I N U E    G A M E", group);
	
	return group;
}


UI_Group* UIFactory::CreateOptionsMenu()
{
	float x((app->win->width * 0.5 / app->win->GetUIScale()) - (optionsMenuBackground.w * 0.5));
	float y((app->win->height * 0.5 / app->win->GetUIScale()) - (optionsMenuBackground.h * 0.5));

	UI_Group* group = new UI_Group(GROUP_TAG::OPTIONS_MENU);

	UI* background;
	UI* scrollbar;
	
	background = CreateImage(x, y, nullptr, optionsMenuBackground, group, true);

	scrollbar = CreateImage(20, 60, background, scrollbarBar, group, false);
	CreateMusicScrollbar(0, 0, scrollbar, group);

	scrollbar = CreateImage(140, 60, background, scrollbarBar, group, false);
	CreateSFXScrollbar(0, 0, scrollbar, group);

	CreateFullscreenButton(20, 100, background, group);

	CreateCloseOptionMenuButton(optionsMenuBackground.w - (0.75f * closeButton.w), (0.25f * closeButton.h), background, group);

	CreateText(30, 0, background, "Options", group);

	CreateText(20, 25, background, "Music", group);

	CreateText(140, 25, background, "SFX", group);

	CreateText(20, 75, background, "Fullscreen mode", group);

	return group;
}


UI_Group* UIFactory::CreateCreditsMenu()
{
	float x((app->win->width * 0.5 / app->win->GetUIScale()) - (pauseMenuBackground.w * 0.5));
	float y((app->win->height * 0.5 / app->win->GetUIScale()) - (pauseMenuBackground.h * 0.5));
	
	UI_Group* group = new UI_Group(GROUP_TAG::CREDITS_MENU);

	UI* background;

	background = CreateImage(x, y, nullptr, pauseMenuBackground, group, true);

	CreateCloseCreditsButton(pauseMenuBackground.w - (3 * closeButton.w / 4), (1 * closeButton.h / 4), background, group);

	CreateImage(40, 50, background, creditsBackgroundImage, group);

	CreateText(5, 0, background, "Aaron Guerrero Cruz", group);

	CreateText(15, 10, background, "Lead", group);

	CreateText(5, 35, background, "Jose Luis Redondo Tello", group);

	CreateText(15, 45, background, "Code", group);

	CreateText(5, 70, background, "Ferran-Roger Basart i Bosch", group);

	CreateText(15, 80, background, "Management + UI", group);
	
	CreateText(5, 105, background, "Alex Melenchon Maza", group);

	CreateText(15, 115, background, "Design", group);

	CreateText(5, 140, background, "Adria Serrano Lopez", group);

	CreateText(15, 150, background, "Audio + Art", group);

	CreateText(5, 175, background, "Oscar Perez Martin", group);

	CreateText(15, 185, background, "QA", group);

	return group;
}


UI_Group* UIFactory::CreateBasicInGameUI()
{
	float x(app->win->width / app->win->GetUIScale());
	float y(app->win->height / app->win->GetUIScale());

	UI* background;
	UI_Group* group = new UI_Group(GROUP_TAG::IN_GAME);

	//rect = RectConstructor(556, 35, 15, 14);
	//father = AddButton(fMPoint(w / app->win->GetUIScale() - 87, 35), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"PortraitHideButton", EVENT_ENUM::NULL_EVENT, false, false, true, false);
	//AddUIElement(fMPoint(w / app->win->GetUIScale() - 72, 35), nullptr, UI_TYPE::UI_PORTRAIT, rect, P2SString("portraitVector"), nullptr, DRAGGABLE::DRAG_OFF);
	//AddButton(fMPoint(162, h / app->win->GetUIScale() - 85), nullptr, UI_TYPE::UI_BUTTON, rect, P2SString("minimapHideButton"), EVENT_ENUM::NULL_EVENT, false, false, true, false);

	//CreateImage(0, app->minimap->position.y, nullptr, minimapBackground, group);
	CreatePortraitManager(x, y, nullptr, group);

	CreatePauseGameButton(x - (1.25f) * pauseButton.w, ((1.25f) * pauseButton.w) - pauseButton.w, nullptr, group);

	CreateResourcesPortrait(x - 65, y - 97, nullptr, group);

	background = CreateImage(x - dataPageBackground.w, y - dataPageBackground.h, nullptr, dataPageBackground, group);

	CreateImage(x - dataPageBackground.w, y - dataPageBackground.h, nullptr, dataPageImageBackground, group);

	CreateDataPage(background, nullptr, group);

	return group;
}


UI_Group* UIFactory::CreateDataPageComponents()
{
	UI_Group* group = new UI_Group(GROUP_TAG::DATA_PAGE);

	UI* background;

	return group;
}


UI_Group* UIFactory::CreatePauseMenu()
{
	float x((app->win->width * 0.5 / app->win->GetUIScale()) - (pauseMenuBackground.w * 0.5));
	float y((app->win->height * 0.5 / app->win->GetUIScale()) - (pauseMenuBackground.h * 0.5));
	
	UI_Group* group = new UI_Group(GROUP_TAG::PAUSE_MENU);
	
	UI* background;

	background = CreateImage(x, y, nullptr, pauseMenuBackground, group, true);

	CreateResumeGameButton(0, 8, background, group);

	CreateSaveGameButton(0, 52, background, group);

	CreateLoadGameButton(0, 97, background, group);

	CreateOptionsButton(0, 142, background, group);

	CreateReturnToMainMenuButton(0, 187, background, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverReviveMenu(Button* button)
{
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	switch (button->GetTag())
	{
	case BUTTON_TAG::REVIVE_GATHERER:
		CreateText(5, 0, background, "Revive gatherer hero:", group);
		break;
	case BUTTON_TAG::REVIVE_MELEE:
		CreateText(5, 0, background, "Revive melee hero:", group);
		break;
	case BUTTON_TAG::REVIVE_RANGED:
		CreateText(5, 0, background, "Revive ranged hero:", group);
		break;
	default:
		break;
	}

	CreateImage(5, 20, background, resourceIcon, group, false, false);

	CreateText(25, 15, background, "-500", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverBuyTurretMenu(Button* button)
{
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	CreateText(5, 0, background, "Buy turret:", group);

	CreateImage(5, 20, background, resourceIcon, group, false, false);

	CreateText(25, 15, background, "-120", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverUpgradeTurretMenu(Button* button)
{
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	CreateText(5, 0, background, "Upgrade turret:", group);

	CreateImage(5, 20, background, resourceIcon, group, false, false);

	CreateText(25, 15, background, "-200", group);

	return group;
}


// Element specific functions

UI* UIFactory::CreateImage(float x, float y, UI* parent, SDL_Rect rect, UI_Group* group, bool dragable, bool interactable)
{
	UI_Image* uiImage = new UI_Image(x, y, parent, rect, app->uiManager->GetAtlasTexture(), dragable, interactable);

	group->AddUiElement(uiImage);

	return uiImage;
}


UI* UIFactory::CreateNonGroupImage(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, SDL_Rect rect, bool dragable, bool interactable)
{
	UI_Image* uiImage = new UI_Image(x, y, parent, rect, app->uiManager->GetAtlasTexture(), dragable, interactable);

	dataPagesVector->push_back(uiImage);

	return uiImage;
}


UI* UIFactory::CreateText(float x, float y, UI* parent, char* text, UI_Group* group, bool interactable)
{
	UI_Text* uiText = new UI_Text(x, y, parent, text, interactable);

	group->AddUiElement(uiText);

	return uiText;
}

UI* UIFactory::CreateNonGroupText(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, char* text, bool interactable)
{
	UI_Text* uiText = new UI_Text(x, y, parent, text, interactable);

	dataPagesVector->push_back(uiText);

	return uiText;
}

UI* UIFactory::CreateNewGameButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, menuButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::NEW_GAME);

	group->AddUiElement(button);

	CreateText(55, 5, button, "N E W  G A M E", group);

	return button;
}


UI* UIFactory::CreateOptionsButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, menuButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::OPTIONS);

	group->AddUiElement(button);

	CreateText(58, 5, button, "O P T I O N S", group);

	return button;
}


UI* UIFactory::CreateFullscreenButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button;
		
	if (app->win->stateResolution == RESOLUTION_MODE::FULLSCREEN)
	{
		button = new Button(fMPoint{ x, y }, parent, fullscreenOffButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::FULLSCREEN_OFF);
	}
	else
	{
		button = new Button(fMPoint{ x, y }, parent, fullscreenOnButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::FULLSCREEN_ON);
	}

	group->AddUiElement(button);

	return nullptr;
}


UI* UIFactory::CreateCreditsButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, menuButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::CREDITS);

	group->AddUiElement(button);

	CreateText(60, 5, button, "C R E D I T S", group);

	return button;
}


UI* UIFactory::CreateExitGameButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, menuButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::EXIT_GAME);

	group->AddUiElement(button);

	CreateText(50, 5, button, "E X I T    G A M E", group);

	return button;
}


UI* UIFactory::CreatePauseGameButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, pauseButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::PAUSE);

	group->AddUiElement(button);

	return nullptr;
}


UI* UIFactory::CreateResumeGameButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, menuButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::RESUME);

	group->AddUiElement(button);

	CreateText(37, 5, button, "R E S U M E  G A M E", group);

	return button;
}


UI* UIFactory::CreateSaveGameButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, menuButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::SAVE);

	group->AddUiElement(button);

	CreateText(47, 5, button, "S A V E  G A M E", group);
	
	return button;
}


UI* UIFactory::CreateLoadGameButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, menuButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::LOAD);

	group->AddUiElement(button);

	CreateText(48, 5, button, "L O A D  G A M E", group);

	return button;
}


UI* UIFactory::CreateReturnToMainMenuButton(float x, float y, UI* parent, UI_Group* group)
{
	// The event triggered is UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU
	Button* button = new Button(fMPoint{ x, y }, parent, menuButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::MAIN_MENU);

	group->AddUiElement(button);

	CreateText(48, 5, button, "M A I N  M E N U", group);

	return button;
}



UI* UIFactory::CreateCloseOptionMenuButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, closeButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::CLOSE_OPTIONS_MENU);

	group->AddUiElement(button);

	return button;
}


UI* UIFactory::CreateCloseCreditsButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, closeButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::CLOSE_CREDITS_MENU);

	group->AddUiElement(button);

	return button;
}


UI* UIFactory::CreateClosePauseMenuButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, closeButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::RESUME);

	group->AddUiElement(button);

	return button;
}


UI* UIFactory::CreateGathererReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, reviveButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::REVIVE_GATHERER);
	dataPagesVector->push_back(button);

	return button;
}



UI* UIFactory::CreateMeleeReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, reviveButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::REVIVE_MELEE);
	dataPagesVector->push_back(button);

	return button;
}

UI* UIFactory::CreateRangedReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, reviveButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::REVIVE_RANGED);
	dataPagesVector->push_back(button);

	return button;
}

UI* UIFactory::CreateBuyTurretButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, reviveButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::BUY_TURRET);
	dataPagesVector->push_back(button);

	return button;
}

UI* UIFactory::CreateUpgradeTurretButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, reviveButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::UPGRADE_TURRET);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateMusicScrollbar(float x, float y, UI* parent, UI_Group* group)
{
	UI_Scrollbar* scrollbar = new UI_Scrollbar( x, y, parent, scrollbarButton, app->uiManager->GetAtlasTexture(), 128.0f);

	group->AddUiElement(scrollbar);

	return scrollbar;
}


UI* UIFactory::CreateSFXScrollbar(float x, float y, UI* parent, UI_Group* group)
{
	UI_Scrollbar* scrollbar = new UI_Scrollbar(x, y, parent, scrollbarButton, app->uiManager->GetAtlasTexture(), 455.0f);

	group->AddUiElement(scrollbar);

	return scrollbar;
}


UI* UIFactory::CreateResourcesPortrait(float x, float y, UI* parent, UI_Group* group)
{
	UI* background = CreateImage(x, y, nullptr, resourcesBackground, group);

	CreateImage(6, 7, background, resourceIcon, group);

	ResourcesPortrait* resourcesPortrait = new ResourcesPortrait(24, 3, background, false);

	group->AddUiElement(resourcesPortrait);

	return resourcesPortrait;
}

UI* UIFactory::CreateDataPage(UI* parent, Entity* entity, UI_Group* group)
{
	DataPages* dataPage = new DataPages(0, 0, parent, entity);

	group->AddUiElement(dataPage);

	return dataPage;
}


void UIFactory::CreateGathererPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Hero* focus = (Hero*)app->player->GetFocusedEntity();

	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, gathererPicture);

	CreateNonGroupImage(68, 4, dataPage, dataPagesVector, healthBarContainer);
	
	CreateNonGroupImage(69, 6, dataPage, dataPagesVector, dataPageHealthbarGreenImage);
	
	CreateNonGroupImage(68, 12, dataPage, dataPagesVector, healthBarContainer);
	
	CreateNonGroupImage(69, 14, dataPage, dataPagesVector, dataPageHealthbarBlueImage);

	//stats
	sprintf_s(stats, 40, "AD: %i", focus->GetAttackDamage());
	CreateNonGroupText(133, 0, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Exp: %i/%i", focus->GetHeroXP(), focus->GetExpToLevelUp());
	CreateNonGroupText(68, 12, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AS: %.2f", focus->GetAttackSpeed());
	CreateNonGroupText(133, 12, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rng: %i", focus->GetAttackRange());
	CreateNonGroupText(68, 25, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rec: %i", focus->GetRecoveryHitPointsRate());
	CreateNonGroupText(133, 25, dataPage, dataPagesVector, stats);
}


void UIFactory::CreateMeleePage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Hero* focus = (Hero*)app->player->GetFocusedEntity();

	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, meleePicture);

	CreateNonGroupImage(68, 4, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 6, dataPage, dataPagesVector, dataPageHealthbarGreenImage);

	CreateNonGroupImage(68, 12, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 14, dataPage, dataPagesVector, dataPageHealthbarBlueImage);

	//stats
	sprintf_s(stats, 40, "AD: %i", focus->GetAttackDamage());
	CreateNonGroupText(133, 0, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Exp: %i/%i", focus->GetHeroXP(), focus->GetExpToLevelUp());
	CreateNonGroupText(68, 12, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AS: %.2f", focus->GetAttackSpeed());
	CreateNonGroupText(133, 12, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rng: %i", focus->GetAttackRange());
	CreateNonGroupText(68, 25, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rec: %i", focus->GetRecoveryHitPointsRate());
	CreateNonGroupText(133, 25, dataPage, dataPagesVector, stats);
}


void UIFactory::CreateRangedPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Hero* focus = (Hero*)app->player->GetFocusedEntity();

	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, rangedPicture);

	CreateNonGroupImage(68, 4, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 6, dataPage, dataPagesVector, dataPageHealthbarGreenImage);

	CreateNonGroupImage(68, 12, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 14, dataPage, dataPagesVector, dataPageHealthbarBlueImage);

	//stats
	sprintf_s(stats, 40, "AD: %i", focus->GetAttackDamage());
	CreateNonGroupText(133, 0, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Exp: %i/%i", focus->GetHeroXP(), focus->GetExpToLevelUp());
	CreateNonGroupText(68, 12, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AS: %.2f", focus->GetAttackSpeed());
	CreateNonGroupText(133, 12, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rng: %i", focus->GetAttackRange());
	CreateNonGroupText(68, 25, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rec: %i", focus->GetRecoveryHitPointsRate());
	CreateNonGroupText(133, 25, dataPage, dataPagesVector, stats);
}


void UIFactory::CreateWanamingoPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Enemy* focus = (Enemy*)app->player->GetFocusedEntity();

	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, wanamingoDataPagePicture);

	CreateNonGroupImage(68, 8, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 10, dataPage, dataPagesVector, dataPageHealthbarGreenImage);

	//stats
	sprintf_s(stats, 40, "AD: %i", focus->GetAD());
	CreateNonGroupText(133, 0, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rng: %i", focus->GetVision());
	CreateNonGroupText(68, 12, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AS: %i", focus->GetAS());
	CreateNonGroupText(133, 12, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rec: %i", focus->GetRecov());
	CreateNonGroupText(68, 25, dataPage, dataPagesVector, stats);
}


void UIFactory::CreateBasePage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Base* focus = (Base*)app->player->GetFocusedEntity();

	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, baseDataPagePicture);

	CreateNonGroupImage(68, 8, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 10, dataPage, dataPagesVector, dataPageHealthbarGreenImage);

	CreateGathererReviveButton(140, 10, dataPage, dataPagesVector);

	CreateMeleeReviveButton(140, 20, dataPage, dataPagesVector);

	CreateRangedReviveButton(140, 30, dataPage, dataPagesVector);

	CreateBuyTurretButton(68, 30, dataPage, dataPagesVector);

	CreateUpgradeTurretButton(108, 30, dataPage, dataPagesVector);

	//stats
	sprintf_s(stats, 40, "Resources: %i", focus->GetRsrc());
	CreateNonGroupText(68, 10, dataPage, dataPagesVector, stats);

}


void UIFactory::CreateNonPlayerBasePage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, baseDataPagePicture);

	CreateNonGroupImage(68, 8, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 10, dataPage, dataPagesVector, dataPageHealthbarGreenImage);
}


void UIFactory::CreateTurretPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Turret* focus = (Turret*)app->player->GetFocusedEntity();

	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, turretDataPagePicture);

	CreateNonGroupImage(68, 8, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 10, dataPage, dataPagesVector, dataPageHealthbarGreenImage);

	//stats
	sprintf_s(stats, 40, "LVL: %i", focus->GetLvl());
	CreateNonGroupText(-45, -15, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AD: %i", focus->GetAD());
	CreateNonGroupText(-45, -30, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rng: %i", focus->GetRng());
	CreateNonGroupText(-45, -45, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AS: %i", focus->GetAS());
	CreateNonGroupText(-45, -60, dataPage, dataPagesVector, stats);
}


void UIFactory::CreateUpgradeCenterPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{


}


void UIFactory::CreateBarricadePage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{


}




 
UI* UIFactory::CreatePortraitManager(float x, float y, UI* parent, UI_Group* group)
{
	UI* element = new HeroesPortraitManager(x, y, nullptr, false);

	group->AddUiElement(element);

	app->uiManager->SetPortraitManager((HeroesPortraitManager*)element);

	return element;
}


HeroPortrait* UIFactory::CreatePortrait(Hero* hero)
{
	HeroPortrait* portrait = new HeroPortrait(hero);

	Button* BackGround = nullptr;
	UI* icon = nullptr;
	UI* healthBar = nullptr;
	UI* manaBar = nullptr;
	UI* healthBarCont = nullptr;
	UI* manaBarCont = nullptr;
	
	switch (hero->GetType())
	{
	case ENTITY_TYPE::HERO_GATHERER:
		BackGround = new Button(fMPoint{ app->win->width / app->win->GetUIScale() - heroPortrait.w - 5, 0 }, portrait, heroPortrait, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::GATHERER_PORTRAIT);
		portrait->AddElement(BackGround);

		icon = new UI_Image(10, 5, BackGround, gathererHeroIcon, app->uiManager->GetAtlasTexture(), false, false);
		portrait->AddElement(icon);

		break;


	case ENTITY_TYPE::HERO_MELEE:
		BackGround = new Button(fMPoint{ app->win->width / app->win->GetUIScale() - heroPortrait.w - 5, 0 }, portrait, heroPortrait, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::MELEE_PORTRAIT);
		portrait->AddElement(BackGround);

		icon = new UI_Image(6, 5, BackGround, meleHeroIcon, app->uiManager->GetAtlasTexture(), false, false);
		portrait->AddElement(icon);

		break;


	case ENTITY_TYPE::HERO_RANGED:
		BackGround = new Button(fMPoint{ app->win->width / app->win->GetUIScale() - heroPortrait.w - 5, 0 }, portrait, heroPortrait, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::RANGED_PORTRAIT);
		portrait->AddElement(BackGround);

		icon = new UI_Image(7, 3, BackGround, rangedHeroIcon, app->uiManager->GetAtlasTexture(), false, false);
		portrait->AddElement(icon);

		break;


	default:
		assert(true); //shouldt have anything that is not a hero
		break;
	}

	
	healthBar = new UI_Image(1, 25, BackGround, littleHealthbarGreenImage, app->uiManager->GetAtlasTexture(), false, false);
	portrait->AddHealthBar(healthBar);

	healthBarCont = new UI_Image(0, 24, BackGround, littleHealthBarContainer, app->uiManager->GetAtlasTexture(), false, false);
	portrait->AddElement(healthBarCont);

	manaBar = new UI_Image(1, 33, BackGround, littleHealthbarBlueImage, app->uiManager->GetAtlasTexture(), false, false);
	portrait->AddManaBar(manaBar);

	manaBarCont = new UI_Image(0, 32, BackGround, littleHealthBarContainer, app->uiManager->GetAtlasTexture(), false, false);
	portrait->AddElement(manaBarCont);

	return portrait;
}

SDL_Rect UIFactory::GetHealthBarBackground()
{
	return healthBarContainer;
}


SDL_Rect UIFactory::GetGreenHealthBar()
{
	return dataPageHealthbarGreenImage;
}


SDL_Rect UIFactory::GetBlueHealthBar()
{
	return dataPageHealthbarBlueImage;
}

