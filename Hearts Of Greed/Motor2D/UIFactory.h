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
enum class ENTITY_TYPE;

struct UIFactory
{
public:

	UIFactory();
	~UIFactory();

	void ResetUpgradeCost();

	// Getters

	SDL_Rect GetHealthBarBackground();
	SDL_Rect GetGreenHealthBar();
	SDL_Rect GetBlueHealthBar();

	// Menu creation functions

	UI_Group* CreateMainMenu();
	UI_Group* CreateOptionsMenu();
	UI_Group* CreateCreditsMenu();

	UI_Group* CreateDialogMenu(ENTITY_TYPE character1, ENTITY_TYPE character2);
	UI_Group* CreatePopUp(P2SString& string);
	
	// Image creation functions

	UI* CreateImage(float x, float y, UI* parent, SDL_Rect rect, UI_Group* group, bool dragable = false, bool interactable = true);
	UI* CreateNonGroupImage(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, SDL_Rect rect, bool dragable = false, bool interactable = true);

	// Text creation functions

	UI* CreateText(float x, float y, UI* parent, char* text, UI_Group* group, bool interactable = false, Uint32 lenght = 0);
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

	UI* CreateSaveOKButton(float x, float y, UI* parent, UI_Group* group);

	// Data Page Buttons

	UI* CreateGathererReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateMeleeReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateRangedReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateRobottoReviveButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateBuyTurretButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateUpgradeTurretButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateBuyUpgradeMenuButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateBuyBarricadeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateUpgradeBarricadeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);

	UI* CreateLifeUpgradeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, BUTTON_TAG tag);
	UI* CreateDamageUpgradeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, BUTTON_TAG tag);
	UI* CreateEnergyUpgradeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, BUTTON_TAG tag);
	UI* CreateAttackSpeedUpgradeButton(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector, BUTTON_TAG tag);

	UI* CreateGathererPassive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateGathererActive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateMeleePassive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateMeleeActive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateRangedPassive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateRangedActive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateRobottoPassive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);
	UI* CreateRobottoActive1Button(float x, float y, UI* parent, std::vector<UI*>* dataPagesVector);


	// Hover menu creation functions

	UI_Group* CreateBasicInGameUI();
	UI_Group* CreatePauseMenu();
	UI_Group* CreateOnHoverReviveMenu(Button* button);
	UI_Group* CreateOnHoverBuyTurretMenu();
	UI_Group* CreateOnHoverUpgradeTurretMenu();
	UI_Group* CreateOnHoverBuyUpgradeCenterMenu();
	UI_Group* CreateOnHoverBuyBarricadeMenu();
	UI_Group* CreateOnHoverUpgradeBarricadeMenu();
	UI_Group* CreateOnHoverGathererLifeUpgradeMenu();
	UI_Group* CreateOnHoverGathererDamageUpgradeMenu();
	UI_Group* CreateOnHoverGathererEnergyUpgradeMenu();
	UI_Group* CreateOnHoverGathererAttackSpeedUpgradeMenu();
	UI_Group* CreateOnHoverMeleeLifeUpgradeMenu();
	UI_Group* CreateOnHoverMeleeDamageUpgradeMenu();
	UI_Group* CreateOnHoverMeleeEnergyUpgradeMenu();
	UI_Group* CreateOnHoverMeleeAttackSpeedUpgradeMenu();
	UI_Group* CreateOnHoverRangedLifeMenuMenu();
	UI_Group* CreateOnHoverRangedDamageUpgradeMenu();
	UI_Group* CreateOnHoverRangedEnergyUpgradeMenu();
	UI_Group* CreateOnHoverRangedAttackSpeedUpgradeMenu();
	UI_Group* CreateOnHoverRobottoLifeMenuMenu();
	UI_Group* CreateOnHoverRobottoDamageUpgradeMenu();
	UI_Group* CreateOnHoverRobottoEnergyUpgradeMenu();
	UI_Group* CreateOnHoverRobottoAttackSpeedUpgradeMenu();
	UI_Group* CreateOnHoverGathererPassive1Menu();
	UI_Group* CreateOnHoverGathererActive1Menu();
	UI_Group* CreateOnHoverMeleePassive1Menu();
	UI_Group* CreateOnHoverMeleeActive1Menu();
	UI_Group* CreateOnHoverRangedPassive1Menu();
	UI_Group* CreateOnHoverRangedActive1Menu();
	UI_Group* CreateOnHoverRobottoPassive1Menu();
	UI_Group* CreateOnHoverRobottoActive1Menu();
	UI_Group* CreateOnHoverGathererPassive1UpgradeMenu();
	UI_Group* CreateOnHoverGathererActive1UpgradeMenu();
	UI_Group* CreateOnHoverMeleePassive1UpgradeMenu();
	UI_Group* CreateOnHoverMeleeActive1UpgradeMenu();
	UI_Group* CreateOnHoverRangedPassive1UpgradeMenu();
	UI_Group* CreateOnHoverRangedActive1UpgradeMenu();
	UI_Group* CreateOnHoverRobottoPassive1UpgradeMenu();
	UI_Group* CreateOnHoverRobottoActive1UpgradeMenu();

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
	void CreateRobottoPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateGenericHeroPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateWanamingoPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateSpeedomingoPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateSnipermingoPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateGigamingoPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateBasePage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateNonPlayerBasePage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateTurretPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateUpgradeCenterPage(std::vector<UI*>* dataPagesVector, UI* dataPage);
	void CreateBarricadePage(std::vector<UI*>* dataPagesVector, UI* dataPage);

	UI* CreatePortraitManager(float x, float y, UI* parent, UI_Group* group);
	HeroPortrait* CreatePortrait(Hero* hero);

	UI_Group* CreateSaveConfirmationMenu();
	bool CheckSkillResources();

public:

	SDL_Rect fullscreenOnButton;
	SDL_Rect fullscreenOffButton;

private:
	// Image SDL_Rect

	SDL_Rect portraitBackground;
	SDL_Rect portraitRoundBackground;
	SDL_Rect portraitLevelBackground;
	SDL_Rect portraitHealthbarBackground;
	SDL_Rect portraitHealthbarGreenImage;

	SDL_Rect healthBarContainer;
	SDL_Rect healthbarGreenImage;
	SDL_Rect healthbarBlueImage;
	SDL_Rect dataPageBackground;
	SDL_Rect dataPageImageBackground;

	SDL_Rect gathererPicture;
	SDL_Rect gathererShopPicture;
	SDL_Rect gathererBigPicture;
	SDL_Rect meleePicture;
	SDL_Rect meleeShopPicture;
	SDL_Rect meleeBigPicture;
	SDL_Rect rangedPicture;
	SDL_Rect rangedShopPicture;
	SDL_Rect rangedBigPicture;
	SDL_Rect robottoPicture;
	SDL_Rect robottoShopPicture;
	SDL_Rect robottoBigPicture;
	SDL_Rect baseDataPagePicture;
	SDL_Rect upgradeCenterDataPagePicture;
	SDL_Rect turretDataPagePicture;
	SDL_Rect barricadeDataPagePicture;
	SDL_Rect wanamingoDataPagePicture;
	SDL_Rect speedomingoDataPagePicture;
	SDL_Rect snipermingoDataPagePicture;
	SDL_Rect gigamingoDataPagePicture;
	SDL_Rect turretShopPicture;
	SDL_Rect barricadeShopPicture;

	SDL_Rect resourceIcon;
	SDL_Rect resourceIconSkill;
	SDL_Rect resourceIconBoost;

	SDL_Rect creditsBackgroundImage;

	SDL_Rect pauseMenuBackground;
	SDL_Rect optionsMenuBackground;
	SDL_Rect creditsMenuBackground;
	SDL_Rect minimapBackground;
	SDL_Rect shopBackground;
	SDL_Rect resourcesBackground;

	SDL_Rect upgradeArrowIcon;
	SDL_Rect plusIcon;

	// Button SDL_Rect

	SDL_Rect menuButton;
	SDL_Rect pauseButton;
	SDL_Rect closeButton;
	//	SDL_Rect hideLeftButton;
	//	SDL_Rect hideRightButton;
	SDL_Rect shopButton;
	SDL_Rect upgradeCenterPurchaseButton;
	SDL_Rect reviveGathererButton;
	SDL_Rect reviveMeleeButton;
	SDL_Rect reviveRangedButton;
	SDL_Rect reviveRobottoButton;

	SDL_Rect okButton;

	// Scrollbar SDL_Rect

	SDL_Rect scrollbarBar;
	SDL_Rect scrollbarButton;


	SDL_Rect deadHeroIcon;
	SDL_Rect gathererHeroIcon;
	SDL_Rect meleHeroIcon;
	SDL_Rect rangedHeroIcon;
	SDL_Rect robottoHeroIcon;
	SDL_Rect gathererHeroPortrait;
	SDL_Rect meleHeroPortrait;
	SDL_Rect rangedHeroPortrait;
	SDL_Rect robottoHeroPortrait;
	SDL_Rect hqCommanderPortrait;

	SDL_Rect ugradeSkillButton;

	SDL_Rect lifeUpgradeButton;
	SDL_Rect damageUpgradeButton;
	SDL_Rect energyUpgradeButton;
	SDL_Rect attackSpeedUpgradeButton;

	SDL_Rect gathererPassive1Button;
	SDL_Rect meleePassive1Button;
	SDL_Rect rangedPassive1Button;
	SDL_Rect robottoPassive1Button;
	SDL_Rect gathererActive1Button;
	SDL_Rect meleeActive1Button;
	SDL_Rect rangedActive1Button;
	SDL_Rect robottoActive1Button;

	SDL_Rect dayNightIcon;

	// Hover menus SDL_Rect
	SDL_Rect reviveHoverBackground;
	SDL_Rect upgradeHoverBackground;

	// Minimap Frame
	SDL_Rect miniFrame;

	//Dialog things
	SDL_Rect dialogWindow;
	SDL_Rect skipButton;

	SDL_Rect popUpWindow;

public:

	float gathererLifeUpgradeCost;
	float gathererDamageUpgradeCost;
	float gathererEnergyUpgradeCost;
	float gathererAtkSpeedUpgradeCost;

	float meleeLifeUpgradeCost;
	float meleeDamageUpgradeCost;
	float meleeEnergyUpgradeCost;
	float meleeAtkSpeedUpgradeCost;

	float rangedLifeUpgradeCost;
	float rangedDamageUpgradeCost;
	float rangedEnergyUpgradeCost;
	float rangedAtkSpeedUpgradeCost;

	float robottoLifeUpgradeCost;
	float robottoDamageUpgradeCost;
	float robottoEnergyUpgradeCost;
	float robottoAtkSpeedUpgradeCost;

	int reviveCost;
	int turretUpgradeCost;
	int barricadeUpgradeCost;
};

#endif __UIFACTORY_H__