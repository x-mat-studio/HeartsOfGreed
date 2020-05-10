#ifndef __UIFACTORY_H__
#define __UIFACTORY_H__

#include "p2SString.h"
#include "SDL/include/SDL_rect.h"

class UI;
class UI_Group;
class Button;
class HeroPortrait;

class Entity;
class Hero;

enum class BUTTON_TAG;

struct UIFactory
{
public:

	UIFactory();
	~UIFactory();

	// Getters

	SDL_Rect GetHealthBarBackground();
	SDL_Rect GetGreenHealthBar();
	SDL_Rect GetBlueHealthBar();

	// Menu creation functions

	UI_Group* CreateMainMenu();
	UI_Group* CreateOptionsMenu();
	UI_Group* CreateCreditsMenu();

	// Image creation functions

	UI* CreateImage(float x, float y, UI* parent, SDL_Rect rect, UI_Group* group, bool dragable = false, bool interactable = true);
	UI* CreateNonGroupImage(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, SDL_Rect rect, bool dragable = false, bool interactable = true);

	// Text creation functions

	UI* CreateText(float x, float y, UI* parent, char* text, UI_Group* group, bool interactable = false);
	UI* CreateNonGroupText(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, char* text, bool interactable = false);

	// Button creation functions

	UI* CreateContinueGameButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateNewGameButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateOptionsButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateFullscreenButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateCreditsButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateExitGameButton(float x, float y, UI* parent, UI_Group* group);

	UI* CreatePauseGameButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateResumeGameButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateSaveGameButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateLoadGameButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateReturnToMainMenuButton(float x, float y, UI* parent, UI_Group* group);

	//	UI* CreateHideButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateCloseOptionMenuButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateCloseCreditsButton(float x, float y, UI* parent, UI_Group* group);
	UI* CreateClosePauseMenuButton(float x, float y, UI* parent, UI_Group* group);

	// Data Page Buttons

	UI* CreateGathererReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateMeleeReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateRangedReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateBuyTurretButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateUpgradeTurretButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateBuyBarricadeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateUpgradeBarricadeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);

	UI* CreateLifeUpgradeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, BUTTON_TAG tag);
	UI* CreateDamageUpgradeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, BUTTON_TAG tag);

	UI* CreateGathererPassive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateGathererActive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateMeleePassive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateMeleeActive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateRangedPassive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateRangedActive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);

	// Hover menu creation functions

	UI_Group* CreateBasicInGameUI();
	UI_Group* CreatePauseMenu();
	UI_Group* CreateOnHoverReviveMenu(Button* button);
	UI_Group* CreateOnHoverBuyTurretMenu();
	UI_Group* CreateOnHoverUpgradeTurretMenu();
	UI_Group* CreateOnHoverBuyBarricadeMenu();
	UI_Group* CreateOnHoverUpgradeBarricadeMenu();
	UI_Group* CreateOnHoverGathererLifeUpgradeMenu();
	UI_Group* CreateOnHoverGathererDamageUpgradeMenu();
	UI_Group* CreateOnHoverMeleeLifeUpgradeMenu();
	UI_Group* CreateOnHoverMeleeDamageUpgradeMenu();
	UI_Group* CreateOnHoverRangedLifeMenuMenu();
	UI_Group* CreateOnHoverRangedDamageUpgradeMenu();
	UI_Group* CreateOnHoverGathererPassive1Menu();
	UI_Group* CreateOnHoverGathererActive1Menu();
	UI_Group* CreateOnHoverMeleePassive1Menu();
	UI_Group* CreateOnHoverMeleeActive1Menu();
	UI_Group* CreateOnHoverRangedPassive1Menu();
	UI_Group* CreateOnHoverRangedActive1Menu();

	// Scrollbar creation functions

	UI* CreateMusicScrollbar(float x, float y, UI* parent, UI_Group* group);
	UI* CreateSFXScrollbar(float x, float y, UI* parent, UI_Group* group);

	//Portrait things
	UI* CreateResourcesPortrait(float x, float y, UI* parent, UI_Group* group);

	// Data Pages creation functions

	UI* CreateDataPage(UI* parent, Entity* entity, UI_Group* group);
	void CreateGathererPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateMeleePage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateRangedPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateGenericHeroPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateWanamingoPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateBasePage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateNonPlayerBasePage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateTurretPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateUpgradeCenterPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateBarricadePage(std::vector<UI*>* dataPagesVector, UI* dataPage);

	UI* CreatePortraitManager(float x, float y, UI* parent, UI_Group* group);
	HeroPortrait* CreatePortrait(Hero* hero);

public:

	SDL_Rect fullscreenOnButton;
	SDL_Rect fullscreenOffButton;


private:
	// Image SDL_Rect

	SDL_Rect portraitBackground;
	SDL_Rect portraitLevelBackground;
	SDL_Rect portraitHealthbarBackground;
	SDL_Rect portraitHealthbarGreenImage;

	SDL_Rect healthBarContainer;
	SDL_Rect dataPageHealthbarGreenImage;
	SDL_Rect dataPageHealthbarBlueImage;
	SDL_Rect dataPageBackground;
	SDL_Rect dataPageImageBackground;

	SDL_Rect littleHealthBarContainer;
	SDL_Rect littleHealthbarGreenImage;
	SDL_Rect littleHealthbarBlueImage;

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
	SDL_Rect shopButton;
	SDL_Rect reviveButton;

	// Scrollbar SDL_Rect

	SDL_Rect scrollbarBar;
	SDL_Rect scrollbarButton;


	SDL_Rect heroPortrait;
	SDL_Rect gathererHeroIcon;
	SDL_Rect meleHeroIcon;
	SDL_Rect rangedHeroIcon;

	SDL_Rect lifeUpgradeButton;
	SDL_Rect damageUpgradeButton;

	SDL_Rect gathererPassive1Button;
	SDL_Rect meleePassive1Button;
	SDL_Rect rangedPassive1Button;
	SDL_Rect gathererActive1Button;
	SDL_Rect meleeActive1Button;
	SDL_Rect rangedActive1Button;

	// Hover menus SDL_Rect
	SDL_Rect reviveHoverBackground;
	SDL_Rect upgradeHoverBackground;

public:

	float gathererLifeUpgradeCost;
	float gathererDamageUpgradeCost;
	float gathererManaUpgradeCost;
	float gathererSpeedUpgradeCost;
	float gathererLifeUpgradeValue;
	float gathererDamageUpgradeValue;
	float gathererManaUpgradeValue;
	float gathererSpeedUpgradeValue;

	float meleeLifeUpgradeCost;
	float meleeDamageUpgradeCost;
	float meleeManaUpgradeCost;
	float meleeSpeedUpgradeCost;
	float meleeLifeUpgradeValue;
	float meleeDamageUpgradeValue;
	float meleeManaUpgradeValue;
	float meleeSpeedUpgradeValue;

	float rangedLifeUpgradeCost;
	float rangedDamageUpgradeCost;
	float rangedLifeUpgradeCost;
	float rangedDamageUpgradeCost;
	float rangedLifeUpgradeValue;
	float rangedDamageUpgradeValue;
	float rangedLifeUpgradeValue;
	float rangedDamageUpgradeValue;
};

#endif __UIFACTORY_H__