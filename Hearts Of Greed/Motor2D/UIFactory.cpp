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
#include "DialogText.h"
#include "TimerClock.h"

#include "Base.h"
#include "Turret.h"
#include "Barricade.h"
#include "Enemy.h"
#include "Hero.h"

#include "Window.h"
#include "Input.h"
#include "Minimap.h"
#include "EntityManager.h"
#include "Player.h"
#include "Hero.h"

UIFactory::UIFactory() :
	portraitBackground{ 643, 145, 72, 56 },
	portraitRoundBackground{ 400, 328, 45, 45 },
	portraitLevelBackground{ 55, 210, 18, 18 },
	portraitHealthbarBackground{ 34, 22, 52, 10 },
	portraitHealthbarGreenImage{ 29, 79, 50, 8 },
	dataPageBackground{ 678, 369, 190, 67 },
	dataPageImageBackground{ 721, 202, 65, 51 },
	gathererPicture{ 352, 149, 61, 47 },
	gathererBigPicture{ 224, 105, 123, 96 },
	gathererShopPicture{ 581, 24, 36, 27 },
	meleePicture{ 566, 149, 61, 47 },
	meleeShopPicture{ 658, 24, 36, 27 },
	meleeBigPicture{ 425, 105, 123, 96 },
	rangedPicture{ 150, 149, 61, 47 },
	rangedShopPicture{ 619, 24, 36, 27 },
	rangedBigPicture{ 17, 105, 123, 96 },
	robottoPicture{ 807, 515, 61, 47 },
	robottoShopPicture{ 875, 512, 36, 27 },
	robottoBigPicture{ 677, 512, 123, 96 },
	hqCommanderPortrait{ 951, 540, 61, 47 },
	baseDataPagePicture{ 634, 90, 59, 45 },
	upgradeCenterDataPagePicture{ 711, 306, 61, 47 },
	turretDataPagePicture{ 580, 400, 61, 47 },
	barricadeDataPagePicture{ 713, 256, 61, 47 },
	wanamingoDataPagePicture{ 885, 150, 59, 45 },
	speedomingoDataPagePicture{ 949, 159, 23, 31 },
	snipermingoDataPagePicture{ 987, 157, 31, 36 },
	gigamingoDataPagePicture{ 1029, 145, 38, 50 },
	turretShopPicture{ 695, 14, 25, 37 },
	barricadeShopPicture{ 705, 53, 24, 37 },
	resourceIcon{ 18, 209, 11, 19 },
	resourceIconSkill{ 111, 209, 20, 20 },
	resourceIconBoost{ 83, 209, 17, 19 },
	creditsBackgroundImage{ 563, 237, 117, 122 },
	pauseMenuBackground{ 15, 271, 194, 231 },
	optionsMenuBackground{ 677, 369, 278, 133 },
	creditsMenuBackground{ 15, 271, 194, 231 },
	minimapBackground{ 221, 317, 162, 150 },
	shopBackground{ 15, 271, 194, 231 },
	resourcesBackground{ 415, 435, 95, 60 },

	menuButton{ 17, 12, 195, 36 },
	pauseButton{ 449, 24, 24, 24 },
	closeButton{ 424, 25, 23, 23 },
	fullscreenOnButton{ 739, 27, 23, 23 },
	fullscreenOffButton{ 763, 27, 23, 23 },
	okButton{ 763, 53, 23, 23 },
	shopButton{ 480, 62, 33, 33 },
	upgradeCenterPurchaseButton{ 980, 706, 88, 87 },
	reviveGathererButton{ 783, 706, 88, 87 },
	reviveMeleeButton{ 883, 706, 88, 87 },
	reviveRangedButton{ 785, 804, 88, 87 },
	reviveRobottoButton{ 884, 804, 88, 87 },

	upgradeArrowIcon{ 740, 57, 14, 11 },
	plusIcon{ 740, 71, 14, 14 },

	scrollbarBar{ 272, 45, 90, 4 },
	scrollbarButton{ 257, 15, 13, 34 },

	healthbarGreenImage{ 313, 81, 59, 4 },
	healthbarBlueImage{ 375, 81, 59, 4 },
	healthBarContainer{ 251, 86, 61, 8 },

	deadHeroIcon{ 478, 328, 33, 40 },
	gathererHeroIcon{ 100, 521, 33, 39 },
	meleHeroIcon{ 17, 518, 43, 42 },
	rangedHeroIcon{ 59, 518, 40, 41 },
	robottoHeroIcon{ 136, 520, 40, 40 },
	gathererHeroPortrait{ 842, 628, 45, 45 },
	meleHeroPortrait{ 720, 626, 45, 45 },
	rangedHeroPortrait{ 657, 626, 45, 45 },
	robottoHeroPortrait{ 783, 628, 45, 45 },


	reviveHoverBackground{ 222, 508, 150, 50 },
	upgradeHoverBackground{ 222, 508, 180, 70 },
	lifeUpgradeButton{ 430, 581, 88, 87 },
	damageUpgradeButton{ 34, 581, 88, 87 },
	energyUpgradeButton{ 532, 581, 88, 87 },
	attackSpeedUpgradeButton{ 328, 581, 88, 87 },

	miniFrame{ 509, 706, 238, 125 },

	ugradeSkillButton{ 763, 79, 15, 11 },
	gathererPassive1Button{ 140, 827, 93, 91 },
	meleePassive1Button{ 245, 826, 93, 91 },
	rangedPassive1Button{ 36, 828, 93, 91 },
	robottoPassive1Button{ 353, 828, 93, 91 },
	gathererActive1Button{ 36, 708, 93, 91 },
	meleeActive1Button{ 244, 708, 93, 91 },
	rangedActive1Button{ 140, 707, 93, 91 },
	robottoActive1Button{ 348, 707, 93, 91 },

	dialogWindow{ 1096, 41, 859, 615 },
	skipButton{ 54 ,21, 35, 18 },

	dayNightIcon{ 522, 851, 35, 35 },

	popUpWindow{ 507, 844, 211, 77 },

	gathererLifeUpgradeCost(100),
	gathererDamageUpgradeCost(100),
	gathererEnergyUpgradeCost(100),
	gathererAtkSpeedUpgradeCost(100),

	meleeLifeUpgradeCost(100),
	meleeDamageUpgradeCost(100),
	meleeEnergyUpgradeCost(100),
	meleeAtkSpeedUpgradeCost(100),

	rangedLifeUpgradeCost(100),
	rangedDamageUpgradeCost(100),
	rangedEnergyUpgradeCost(100),
	rangedAtkSpeedUpgradeCost(100),

	robottoLifeUpgradeCost(100),
	robottoDamageUpgradeCost(100),
	robottoEnergyUpgradeCost(100),
	robottoAtkSpeedUpgradeCost(100),

	reviveCost(500),
	turretUpgradeCost(200),
	barricadeUpgradeCost(150)

{}


UIFactory::~UIFactory()
{
}

// Menu specific functions

UI_Group* UIFactory::CreateMainMenu()
{
	float x(app->win->width / app->win->GetUIScale() - menuButton.w - 20);
	float y(app->win->height / (app->win->GetUIScale() * 4));

	UI_Group* group = new UI_Group(GROUP_TAG::MAIN_MENU);

	CreateNewGameButton(x, y, nullptr, group);

	CreateContinueGameButton(x, y + 40, nullptr, group);

	CreateOptionsButton(x, y + 80, nullptr, group);

	CreateCreditsButton(x, y + 120, nullptr, group);

	CreateExitGameButton(x, y + 160, nullptr, group);

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

	CreateText(15, 45, background, "Code + UI", group);

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


UI_Group* UIFactory::CreateDialogMenu(ENTITY_TYPE character1, ENTITY_TYPE character2)
{
	UI_Group* group = new UI_Group(GROUP_TAG::DIALOG);

	Button* button = new Button(fMPoint(60, 0), nullptr, dialogWindow, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::NEXT_DIALOG);
	group->AddUiElement(button);

	DialogText* text = new DialogText(0, 0, fMPoint(160, 41), fMPoint(85, 130), button, false);
	group->AddUiElement(text);

	Button* skip = new Button(fMPoint(420, 20), button, skipButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::SKIP_DIALOG);
	group->AddUiElement(skip);

	CreateText(3, -5, skip, "Skip", group);

	switch (character1)
	{
	case ENTITY_TYPE::HERO_MELEE:
		CreateImage(76, 49, button, meleePicture, group, false, false);
		break;
	case ENTITY_TYPE::HERO_RANGED:
		CreateImage(76, 49, button, rangedPicture, group, false, false);
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		CreateImage(76, 49, button, gathererPicture, group, false, false);
		break;
	case ENTITY_TYPE::HERO_ROBO:
		CreateImage(76, 49, button, robottoHeroPortrait, group, false, false);
		break;
	case ENTITY_TYPE::HQ_COMANDER:
		CreateImage(76, 49, button, hqCommanderPortrait, group, false, false);
		break;

	default:
		assert("wrong type");
		break;
	}

	switch (character2)
	{
	case ENTITY_TYPE::HERO_MELEE:
		CreateImage(338, 153, button, meleePicture, group, false, false);
		break;
	case ENTITY_TYPE::HERO_RANGED:
		CreateImage(338, 153, button, rangedPicture, group, false, false);
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		CreateImage(338, 153, button, gathererPicture, group, false, false);
		break;
	case ENTITY_TYPE::HERO_ROBO:
		CreateImage(338, 153, button, robottoHeroPortrait, group, false, false);
		break;
	case ENTITY_TYPE::HQ_COMANDER:
		CreateImage(338, 153, button, hqCommanderPortrait, group, false, false);
		break;

	default:
		assert("wrong type");
		break;
	}


	return group;
}


UI_Group* UIFactory::CreatePopUp(P2SString& string)
{
	UI_Group* group = new UI_Group(GROUP_TAG::POP_UP);

	UI* image = CreateImage(200, 10, nullptr, popUpWindow, group, false, false);

	Button* button = new Button(fMPoint(200, 0), image, closeButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::CLOSE_POP_UP);
	group->AddUiElement(button);

	CreateText(5, 5, image, string.GetCharArray(), group);

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
	CreatePortraitManager(640, 0, nullptr, group);

	//CreateImage(-12, app->minimap->position.y - 273, nullptr, miniFrame, group);
	UI* img = CreateImage(10, 5, nullptr, { 40, 291, 45, 20 }, group);
	CreateImage(40, 5, nullptr, { 40, 291, 45, 20 }, group);

	TimerClock* clock = new TimerClock(5, -5, img);
	group->AddUiElement(clock);

	CreateImage(55, -1, nullptr, dayNightIcon, group);


	CreatePauseGameButton(x - (1.25f) * pauseButton.w, ((1.25f) * pauseButton.w) - pauseButton.w, nullptr, group);

	CreateResourcesPortrait(x - 95, y - 127, nullptr, group);

	background = CreateImage(x - dataPageBackground.w, y - dataPageBackground.h, nullptr, dataPageBackground, group);

	CreateImage(x - dataPageBackground.w, y - dataPageBackground.h, nullptr, dataPageImageBackground, group);

	CreateDataPage(background, nullptr, group);

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
	char stats[40];

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
	case BUTTON_TAG::REVIVE_ROBOTTO:
		CreateText(5, 0, background, "Revive robot hero:", group);
		break;
	default:
		break;
	}

	CreateImage(5, 25, background, resourceIcon, group, false, false);

	sprintf_s(stats, 40, "- %i", reviveCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverBuyTurretMenu()
{
	char stats[40];
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	CreateText(5, 0, background, "Buy turret:", group);

	CreateImage(5, 25, background, resourceIcon, group, false, false);

	sprintf_s(stats, 40, "- %i", app->player->GetTurretCost());
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverUpgradeTurretMenu()
{
	char stats[40];
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	CreateText(5, 0, background, "Upgrade turret:", group);

	CreateImage(5, 25, background, resourceIcon, group, false, false);

	sprintf_s(stats, 40, "- %i", turretUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverBuyUpgradeCenterMenu()
{
	char stats[40];
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	CreateText(5, 0, background, "Buy upgrade center:", group);

	CreateImage(5, 25, background, resourceIcon, group, false, false);

	sprintf_s(stats, 40, "- %i", app->player->GetUpgradeCenterCost());
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverBuyBarricadeMenu()
{
	char stats[40];
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	CreateText(5, 0, background, "Buy barricade:", group);

	CreateImage(5, 25, background, resourceIcon, group, false, false);

	sprintf_s(stats, 40, "- %i", app->player->GetBarricadeCost());
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverUpgradeBarricadeMenu()
{
	char stats[40];
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	CreateText(5, 0, background, "Upgrade barricade:", group);

	CreateImage(5, 25, background, resourceIcon, group, false, false);

	sprintf_s(stats, 40, "- %i", barricadeUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverGathererLifeUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade HP: +%.2f", app->entityManager->gathererLifeUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", gathererLifeUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverGathererDamageUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade damage: +%.2f", app->entityManager->gathererDamageUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", gathererDamageUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverGathererEnergyUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade energy: +%.2f", app->entityManager->gathererEnergyUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", gathererEnergyUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverGathererAttackSpeedUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade atk. speed: +%.2f", app->entityManager->gathererAtkSpeedUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", gathererAtkSpeedUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverMeleeLifeUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade HP: +%.2f", app->entityManager->meleeLifeUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", meleeLifeUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverMeleeDamageUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade damage: +%.2f", app->entityManager->meleeDamageUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", meleeDamageUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverMeleeEnergyUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade energy: +%.2f", app->entityManager->meleeEnergyUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", meleeEnergyUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverMeleeAttackSpeedUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade atk. speed: +%.2f", app->entityManager->meleeAtkSpeedUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", meleeAtkSpeedUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRangedLifeMenuMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade HP: +%.2f", app->entityManager->rangedLifeUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", rangedLifeUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRangedDamageUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade damage: +%.2f", app->entityManager->rangedDamageUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", rangedDamageUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRangedEnergyUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade energy: +%.2f", app->entityManager->rangedEnergyUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", rangedEnergyUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRangedAttackSpeedUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade atk. speed: +%.2f", app->entityManager->rangedAtkSpeedUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", rangedAtkSpeedUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRobottoLifeMenuMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade HP: +%.2f", app->entityManager->robottoLifeUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", robottoLifeUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRobottoDamageUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade damage: +%.2f", app->entityManager->robottoDamageUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", robottoDamageUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRobottoEnergyUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - reviveHoverBackground.w, pos.y - reviveHoverBackground.h, nullptr, reviveHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade energy: +%.2f", app->entityManager->robottoEnergyUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", robottoEnergyUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRobottoAttackSpeedUpgradeMenu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	sprintf_s(stats, 40, "Upgrade atk. speed: +%.2f", app->entityManager->robottoAtkSpeedUpgradeValue);
	strcat_s(stats, " %");
	CreateText(5, 0, background, stats, group);

	CreateImage(5, 25, background, resourceIconBoost, group, false, false);

	sprintf_s(stats, 40, "- %.0f", robottoAtkSpeedUpgradeCost);
	CreateText(25, 20, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverGathererPassive1Menu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	Hero* hero = (Hero*)app->player->focusedEntity;
	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Handy mastery:", group);

	sprintf_s(stats, 40, "Grants +%i gems by killing", hero->GetPassiveSkill().dmg);

	CreateText(5, 15, background, stats, group);

	CreateText(5, 25, background, "enemies, boosts robotic", group);

	sprintf_s(stats, 40, "allies' damage by %.0f.", hero->GetPassiveSkill().executionTime);

	CreateText(5, 35, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverGathererActive1Menu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	Hero* hero = (Hero*)app->player->focusedEntity;
	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Detonation:", group);

	sprintf_s(stats, 40, "Causes +%i dmg on", hero->GetSkill1().dmg);

	CreateText(5, 15, background, stats, group);

	CreateText(5, 25, background, "the chosen area.", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverMeleePassive1Menu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	Hero* hero = (Hero*)app->player->focusedEntity;
	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Born to fight:", group);

	sprintf_s(stats, 40, "Grants +%i healing outside", hero->GetPassiveSkill().coolDown);

	CreateText(5, 15, background, stats, group);

	sprintf_s(stats, 40, "battle and -%i damage taken", hero->GetPassiveSkill().dmg);

	CreateText(5, 25, background, stats, group);

	CreateText(5, 35, background, "per enemy on combat.", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverMeleeActive1Menu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	Hero* hero = (Hero*)app->player->focusedEntity;
	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Hammer slam:", group);

	sprintf_s(stats, 40, "Causes +%i dmg around him.", hero->GetSkill1().dmg);

	CreateText(5, 15, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRangedPassive1Menu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	Hero* hero = (Hero*)app->player->focusedEntity;
	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Bloodhsed:", group);

	sprintf_s(stats, 40, "Gives health / %i points of", hero->GetPassiveSkill().dmg);

	CreateText(5, 15, background, stats, group);

	CreateText(5, 25, background, "bleeding to enemies that", group);

	CreateText(5, 35, background, "have been attacked by her.", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRangedActive1Menu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	Hero* hero = (Hero*)app->player->focusedEntity;
	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Slowing grenade:", group);

	sprintf_s(stats, 40, "Slows enemies by %0.1f per", hero->GetSkill1().effectSeverity);

	CreateText(5, 15, background, stats, group);

	CreateText(5, 25, background, "cent of their movement", group);

	sprintf_s(stats, 40, "during %0.1f seconds.", hero->GetSkill1().effectTime);

	CreateText(5, 35, background, stats, group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRobottoPassive1Menu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	Hero* hero = (Hero*)app->player->focusedEntity;
	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Kill streak:", group);

	sprintf_s(stats, 40, "Gains a boost of +%i damage", hero->GetPassiveSkill().dmg);

	CreateText(5, 15, background, stats, group);

	sprintf_s(stats, 40, "and +%i speed by killing", hero->GetPassiveSkill().rangeRadius);

	CreateText(5, 25, background, stats, group);

	CreateText(5, 35, background, "enemies (max 5 boosts).", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRobottoActive1Menu()
{
	char stats[40];

	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	Hero* hero = (Hero*)app->player->focusedEntity;
	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Immolation:", group);

	sprintf_s(stats, 40, "Causes +%i dmg by", hero->GetSkill1().dmg);

	CreateText(5, 15, background, stats, group);

	CreateText(5, 25, background, "destroying himself.", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverGathererPassive1UpgradeMenu()
{
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Upgrade 'handy mastery':", group);

	CreateImage(5, 25, background, resourceIconSkill, group, false, false);

	CreateText(25, 20, background, "   -1", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverGathererActive1UpgradeMenu()
{
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Upgrade 'detonation':", group);

	CreateImage(5, 25, background, resourceIconSkill, group, false, false);

	CreateText(25, 20, background, "   -1", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverMeleePassive1UpgradeMenu()
{
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Upgrade 'born to fight':", group);

	CreateImage(5, 25, background, resourceIconSkill, group, false, false);

	CreateText(25, 20, background, "   -1", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverMeleeActive1UpgradeMenu()
{
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Upgrade 'hammer slam':", group);

	CreateImage(5, 25, background, resourceIconSkill, group, false, false);

	CreateText(25, 20, background, "   -1", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRangedPassive1UpgradeMenu()
{
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Upgrade 'bloodhsed':", group);

	CreateImage(5, 25, background, resourceIconSkill, group, false, false);

	CreateText(25, 20, background, "   -1", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRangedActive1UpgradeMenu()
{
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Upgrade :", group);

	CreateImage(5, 25, background, resourceIconSkill, group, false, false);

	CreateText(25, 20, background, "   -1", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRobottoPassive1UpgradeMenu()
{
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Upgrade 'kill strike':", group);

	CreateImage(5, 25, background, resourceIconSkill, group, false, false);

	CreateText(25, 20, background, "   -1", group);

	return group;
}


UI_Group* UIFactory::CreateOnHoverRobottoActive1UpgradeMenu()
{
	iMPoint pos(app->input->GetMousePosScreen() / app->win->GetUIScale());

	UI_Group* group = new UI_Group(GROUP_TAG::IN_HOVER_MENU);

	UI* background = CreateImage(pos.x - upgradeHoverBackground.w, pos.y - upgradeHoverBackground.h, nullptr, upgradeHoverBackground, group, false, false);

	CreateText(5, 0, background, "Upgrade 'immolation':", group);

	CreateImage(5, 25, background, resourceIconSkill, group, false, false);

	CreateText(25, 20, background, "   -1", group);

	return group;
}


UI_Group* UIFactory::CreateSaveConfirmationMenu()
{
	float x((app->win->width * 0.5 / app->win->GetUIScale()) - (upgradeHoverBackground.w * 0.5));
	float y((app->win->height * 0.5 / app->win->GetUIScale()) - (upgradeHoverBackground.h * 0.5));

	UI_Group* group = new UI_Group(GROUP_TAG::SAVE_CHECK_MENU);

	UI* background = CreateImage(x, y, nullptr, upgradeHoverBackground, group, true, true);

	CreateText(5, -3, background, "        The game has saved", group);

	CreateText(5, 7, background, "               correctly.", group);

	CreateSaveOKButton((upgradeHoverBackground.w * 0.5) - (okButton.w * 0.5), 30, background, group);

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


UI* UIFactory::CreateContinueGameButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, menuButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::CONTINUE_GAME);

	group->AddUiElement(button);

	CreateText(25, 5, button, "C O N T I N U E    G A M E", group);

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
	Button* button = new Button(fMPoint{ x, y }, parent, closeButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::CLOSE_SAVE_OK_MENU);

	group->AddUiElement(button);

	return button;
}


UI* UIFactory::CreateSaveOKButton(float x, float y, UI* parent, UI_Group* group)
{
	Button* button = new Button(fMPoint{ x, y }, parent, okButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::CLOSE_SAVE_OK_MENU);
	group->AddUiElement(button);

	return button;
}


UI* UIFactory::CreateGathererReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 30, 30 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::REVIVE_GATHERER, reviveGathererButton);
	dataPagesVector->push_back(button);

	return button;
}



UI* UIFactory::CreateMeleeReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 30, 30 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::REVIVE_MELEE, reviveMeleeButton);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateRangedReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 30, 30 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::REVIVE_RANGED, reviveRangedButton);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateRobottoReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 30, 30 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::REVIVE_ROBOTTO, reviveRobottoButton);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateLifeUpgradeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, BUTTON_TAG tag)
{
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), tag, lifeUpgradeButton);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateDamageUpgradeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, BUTTON_TAG tag)
{
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), tag, damageUpgradeButton);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateEnergyUpgradeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, BUTTON_TAG tag)
{
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), tag, energyUpgradeButton);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateAttackSpeedUpgradeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, BUTTON_TAG tag)
{
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), tag, attackSpeedUpgradeButton);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateGathererPassive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Hero* focus = (Hero*)app->player->GetFocusedEntity();
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::GATHERER_PASSIVE1, gathererPassive1Button);
	dataPagesVector->push_back(button);

	if (CheckSkillResources() == true && focus->GetPassiveSkill().lvl < 5)
	{
		Button* button = new Button(fMPoint{ x, y - 13 }, parent, ugradeSkillButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::GATHERER_PASSIVE1_UPGRADE);
		dataPagesVector->push_back(button);
	}

	return button;
}


UI* UIFactory::CreateGathererActive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Hero* focus = (Hero*)app->player->GetFocusedEntity();
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::GATHERER_ACTIVE1, gathererActive1Button);
	dataPagesVector->push_back(button);

	if (CheckSkillResources() == true && focus->GetSkill1().lvl < 5)
	{
		Button* button = new Button(fMPoint{ x, y - 13 }, parent, ugradeSkillButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::GATHERER_ACTIVE1_UPGRADE);
		dataPagesVector->push_back(button);
	}

	return button;
}


UI* UIFactory::CreateMeleePassive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Hero* focus = (Hero*)app->player->GetFocusedEntity();
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::MELEE_PASSIVE1, meleePassive1Button);
	dataPagesVector->push_back(button);

	if (CheckSkillResources() == true && focus->GetPassiveSkill().lvl < 5)
	{
		Button* button = new Button(fMPoint{ x, y - 13 }, parent, ugradeSkillButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::MELEE_PASSIVE1_UPGRADE);
		dataPagesVector->push_back(button);
	}

	return button;
}


UI* UIFactory::CreateMeleeActive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Hero* focus = (Hero*)app->player->GetFocusedEntity();
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::MELEE_ACTIVE1, meleeActive1Button);
	dataPagesVector->push_back(button);

	if (CheckSkillResources() == true && focus->GetSkill1().lvl < 5)
	{
		Button* button = new Button(fMPoint{ x, y - 13 }, parent, ugradeSkillButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::MELEE_ACTIVE1_UPGRADE);
		dataPagesVector->push_back(button);
	}

	return button;
}


UI* UIFactory::CreateRangedPassive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Hero* focus = (Hero*)app->player->GetFocusedEntity();
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::RANGED_PASSIVE1, rangedPassive1Button);
	dataPagesVector->push_back(button);

	if (CheckSkillResources() == true && focus->GetPassiveSkill().lvl < 5)
	{
		Button* button = new Button(fMPoint{ x, y - 13 }, parent, ugradeSkillButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::RANGED_PASSIVE1_UPGRADE);
		dataPagesVector->push_back(button);
	}

	return button;
}


UI* UIFactory::CreateRangedActive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Hero* focus = (Hero*)app->player->GetFocusedEntity();
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::RANGED_ACTIVE1, rangedActive1Button);
	dataPagesVector->push_back(button);

	if (CheckSkillResources() == true && focus->GetSkill1().lvl < 5)
	{
		Button* button = new Button(fMPoint{ x, y - 13 }, parent, ugradeSkillButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::RANGED_ACTIVE1_UPGRADE);
		dataPagesVector->push_back(button);
	}

	return button;
}


UI* UIFactory::CreateRobottoPassive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Hero* focus = (Hero*)app->player->GetFocusedEntity();
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::ROBOTTO_PASSIVE1, robottoPassive1Button);
	dataPagesVector->push_back(button);

	if (CheckSkillResources() == true && focus->GetPassiveSkill().lvl < 5)
	{
		Button* button = new Button(fMPoint{ x, y - 13 }, parent, ugradeSkillButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::ROBOTTO_PASSIVE1_UPGRADE);
		dataPagesVector->push_back(button);
	}

	return button;
}


UI* UIFactory::CreateRobottoActive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Hero* focus = (Hero*)app->player->GetFocusedEntity();
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 15, 15 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::ROBOTTO_ACTIVE1, robottoActive1Button);
	dataPagesVector->push_back(button);

	if (CheckSkillResources() == true && focus->GetSkill1().lvl < 5)
	{
		Button* button = new Button(fMPoint{ x, y - 13 }, parent, ugradeSkillButton, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::ROBOTTO_ACTIVE1_UPGRADE);
		dataPagesVector->push_back(button);
	}

	return button;
}


UI* UIFactory::CreateBuyTurretButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, turretShopPicture, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::BUY_TURRET);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateUpgradeTurretButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, turretShopPicture, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::UPGRADE_TURRET);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateBuyUpgradeMenuButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, { 0, 0, 30, 30 }, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::BUY_UPGRADE_CENTER, upgradeCenterPurchaseButton);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateBuyBarricadeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, barricadeShopPicture, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::BUY_BARRICADE);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateUpgradeBarricadeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector)
{
	Button* button = new Button(fMPoint{ x, y }, parent, barricadeShopPicture, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::UPGRADE_BARRICADE);
	dataPagesVector->push_back(button);

	return button;
}


UI* UIFactory::CreateMusicScrollbar(float x, float y, UI* parent, UI_Group* group)
{
	UI_Scrollbar* scrollbar = new UI_Scrollbar(x, y, parent, scrollbarButton, app->uiManager->GetAtlasTexture(), 128.0f);

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

	//resources

	CreateImage(24, 7, background, resourceIcon, group);

	//skill

	CreateImage(3, 37, background, resourceIconSkill, group);

	//boost

	CreateImage(41, 37, background, resourceIconBoost, group);

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
	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, gathererPicture);

	CreateGathererPassive1Button(68, 0, dataPage, dataPagesVector);
	CreateGathererActive1Button(85, 0, dataPage, dataPagesVector);

	CreateLifeUpgradeButton(-15, 0, dataPage, dataPagesVector, BUTTON_TAG::GATHERER_LIFE_UPGRADE);
	CreateDamageUpgradeButton(-15, 17, dataPage, dataPagesVector, BUTTON_TAG::GATHERER_DAMAGE_UPGRADE);
	CreateEnergyUpgradeButton(-15, 34, dataPage, dataPagesVector, BUTTON_TAG::GATHERER_ENERGY_UPGRADE);
	CreateAttackSpeedUpgradeButton(-15, 51, dataPage, dataPagesVector, BUTTON_TAG::GATHERER_ATTACK_SPEED_UPGRADE);

	CreateGenericHeroPage(dataPagesVector, dataPage);
}


void UIFactory::CreateMeleePage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, meleePicture);

	CreateMeleePassive1Button(68, 0, dataPage, dataPagesVector);
	CreateMeleeActive1Button(85, 0, dataPage, dataPagesVector);

	CreateLifeUpgradeButton(-15, 0, dataPage, dataPagesVector, BUTTON_TAG::MELEE_LIFE_UPGRADE);
	CreateDamageUpgradeButton(-15, 17, dataPage, dataPagesVector, BUTTON_TAG::MELEE_DAMAGE_UPGRADE);
	CreateEnergyUpgradeButton(-15, 34, dataPage, dataPagesVector, BUTTON_TAG::MELEE_ENERGY_UPGRADE);
	CreateAttackSpeedUpgradeButton(-15, 51, dataPage, dataPagesVector, BUTTON_TAG::MELEE_ATTACK_SPEED_UPGRADE);

	CreateGenericHeroPage(dataPagesVector, dataPage);
}


void UIFactory::CreateRangedPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, rangedPicture);

	CreateRangedPassive1Button(68, 0, dataPage, dataPagesVector);
	CreateRangedActive1Button(85, 0, dataPage, dataPagesVector);

	CreateLifeUpgradeButton(-15, 0, dataPage, dataPagesVector, BUTTON_TAG::RANGED_LIFE_UPGRADE);
	CreateDamageUpgradeButton(-15, 17, dataPage, dataPagesVector, BUTTON_TAG::RANGED_DAMAGE_UPGRADE);
	CreateEnergyUpgradeButton(-15, 34, dataPage, dataPagesVector, BUTTON_TAG::RANGED_ENERGY_UPGRADE);
	CreateAttackSpeedUpgradeButton(-15, 51, dataPage, dataPagesVector, BUTTON_TAG::RANGED_ATTACK_SPEED_UPGRADE);

	CreateGenericHeroPage(dataPagesVector, dataPage);
}


void UIFactory::CreateRobottoPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, robottoPicture);

	CreateRobottoPassive1Button(68, 0, dataPage, dataPagesVector);
	CreateRobottoActive1Button(85, 0, dataPage, dataPagesVector);

	CreateLifeUpgradeButton(-15, 0, dataPage, dataPagesVector, BUTTON_TAG::ROBOTTO_LIFE_UPGRADE);
	CreateDamageUpgradeButton(-15, 17, dataPage, dataPagesVector, BUTTON_TAG::ROBOTTO_DAMAGE_UPGRADE);
	CreateEnergyUpgradeButton(-15, 34, dataPage, dataPagesVector, BUTTON_TAG::ROBOTTO_ENERGY_UPGRADE);
	CreateAttackSpeedUpgradeButton(-15, 51, dataPage, dataPagesVector, BUTTON_TAG::ROBOTTO_ATTACK_SPEED_UPGRADE);

	CreateGenericHeroPage(dataPagesVector, dataPage);
}


void UIFactory::CreateGenericHeroPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Hero* focus = (Hero*)app->player->GetFocusedEntity();

	CreateNonGroupImage(68, 15, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 17, dataPage, dataPagesVector, healthbarGreenImage);

	CreateNonGroupImage(68, 23, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 25, dataPage, dataPagesVector, healthbarBlueImage);

	//stats
	sprintf_s(stats, 40, "AD: %.0f", focus->GetAttackDamage());
	CreateNonGroupText(133, 10, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Exp: %i/%i", focus->GetHeroXP(), focus->GetExpToLevelUp());
	CreateNonGroupText(68, 22, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AS: %.2f", focus->GetAttackSpeed());
	CreateNonGroupText(133, 22, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rng: %i", focus->GetAttackRange());
	CreateNonGroupText(68, 35, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rec: %i", focus->GetRecoveryHitPointsRate());
	CreateNonGroupText(133, 35, dataPage, dataPagesVector, stats);

	if (focus->GetHeroSkillPoints() > 0)
	{
		CreateNonGroupImage(gathererPicture.w - 30, -upgradeArrowIcon.h - 1, dataPage, dataPagesVector, upgradeArrowIcon);

		sprintf_s(stats, 40, "x%i", focus->GetHeroSkillPoints());
		CreateNonGroupText(gathererPicture.w - 16, -upgradeArrowIcon.h - 10, dataPage, dataPagesVector, stats);
	}
}


void UIFactory::CreateWanamingoPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Enemy* focus = (Enemy*)app->player->GetFocusedEntity();

	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, wanamingoDataPagePicture);

	CreateNonGroupImage(68, 19, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 21, dataPage, dataPagesVector, healthbarGreenImage);

	//stats
	sprintf_s(stats, 40, "AD: %i", focus->GetAD());
	CreateNonGroupText(133, 7, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rng: %i", focus->GetVision());
	CreateNonGroupText(68, 19, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AS: %.2f", focus->GetAS());
	CreateNonGroupText(133, 19, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rec: %i", focus->GetRecov());
	CreateNonGroupText(68, 32, dataPage, dataPagesVector, stats);
}


void UIFactory::CreateSpeedomingoPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Enemy* focus = (Enemy*)app->player->GetFocusedEntity();

	CreateNonGroupImage(13, 10, dataPage, dataPagesVector, speedomingoDataPagePicture);

	CreateNonGroupImage(68, 19, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 21, dataPage, dataPagesVector, healthbarGreenImage);

	//stats
	sprintf_s(stats, 40, "AD: %i", focus->GetAD());
	CreateNonGroupText(133, 7, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rng: %i", focus->GetVision());
	CreateNonGroupText(68, 19, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AS: %.2f", focus->GetAS());
	CreateNonGroupText(133, 19, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rec: %i", focus->GetRecov());
	CreateNonGroupText(68, 32, dataPage, dataPagesVector, stats);
}


void UIFactory::CreateSnipermingoPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Enemy* focus = (Enemy*)app->player->GetFocusedEntity();

	CreateNonGroupImage(13, 10, dataPage, dataPagesVector, snipermingoDataPagePicture);

	CreateNonGroupImage(68, 19, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 21, dataPage, dataPagesVector, healthbarGreenImage);

	//stats
	sprintf_s(stats, 40, "AD: %i", focus->GetAD());
	CreateNonGroupText(133, 7, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rng: %i", focus->GetVision());
	CreateNonGroupText(68, 19, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AS: %.2f", focus->GetAS());
	CreateNonGroupText(133, 19, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rec: %i", focus->GetRecov());
	CreateNonGroupText(68, 32, dataPage, dataPagesVector, stats);
}


void UIFactory::CreateGigamingoPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Enemy* focus = (Enemy*)app->player->GetFocusedEntity();

	CreateNonGroupImage(18, 3, dataPage, dataPagesVector, gigamingoDataPagePicture);

	CreateNonGroupImage(68, 19, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 21, dataPage, dataPagesVector, healthbarGreenImage);

	//stats
	sprintf_s(stats, 40, "AD: %i", focus->GetAD());
	CreateNonGroupText(133, 7, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rng: %i", focus->GetVision());
	CreateNonGroupText(68, 19, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AS: %.2f", focus->GetAS());
	CreateNonGroupText(133, 19, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rec: %i", focus->GetRecov());
	CreateNonGroupText(68, 32, dataPage, dataPagesVector, stats);
}


void UIFactory::CreateBasePage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Base* focus = (Base*)app->player->GetFocusedEntity();

	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, baseDataPagePicture);

	CreateNonGroupImage(68, 3, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 5, dataPage, dataPagesVector, healthbarGreenImage);

	CreateGathererReviveButton(158, 5, dataPage, dataPagesVector);

	CreateMeleeReviveButton(158, 35, dataPage, dataPagesVector);

	CreateRangedReviveButton(128, 35, dataPage, dataPagesVector);

	CreateRobottoReviveButton(98, 35, dataPage, dataPagesVector);

	CreateBuyUpgradeMenuButton(68, 35, dataPage, dataPagesVector);

	//stats
	sprintf_s(stats, 40, "Resources: %i", focus->GetRsrc());
	CreateNonGroupText(68, 5, dataPage, dataPagesVector, stats);

}


void UIFactory::CreateNonPlayerBasePage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, baseDataPagePicture);

	CreateNonGroupImage(68, 8, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 10, dataPage, dataPagesVector, healthbarGreenImage);
}


void UIFactory::CreateTurretPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Turret* focus = (Turret*)app->player->GetFocusedEntity();

	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, turretDataPagePicture);

	CreateNonGroupImage(68, 8, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 10, dataPage, dataPagesVector, healthbarGreenImage);

	//stats
	sprintf_s(stats, 40, "LVL: %i", focus->GetLvl());
	CreateNonGroupText(69, 13, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AD: %i", focus->GetAD());
	CreateNonGroupText(135, 13, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "Rng: %i", focus->GetRng());
	CreateNonGroupText(69, 28, dataPage, dataPagesVector, stats);

	sprintf_s(stats, 40, "AS: %.i", focus->GetAS());
	CreateNonGroupText(135, 28, dataPage, dataPagesVector, stats);
}


void UIFactory::CreateUpgradeCenterPage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, upgradeCenterDataPagePicture);

	CreateNonGroupImage(68, 8, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 10, dataPage, dataPagesVector, healthbarGreenImage);

	CreateBuyTurretButton(68, 23, dataPage, dataPagesVector);

	CreateNonGroupImage(68, 23, dataPage, dataPagesVector, plusIcon, false, false);

	CreateUpgradeTurretButton(98, 23, dataPage, dataPagesVector);

	CreateNonGroupImage(98, 23, dataPage, dataPagesVector, upgradeArrowIcon, false, false);

	CreateBuyBarricadeButton(128, 23, dataPage, dataPagesVector);

	CreateNonGroupImage(128, 23, dataPage, dataPagesVector, plusIcon, false, false);

	CreateUpgradeBarricadeButton(158, 23, dataPage, dataPagesVector);

	CreateNonGroupImage(158, 23, dataPage, dataPagesVector, upgradeArrowIcon, false, false);
}


void UIFactory::CreateBarricadePage(std::vector<UI*>* dataPagesVector, UI* dataPage)
{
	char stats[40];
	Barricade* focus = (Barricade*)app->player->GetFocusedEntity();

	CreateNonGroupImage(3, 3, dataPage, dataPagesVector, barricadeDataPagePicture);

	CreateNonGroupImage(68, 8, dataPage, dataPagesVector, healthBarContainer);

	CreateNonGroupImage(69, 10, dataPage, dataPagesVector, healthbarGreenImage);

	sprintf_s(stats, 40, "LVL: %i", focus->GetLevel());
	CreateNonGroupText(69, 13, dataPage, dataPagesVector, stats);
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
		BackGround = new Button(fMPoint{ (float)(-portraitRoundBackground.w * 0.5), 0 }, portrait, portraitRoundBackground, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::GATHERER_PORTRAIT);
		portrait->AddElement(BackGround);

		icon = new UI_Image(11, 6, BackGround, gathererHeroPortrait, app->uiManager->GetAtlasTexture(), false, false);
		portrait->AddElement(icon);

		break;


	case ENTITY_TYPE::HERO_MELEE:
		BackGround = new Button(fMPoint{ (float)(-portraitRoundBackground.w * 0.5), 0 }, portrait, portraitRoundBackground, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::MELEE_PORTRAIT);
		portrait->AddElement(BackGround);

		icon = new UI_Image(11, 6, BackGround, meleHeroPortrait, app->uiManager->GetAtlasTexture(), false, false);
		portrait->AddElement(icon);

		break;


	case ENTITY_TYPE::HERO_RANGED:
		BackGround = new Button(fMPoint{ (float)(-portraitRoundBackground.w * 0.5), 0 }, portrait, portraitRoundBackground, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::RANGED_PORTRAIT);
		portrait->AddElement(BackGround);

		icon = new UI_Image(11, 6, BackGround, rangedHeroPortrait, app->uiManager->GetAtlasTexture(), false, false);
		portrait->AddElement(icon);

		break;

	case ENTITY_TYPE::HERO_ROBO:
		BackGround = new Button(fMPoint{ (float)(-portraitRoundBackground.w * 0.5), 0 }, portrait, portraitRoundBackground, false, app->uiManager->GetAtlasTexture(), BUTTON_TAG::ROBO_PORTRAIT);
		portrait->AddElement(BackGround);

		icon = new UI_Image(11, 6, BackGround, robottoHeroPortrait, app->uiManager->GetAtlasTexture(), false, false);
		portrait->AddElement(icon);

		break;


	default:
		assert("shouldt have anything that is not a hero");
		break;
	}


	healthBar = new UI_Image(6, 38, BackGround, healthbarGreenImage, app->uiManager->GetAtlasTexture(), false, false);
	portrait->AddHealthBar(healthBar);

	healthBarCont = new UI_Image(5, 37, BackGround, healthBarContainer, app->uiManager->GetAtlasTexture(), false, false);
	portrait->AddElement(healthBarCont);

	manaBar = new UI_Image(6, 46, BackGround, healthbarBlueImage, app->uiManager->GetAtlasTexture(), false, false);
	portrait->AddManaBar(manaBar);

	manaBarCont = new UI_Image(5, 45, BackGround, healthBarContainer, app->uiManager->GetAtlasTexture(), false, false);
	portrait->AddElement(manaBarCont);

	return portrait;
}

SDL_Rect UIFactory::GetHealthBarBackground()
{
	return healthBarContainer;
}


SDL_Rect UIFactory::GetGreenHealthBar()
{
	return healthbarGreenImage;
}


SDL_Rect UIFactory::GetBlueHealthBar()
{
	return healthbarBlueImage;
}


void UIFactory::ResetUpgradeCost()
{
	gathererLifeUpgradeCost = 100;
	gathererDamageUpgradeCost = 100;
	gathererEnergyUpgradeCost = 100;
	gathererAtkSpeedUpgradeCost = 100;

	meleeLifeUpgradeCost = 100;
	meleeDamageUpgradeCost = 100;
	meleeEnergyUpgradeCost = 100;
	meleeAtkSpeedUpgradeCost = 100;

	rangedLifeUpgradeCost = 100;
	rangedDamageUpgradeCost = 100;
	rangedEnergyUpgradeCost = 100;
	rangedAtkSpeedUpgradeCost = 100;

	robottoLifeUpgradeCost = 100;
	robottoDamageUpgradeCost = 100;
	robottoEnergyUpgradeCost = 100;
	robottoAtkSpeedUpgradeCost = 100;
}


bool UIFactory::CheckSkillResources()
{
	Hero* hero = (Hero*)app->player->focusedEntity;

	if (app->player->GetResourcesSkill() > 0)
	{
		return true;
	}

	else if (hero->GetHeroSkillPoints() > 0) // A variable has to exist to check if he has leveled up and has the points
	{
		return true;
	}

	return false;
}


