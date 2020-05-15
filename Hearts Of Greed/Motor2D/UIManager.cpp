#include "App.h"

#include "UIManager.h"
#include "UI.h"
#include "UI_Group.h"
#include "UIFactory.h"

#include "Button.h"
#include "HeroesPortraitManager.h"
#include "HeroPortrait.h"

#include "Audio.h"
#include "Window.h"
#include "Input.h"
#include "Player.h"
#include "EntityManager.h"
#include "MainMenuScene.h"
#include "TestScene.h"
#include "EventManager.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "DialogManager.h"

#include "Minimap.h"


#include "Base.h"
#include "Brofiler/Brofiler/Brofiler.h"

ModuleUIManager::ModuleUIManager() :

	Module(),

	mouseOverUI(false),
	atlas(nullptr),
	lastShop(nullptr),
	portraitManager(nullptr),
	hoverElement(nullptr),
	dragElement(nullptr),
	isMenuOn(false),
	factory(nullptr),
	clickSound(-1),
	hoverSound(-1)
{
	name.create("UIManager");
}


// Destructor
ModuleUIManager::~ModuleUIManager()
{
	delete factory;

	app->tex->UnLoad(atlas);
	atlas = nullptr;

	UnregisterEvents();

	uiGroupVector.clear();

}


// Called before render is available
bool ModuleUIManager::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("UI Manager Awake", Profiler::Color::MediumPurple);

	bool ret = true;

	app->eventManager->EventRegister(EVENT_ENUM::GAME_SCENE_ENTERED, this);
	app->eventManager->EventRegister(EVENT_ENUM::CREATE_OPTION_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::CREATE_CREDIT_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::CREATE_INTRO_MENU, this);

	app->eventManager->EventRegister(EVENT_ENUM::PAUSE_GAME, this);

	app->eventManager->EventRegister(EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU, this);

	app->eventManager->EventRegister(EVENT_ENUM::MUSIC_ADJUSTMENT, this);
	app->eventManager->EventRegister(EVENT_ENUM::SFX_ADJUSTMENT, this);
	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_DEAD, this);

	app->eventManager->EventRegister(EVENT_ENUM::DELETE_OPTIONS_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::DELETE_CREDITS_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::DELETE_PAUSE_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::DELETE_IN_HOVER_MENU, this);

	app->eventManager->EventRegister(EVENT_ENUM::FULLSCREEN_INPUT, this);

	app->eventManager->EventRegister(EVENT_ENUM::HIDE_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::UNHIDE_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::EXIT_MENUS, this);

	app->eventManager->EventRegister(EVENT_ENUM::CREATE_DIALOG_WINDOW, this);
	app->eventManager->EventRegister(EVENT_ENUM::CLOSE_DIALOG_WINDOW, this);

	return ret;
}


// Called before the first frame
bool ModuleUIManager::Start()
{
	bool ret = true;

	factory = new UIFactory();

	LoadAtlas();

	hoverSound = app->audio->LoadFx("audio/sfx/Interface/BotonSimple.wav");
	clickSound = app->audio->LoadFx("audio/sfx/Interface/BotonClick.wav");

	return ret;
}

// Called each loop iteration
bool ModuleUIManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("UI Manager Pre-Update", Profiler::Color::Purple);

	bool ret = true;

	CheckFocusedUI();

	CheckListener(this);

	AddPendingPortraits();

	MouseOnUI();

	int numEntities = uiGroupVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		uiGroupVector[i]->PreUpdate(dt);
	}

	return true;
}

// Called each loop iteration
bool ModuleUIManager::Update(float dt)
{
	BROFILER_CATEGORY("UI Manager Update", Profiler::Color::Purple);

	bool ret = true;

	CheckListener(this);

	DragElement();

	for (uint i = 0; i < uiGroupVector.size(); i++)
	{
		uiGroupVector[i]->Update(dt);
	}

	return ret;
}

// Called each loop iteration
bool ModuleUIManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("UI Manager Post Update", Profiler::Color::Purple);

	bool ret = true;

	for (uint i = 0; i < uiGroupVector.size(); i++)
	{
		uiGroupVector[i]->PostUpdate(dt);
	}

	return ret;
}

//// Called before quitting
bool ModuleUIManager::CleanUp()
{
	for (int i = 0; i < uiGroupVector.size(); i++)
	{
		RELEASE(uiGroupVector[i]);
		uiGroupVector[i] = nullptr;

		uiGroupVector.erase(uiGroupVector.begin() + i);
		i--;
	}

	uiGroupVector.clear();

	lastShop = nullptr;
	dragElement = nullptr;
	hoverElement = nullptr;

	return true;
}


void ModuleUIManager::AddUIGroup(UI_Group* element)
{
	GROUP_TAG tag = element->GetTag();

	if (tag == GROUP_TAG::NONE || CheckGroupTag(tag) == false)
	{
		delete element;
		return;
	}

	uiGroupVector.push_back(element);
}


bool ModuleUIManager::DeleteUIGroup(GROUP_TAG tag)
{
	int numUiGroups = uiGroupVector.size();

	for (int i = 0; i < numUiGroups; i++)
	{
		if (uiGroupVector[i]->GetTag() == tag)
		{
			delete uiGroupVector[i];
			uiGroupVector[i] = nullptr;

			uiGroupVector.erase(uiGroupVector.begin() + i);

			return true;
		}
	}


	return false;
}


void ModuleUIManager::ExecuteEvent(EVENT_ENUM eventId)
{
	UI_Group* group = nullptr;



	switch (eventId)
	{
	case EVENT_ENUM::CREATE_INTRO_MENU:

		group = factory->CreateMainMenu();
		AddUIGroup(group);
		break;

	case EVENT_ENUM::PAUSE_GAME:
		group = factory->CreatePauseMenu();
		AddUIGroup(group);
		break;

	case EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU:
		app->eventManager->GenerateEvent(EVENT_ENUM::RETURN_TO_MAIN_MENU, EVENT_ENUM::NULL_EVENT);
		break;

	case EVENT_ENUM::GAME_SCENE_ENTERED:

		group = factory->CreateBasicInGameUI();
		AddUIGroup(group);
		break;

	case EVENT_ENUM::MUSIC_ADJUSTMENT:			break;

	case EVENT_ENUM::SFX_ADJUSTMENT:			break;

	case EVENT_ENUM::ENTITY_DEAD:				break;

	case EVENT_ENUM::HIDE_MENU:					break;

	case EVENT_ENUM::UNHIDE_MENU:				break;

	case EVENT_ENUM::EXIT_MENUS:

		//If none of this menus is open and you are in the Game Scene, create pause menu
		if (CheckGroupTag(GROUP_TAG::DIALOG) == false)
			break;

		else if (DeleteUIGroup(GROUP_TAG::CREDITS_MENU) == true)
			break;

		else if (DeleteUIGroup(GROUP_TAG::OPTIONS_MENU) == true)
			break;

		else if (DeleteUIGroup(GROUP_TAG::PAUSE_MENU) == true) 
		{
			app->gamePause = false;
			break;
		}

		else if (app->testScene->IsEnabled() == true)
		{
			app->eventManager->GenerateEvent(EVENT_ENUM::PAUSE_GAME, EVENT_ENUM::NULL_EVENT);
			app->gamePause = true;
		}
		break;


	case EVENT_ENUM::DELETE_OPTIONS_MENU:
		DeleteUIGroup(GROUP_TAG::OPTIONS_MENU);
		break;


	case EVENT_ENUM::DELETE_CREDITS_MENU:
		DeleteUIGroup(GROUP_TAG::CREDITS_MENU);
		break;


	case EVENT_ENUM::DELETE_PAUSE_MENU:
		DeleteUIGroup(GROUP_TAG::PAUSE_MENU);
		app->gamePause = false;
		break;


	case EVENT_ENUM::DELETE_IN_HOVER_MENU:
		DeleteUIGroup(GROUP_TAG::IN_HOVER_MENU);
		break;

	case EVENT_ENUM::CREATE_DIALOG_WINDOW:
		group = factory->CreateDialogMenu(ENTITY_TYPE::HERO_GATHERER, ENTITY_TYPE::HERO_GATHERER);
		AddUIGroup(group);
		app->gamePause = true;
		break;

	case EVENT_ENUM::CLOSE_DIALOG_WINDOW:
		DeleteUIGroup(GROUP_TAG::DIALOG);
		app->gamePause = false;
		break;
	}
}


SDL_Texture* ModuleUIManager::GetAtlasTexture() const
{
	return atlas;
}

UIFactory* ModuleUIManager::GetFactory() const
{
	return factory;
}

void ModuleUIManager::LoadAtlas()
{
	if (atlas == nullptr)
	{
		atlas = app->tex->Load("spritesheets/atlas.png");
	}
}


//Remember to aply changes
bool ModuleUIManager::MouseOnUI()
{
	bool ret = false;
	mouseOverUI = false;

	int numGroup = uiGroupVector.size();

	int i;

	if (app->player->doingAction == false)
	{
		for (i = numGroup - 1; i >= 0; i--)
		{
			if (uiGroupVector[i]->OnAbove() == true)
			{
				mouseOverUI = true;
				ret = true;
				break;
			}
		}
	}

	if (mouseOverUI == true)
	{
		i--;

		for (i - 1; i >= 0; i--)
		{
			uiGroupVector[i]->UnFocus();
		}
	}

	return ret;
}


void ModuleUIManager::CheckFocusEntity(Entity* entity)
{

}


void ModuleUIManager::CheckDragElement(UI* element)
{
	if (dragElement == element)
	{
		dragElement = nullptr;
	}
}


bool ModuleUIManager::CheckGroupTag(GROUP_TAG tag)
{
	int uiGroupNumber = uiGroupVector.size();

	for (int i = 0; i < uiGroupNumber; i++)
	{
		if (uiGroupVector[i]->GetTag() == tag)
		{
			return false;
		}
	}

	return true;
}


UI* ModuleUIManager::SearchFocusUI() const
{
	int numGroup = uiGroupVector.size();
	UI* focusUI;

	for (int i = numGroup - 1; i >= 0; i--)
	{
		focusUI = uiGroupVector[i]->SearchFocus();

		if (focusUI != nullptr)
		{
			return focusUI;
		}
	}

	return nullptr;
}


void ModuleUIManager::DragElement()
{

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN)
	{
		dragMouse = app->input->GetMousePosScreen() / app->win->GetUIScale();
		dragElement = SearchFocusUI();
	}

	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT)
	{
		if (dragElement != nullptr)
		{
			if (dragElement->GetDragable() == true)
			{
				iMPoint position = app->input->GetMousePosScreen() / app->win->GetUIScale();

				dragElement->Drag(position.x - dragMouse.x, position.y - dragMouse.y);

				dragMouse = position;
			}
		}
	}

	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_UP)
	{
		dragElement = nullptr;
	}
}


void ModuleUIManager::UnregisterEvents()
{
	app->eventManager->EventUnRegister(EVENT_ENUM::GAME_SCENE_ENTERED, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::CREATE_OPTION_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::CREATE_CREDIT_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::PAUSE_GAME, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::CREATE_INTRO_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MUSIC_ADJUSTMENT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SFX_ADJUSTMENT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ENTITY_DEAD, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::DELETE_OPTIONS_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::DELETE_CREDITS_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::DELETE_PAUSE_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::DELETE_IN_HOVER_MENU, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::FULLSCREEN_INPUT, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::HIDE_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::UNHIDE_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::EXIT_MENUS, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::CREATE_DIALOG_WINDOW, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::CLOSE_DIALOG_WINDOW, this);
}


void ModuleUIManager::ExecuteButton(BUTTON_TAG tag, Button* button)
{
	switch (tag)
	{
	case BUTTON_TAG::NULL_TAG:
		assert(true); //you tried to execute a button that was already deleted or some fucked up shit i dont want to get involved with
		break;

	case BUTTON_TAG::CLOSE_OPTIONS_MENU:
		app->eventManager->GenerateEvent(EVENT_ENUM::DELETE_OPTIONS_MENU, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::CLOSE_CREDITS_MENU:
		app->eventManager->GenerateEvent(EVENT_ENUM::DELETE_CREDITS_MENU, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::NEW_GAME:
		app->eventManager->GenerateEvent(EVENT_ENUM::START_GAME, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::CONTINUE_GAME:
		app->testScene->startFromLoad = true;
		app->fadeToBlack->FadeToBlack(app->mainMenu, app->testScene, 3);
		break;

	case BUTTON_TAG::OPTIONS:
		AddUIGroup(factory->CreateOptionsMenu());
		break;

	case BUTTON_TAG::CREDITS:
		AddUIGroup(factory->CreateCreditsMenu());
		break;

	case BUTTON_TAG::EXIT_GAME:
		app->eventManager->GenerateEvent(EVENT_ENUM::EXIT_GAME, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::FULLSCREEN_ON:
		app->eventManager->GenerateEvent(EVENT_ENUM::FULLSCREEN_INPUT, EVENT_ENUM::NULL_EVENT);
		button->rect = factory->fullscreenOffButton;
		button->SetTag(BUTTON_TAG::FULLSCREEN_OFF);
		break;

	case BUTTON_TAG::FULLSCREEN_OFF:
		app->eventManager->GenerateEvent(EVENT_ENUM::FULLSCREEN_INPUT, EVENT_ENUM::NULL_EVENT);
		button->rect = factory->fullscreenOnButton;
		button->SetTag(BUTTON_TAG::FULLSCREEN_ON);
		break;

	case BUTTON_TAG::HIDE:
		break;

	case BUTTON_TAG::SHOW:
		break;

	case BUTTON_TAG::PAUSE:
		AddUIGroup(factory->CreatePauseMenu());
		app->gamePause = true;
		break;

	case BUTTON_TAG::RESUME:
		app->eventManager->GenerateEvent(EVENT_ENUM::DELETE_PAUSE_MENU, EVENT_ENUM::NULL_EVENT);
		app->eventManager->GenerateEvent(EVENT_ENUM::DELETE_OPTIONS_MENU, EVENT_ENUM::NULL_EVENT);

		app->gamePause = false;
		break;

	case BUTTON_TAG::MAIN_MENU:
		app->eventManager->GenerateEvent(EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::SAVE:
		app->SaveGame();
		break;

	case BUTTON_TAG::LOAD:
		app->testScene->startFromLoad = true;
		app->fadeToBlack->FadeToBlack(app->testScene, app->testScene, 3);
		break;

	case BUTTON_TAG::REVIVE_GATHERER:
		app->eventManager->GenerateEvent(EVENT_ENUM::GATHERER_RESURRECT, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::REVIVE_RANGED:
		app->eventManager->GenerateEvent(EVENT_ENUM::RANGED_RESURRECT, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::REVIVE_MELEE:
		app->eventManager->GenerateEvent(EVENT_ENUM::MELEE_RESURRECT, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::BUY_TURRET:
		app->eventManager->GenerateEvent(EVENT_ENUM::TURRET_CONSTRUCT, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::UPGRADE_TURRET:
		app->eventManager->GenerateEvent(EVENT_ENUM::TURRET_UPGRADED, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::BUY_BARRICADE:
		app->eventManager->GenerateEvent(EVENT_ENUM::BARRICADE_CONSTRUCT, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::UPGRADE_BARRICADE:
		app->eventManager->GenerateEvent(EVENT_ENUM::BARRICADE_UPGRADED, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::GATHERER_LIFE_UPGRADE:
		if (app->player->GetResources() >= factory->gathererLifeUpgradeCost)
		{
			app->player->AddResources(-factory->gathererLifeUpgradeCost);
			AugmentValueByTenPercent(&factory->gathererLifeUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::GATHERER_LIFE_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::GATHERER_DAMAGE_UPGRADE:
		if (app->player->GetResources() >= factory->gathererDamageUpgradeCost)
		{
			app->player->AddResources(-factory->gathererDamageUpgradeCost);
			AugmentValueByTenPercent(&factory->gathererDamageUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::GATHERER_DAMAGE_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::GATHERER_ENERGY_UPGRADE:
		if (app->player->GetResources() >= factory->gathererEnergyUpgradeCost)
		{
			app->player->AddResources(-factory->gathererEnergyUpgradeCost);
			AugmentValueByTenPercent(&factory->gathererEnergyUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::GATHERER_ENERGY_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::GATHERER_ATTACK_SPEED_UPGRADE:
		if (app->player->GetResources() >= factory->gathererAtkSpeedUpgradeCost)
		{
			app->player->AddResources(-factory->gathererAtkSpeedUpgradeCost);
			AugmentValueByTenPercent(&factory->gathererAtkSpeedUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::GATHERER_ATTACK_SPEED_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::MELEE_LIFE_UPGRADE:
		if (app->player->GetResources() >= factory->meleeLifeUpgradeCost)
		{
			app->player->AddResources(-factory->meleeLifeUpgradeCost);
			AugmentValueByTenPercent(&factory->meleeLifeUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::MELEE_LIFE_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::MELEE_DAMAGE_UPGRADE:
		if (app->player->GetResources() >= factory->meleeDamageUpgradeCost)
		{
			app->player->AddResources(-factory->meleeDamageUpgradeCost);
			AugmentValueByTenPercent(&factory->meleeDamageUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::MELEE_DAMAGE_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::MELEE_ENERGY_UPGRADE:
		if (app->player->GetResources() >= factory->meleeEnergyUpgradeCost)
		{
			app->player->AddResources(-factory->meleeEnergyUpgradeCost);
			AugmentValueByTenPercent(&factory->meleeEnergyUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::MELEE_ENERGY_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::MELEE_ATTACK_SPEED_UPGRADE:
		if (app->player->GetResources() >= factory->meleeAtkSpeedUpgradeCost)
		{
			app->player->AddResources(-factory->meleeAtkSpeedUpgradeCost);
			AugmentValueByTenPercent(&factory->meleeAtkSpeedUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::MELEE_ATTACK_SPEED_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::RANGED_LIFE_UPGRADE:
		if (app->player->GetResources() >= factory->rangedLifeUpgradeCost)
		{
			app->player->AddResources(-factory->rangedLifeUpgradeCost);
			AugmentValueByTenPercent(&factory->rangedLifeUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::RANGED_LIFE_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::RANGED_DAMAGE_UPGRADE:
		if (app->player->GetResources() >= factory->rangedDamageUpgradeCost)
		{
			app->player->AddResources(-factory->rangedDamageUpgradeCost);
			AugmentValueByTenPercent(&factory->rangedDamageUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::RANGED_DAMAGE_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::RANGED_ENERGY_UPGRADE:
		if (app->player->GetResources() >= factory->rangedEnergyUpgradeCost)
		{
			app->player->AddResources(-factory->rangedEnergyUpgradeCost);
			AugmentValueByTenPercent(&factory->rangedEnergyUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::RANGED_ENERGY_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::RANGED_ATTACK_SPEED_UPGRADE:
		if (app->player->GetResources() >= factory->rangedAtkSpeedUpgradeCost)
		{
			app->player->AddResources(-factory->rangedAtkSpeedUpgradeCost);
			AugmentValueByTenPercent(&factory->rangedAtkSpeedUpgradeCost);
			app->eventManager->GenerateEvent(EVENT_ENUM::RANGED_ATTACK_SPEED_UPGRADE, EVENT_ENUM::NULL_EVENT);
		}
		break;

	case BUTTON_TAG::GATHERER_PASSIVE1_UPGRADE:
		app->eventManager->GenerateEvent(EVENT_ENUM::GATHERER_PASSIVE1_UPGRADE, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::GATHERER_ACTIVE1_UPGRADE:
		app->eventManager->GenerateEvent(EVENT_ENUM::GATHERER_ACTIVE1_UPGRADE, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::MELEE_PASSIVE1_UPGRADE:
		app->eventManager->GenerateEvent(EVENT_ENUM::MELEE_PASSIVE1_UPGRADE, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::MELEE_ACTIVE1_UPGRADE:
		app->eventManager->GenerateEvent(EVENT_ENUM::MELEE_ACTIVE1_UPGRADE, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::RANGED_PASSIVE1_UPGRADE:
		app->eventManager->GenerateEvent(EVENT_ENUM::RANGED_PASSIVE1_UPGRADE, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::RANGED_ACTIVE1_UPGRADE:
		app->eventManager->GenerateEvent(EVENT_ENUM::RANGED_ACTIVE1_UPGRADE, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::GATHERER_PORTRAIT:
		app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_GATHERER, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::MELEE_PORTRAIT:
		app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_MELEE, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::RANGED_PORTRAIT:
		app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_RANGED, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::ROBO_PORTRAIT:
		app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_ROBO, EVENT_ENUM::NULL_EVENT);
		break;

	case BUTTON_TAG::NEXT_DIALOG:
		app->dialogManager->PushInput(DIALOG_INPUT::NEXT_DIALOG);
		break;

	default:
		assert("you forgot to add the case of the button tag :D"); 
		break;
	}

	PlayClickSound();

}


void ModuleUIManager::ExecuteHoverButton(BUTTON_TAG tag, Button* button)
{

	hoverElement = button;

	switch (tag)
	{

	case BUTTON_TAG::REVIVE_GATHERER:
	case BUTTON_TAG::REVIVE_RANGED:
	case BUTTON_TAG::REVIVE_MELEE:
		AddUIGroup(factory->CreateOnHoverReviveMenu(button));
		break;

	case BUTTON_TAG::BUY_TURRET:
		AddUIGroup(factory->CreateOnHoverBuyTurretMenu());
		break;

	case BUTTON_TAG::UPGRADE_TURRET:
		AddUIGroup(factory->CreateOnHoverUpgradeTurretMenu());
		break;

	case BUTTON_TAG::BUY_BARRICADE:
		AddUIGroup(factory->CreateOnHoverBuyBarricadeMenu());
		break;

	case BUTTON_TAG::UPGRADE_BARRICADE:
		AddUIGroup(factory->CreateOnHoverUpgradeBarricadeMenu());
		break;

	case BUTTON_TAG::GATHERER_LIFE_UPGRADE:
		AddUIGroup(factory->CreateOnHoverGathererLifeUpgradeMenu());
		break;

	case BUTTON_TAG::GATHERER_DAMAGE_UPGRADE:
		AddUIGroup(factory->CreateOnHoverGathererDamageUpgradeMenu());
		break;

	case BUTTON_TAG::GATHERER_ENERGY_UPGRADE:
		AddUIGroup(factory->CreateOnHoverGathererEnergyUpgradeMenu());
		break;

	case BUTTON_TAG::GATHERER_ATTACK_SPEED_UPGRADE:
		AddUIGroup(factory->CreateOnHoverGathererAttackSpeedUpgradeMenu());
		break;

	case BUTTON_TAG::MELEE_LIFE_UPGRADE:
		AddUIGroup(factory->CreateOnHoverMeleeLifeUpgradeMenu());
		break;

	case BUTTON_TAG::MELEE_DAMAGE_UPGRADE:
		AddUIGroup(factory->CreateOnHoverMeleeDamageUpgradeMenu());
		break;

	case BUTTON_TAG::MELEE_ENERGY_UPGRADE:
		AddUIGroup(factory->CreateOnHoverMeleeEnergyUpgradeMenu());
		break;

	case BUTTON_TAG::MELEE_ATTACK_SPEED_UPGRADE:
		AddUIGroup(factory->CreateOnHoverMeleeAttackSpeedUpgradeMenu());
		break;

	case BUTTON_TAG::RANGED_LIFE_UPGRADE:
		AddUIGroup(factory->CreateOnHoverRangedLifeMenuMenu());
		break;

	case BUTTON_TAG::RANGED_DAMAGE_UPGRADE:
		AddUIGroup(factory->CreateOnHoverRangedDamageUpgradeMenu());
		break;

	case BUTTON_TAG::RANGED_ENERGY_UPGRADE:
		AddUIGroup(factory->CreateOnHoverRangedEnergyUpgradeMenu());
		break;

	case BUTTON_TAG::RANGED_ATTACK_SPEED_UPGRADE:
		AddUIGroup(factory->CreateOnHoverRangedAttackSpeedUpgradeMenu());
		break;

	case BUTTON_TAG::GATHERER_PASSIVE1_UPGRADE:
		AddUIGroup(factory->CreateOnHoverGathererPassive1Menu());
		break;

	case BUTTON_TAG::GATHERER_ACTIVE1_UPGRADE:
		AddUIGroup(factory->CreateOnHoverGathererActive1Menu());
		break;

	case BUTTON_TAG::MELEE_PASSIVE1_UPGRADE:
		AddUIGroup(factory->CreateOnHoverMeleePassive1Menu());
		break;

	case BUTTON_TAG::MELEE_ACTIVE1_UPGRADE:
		AddUIGroup(factory->CreateOnHoverMeleeActive1Menu());
		break;

	case BUTTON_TAG::RANGED_PASSIVE1_UPGRADE:
		AddUIGroup(factory->CreateOnHoverRangedPassive1Menu());
		break;

	case BUTTON_TAG::RANGED_ACTIVE1_UPGRADE:
		AddUIGroup(factory->CreateOnHoverRangedActive1Menu());
		break;

	

	default:
		break;

	}

	PlayHoverSound();

}

HeroesPortraitManager* ModuleUIManager::GetPortraitManager()
{
	return portraitManager;
}


void ModuleUIManager::SetPortraitManager(HeroesPortraitManager* manager)
{
	portraitManager = manager;
}


void ModuleUIManager::AddPortrait(Hero* portrait)
{
	portraitsToAdd.push_back(factory->CreatePortrait(portrait));
}


void ModuleUIManager::RemovePortrait(Hero* portrait)
{
	if (portraitManager != nullptr)
	{
		portraitManager->DeletePortrait(portrait);
	}
}


void ModuleUIManager::AddPendingPortraits()
{
	int portraitsNumber = portraitsToAdd.size();

	if (portraitManager != nullptr)
	{
		for (int i = 0; i < portraitsNumber; i++)
		{
			portraitManager->AddPortrait(portraitsToAdd[i]);
		}

		portraitsToAdd.clear();
	}
}


void ModuleUIManager::CheckFocusedUI()
{
	if (hoverElement != nullptr)
	{
		if (hoverElement != SearchFocusUI())
		{
			app->eventManager->GenerateEvent(EVENT_ENUM::DELETE_IN_HOVER_MENU, EVENT_ENUM::NULL_EVENT);
			hoverElement = nullptr;
		}
	}
}


void ModuleUIManager::PlayHoverSound()
{
	app->audio->PlayFx(hoverSound, 0, -1);
}


void ModuleUIManager::PlayClickSound()
{
	app->audio->PlayFx(clickSound, 0, -1);
}


void ModuleUIManager::AugmentValueByTenPercent(float* value)
{
	*value *= 1.1;
}


bool ModuleUIManager::Save(pugi::xml_node& data) const
{
	pugi::xml_node iterator = data.append_child("UIManager");

	iterator.append_attribute("gathererLifeShop") = factory->gathererLifeUpgradeCost;
	iterator.append_attribute("gathererDamageShop") = factory->gathererDamageUpgradeCost;
	iterator.append_attribute("gathererEnergyShop") = factory->gathererEnergyUpgradeCost;
	iterator.append_attribute("gathererAtkSpeedShop") = factory->gathererAtkSpeedUpgradeCost;

	iterator.append_attribute("meleeLifeShop") = factory->meleeLifeUpgradeCost;
	iterator.append_attribute("meleeDamageShop") = factory->meleeDamageUpgradeCost;
	iterator.append_attribute("meleeEnergyShop") = factory->meleeEnergyUpgradeCost;
	iterator.append_attribute("meleeAtkSpeedShop") = factory->meleeAtkSpeedUpgradeCost;

	iterator.append_attribute("rangedLifeShop") = factory->rangedLifeUpgradeCost;
	iterator.append_attribute("rangedDamageShop") = factory->rangedDamageUpgradeCost;
	iterator.append_attribute("rangedEnergyShop") = factory->rangedEnergyUpgradeCost;
	iterator.append_attribute("rangedAtkSpeedShop") = factory->rangedAtkSpeedUpgradeCost;
	
	iterator.append_attribute("robottoLifeShop") = factory->robottoLifeUpgradeCost;
	iterator.append_attribute("robottoDamageShop") = factory->robottoDamageUpgradeCost;
	iterator.append_attribute("robottoEnergyShop") = factory->robottoEnergyUpgradeCost;
	iterator.append_attribute("robottoAtkSpeedShop") = factory->robottoAtkSpeedUpgradeCost;
	
	return true;
}


bool ModuleUIManager::Load(pugi::xml_node& data)
{
	pugi::xml_node iterator = data;

	factory->gathererLifeUpgradeCost = iterator.child("UIManager").attribute("gathererLifeShop").as_float();
	factory->gathererDamageUpgradeCost = iterator.child("UIManager").attribute("gathererDamageShop").as_float();
	factory->gathererEnergyUpgradeCost = iterator.child("UIManager").attribute("gathererEnergyShop").as_float();
	factory->gathererAtkSpeedUpgradeCost = iterator.child("UIManager").attribute("gathererAtkSpeedShop").as_float();

	factory->meleeLifeUpgradeCost = iterator.child("UIManager").attribute("meleeLifeShop").as_float();
	factory->meleeDamageUpgradeCost = iterator.child("UIManager").attribute("meleeDamageShop").as_float();
	factory->meleeEnergyUpgradeCost = iterator.child("UIManager").attribute("meleeEnergyShop").as_float();
	factory->meleeAtkSpeedUpgradeCost = iterator.child("UIManager").attribute("meleeAtkSpeedShop").as_float();

	factory->rangedLifeUpgradeCost = iterator.child("UIManager").attribute("rangedLifeShop").as_float();
	factory->rangedDamageUpgradeCost = iterator.child("UIManager").attribute("rangedDamageShop").as_float();
	factory->rangedEnergyUpgradeCost = iterator.child("UIManager").attribute("rangedEnergyShop").as_float();
	factory->rangedAtkSpeedUpgradeCost = iterator.child("UIManager").attribute("rangedAtkSpeedShop").as_float();

	factory->robottoLifeUpgradeCost = iterator.child("UIManager").attribute("robottoLifeShop").as_float();
	factory->robottoDamageUpgradeCost = iterator.child("UIManager").attribute("robottoDamageShop").as_float();
	factory->robottoEnergyUpgradeCost = iterator.child("UIManager").attribute("robottoEnergyShop").as_float();
	factory->robottoAtkSpeedUpgradeCost = iterator.child("UIManager").attribute("robottoAtkSpeedShop").as_float();

	return true;
}

