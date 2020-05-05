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
#include "TestScene.h"
#include "EventManager.h"
#include "Textures.h"

#include "Minimap.h"
#include "Base.h"


#include "Brofiler/Brofiler/Brofiler.h"

ModuleUIManager::ModuleUIManager() :

	mouseOverUI(false),
	atlas(nullptr), 
	lastShop(nullptr),
	portraitManager(nullptr)
{
	name.create("UIManager");
}


// Destructor
ModuleUIManager::~ModuleUIManager()
{
	delete factory;

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
	app->eventManager->EventRegister(EVENT_ENUM::CREATE_SHOP_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::CREATE_INTRO_MENU, this);
	
	app->eventManager->EventRegister(EVENT_ENUM::PAUSE_GAME, this);


	app->eventManager->EventRegister(EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU, this);

	app->eventManager->EventRegister(EVENT_ENUM::MUSIC_ADJUSTMENT, this);
	app->eventManager->EventRegister(EVENT_ENUM::SFX_ADJUSTMENT, this);
	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_DEAD, this);
	
	app->eventManager->EventRegister(EVENT_ENUM::DELETE_OPTIONS_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::DELETE_CREDITS_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::DELETE_PAUSE_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::DELETE_SHOP_MENU, this);

	app->eventManager->EventRegister(EVENT_ENUM::FULLSCREEN_INPUT, this);


	app->eventManager->EventRegister(EVENT_ENUM::HIDE_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::UNHIDE_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::EXIT_MENUS, this);
	

	return ret;
}


// Called before the first frame
bool ModuleUIManager::Start()
{
	bool ret = true;

	factory = new UIFactory();

	return ret;
}

// Called each loop iteration
bool ModuleUIManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("UI Manager Pre-Update", Profiler::Color::Purple);

	bool ret = true;

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

	bool creditMenuExisted;
	bool optionsMenuExisted;
	bool pauseMenuExisted;
	bool shopMenuExisted;

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

	case EVENT_ENUM::CREATE_SHOP_MENU:			break;

	case EVENT_ENUM::MUSIC_ADJUSTMENT:			break;

	case EVENT_ENUM::SFX_ADJUSTMENT:			break;

	case EVENT_ENUM::ENTITY_DEAD:				break;

	case EVENT_ENUM::HIDE_MENU:					break;

	case EVENT_ENUM::UNHIDE_MENU:				break;

	case EVENT_ENUM::EXIT_MENUS:
		
		//If none of this menus is open and you are in the Game Scene, create pause menu
		if (DeleteUIGroup(GROUP_TAG::CREDITS_MENU) == true)
			break;

		else if (DeleteUIGroup(GROUP_TAG::OPTIONS_MENU) == true)
			break;

		else if (DeleteUIGroup(GROUP_TAG::SHOP_MENU) == true)
			break;

		else if (DeleteUIGroup(GROUP_TAG::PAUSE_MENU) == true)
			break;
		
		else if (app->testScene->IsEnabled() == true)
		{
			app->eventManager->GenerateEvent(EVENT_ENUM::PAUSE_GAME, EVENT_ENUM::NULL_EVENT);
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
		break;


	case EVENT_ENUM::DELETE_SHOP_MENU:
		DeleteUIGroup(GROUP_TAG::SHOP_MENU);
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
	app->eventManager->EventUnRegister(EVENT_ENUM::CREATE_SHOP_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MUSIC_ADJUSTMENT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SFX_ADJUSTMENT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ENTITY_DEAD, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::DELETE_OPTIONS_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::DELETE_CREDITS_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::DELETE_PAUSE_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::DELETE_SHOP_MENU, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::HIDE_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::UNHIDE_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::EXIT_MENUS, this);
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


	case BUTTON_TAG::CLOSE_SHOP_MENU:
		app->eventManager->GenerateEvent(EVENT_ENUM::DELETE_SHOP_MENU, EVENT_ENUM::NULL_EVENT);
		break;


	case BUTTON_TAG::NEW_GAME:
		app->eventManager->GenerateEvent(EVENT_ENUM::START_GAME, EVENT_ENUM::NULL_EVENT);
		break;


	case BUTTON_TAG::CONTINUE_GAME:
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
		break;


	case BUTTON_TAG::RESUME:
		app->eventManager->GenerateEvent(EVENT_ENUM::DELETE_PAUSE_MENU, EVENT_ENUM::NULL_EVENT);
		app->eventManager->GenerateEvent(EVENT_ENUM::DELETE_OPTIONS_MENU, EVENT_ENUM::NULL_EVENT);
		break;


	case BUTTON_TAG::MAIN_MENU:
		app->eventManager->GenerateEvent(EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU, EVENT_ENUM::NULL_EVENT);
		break;


	case BUTTON_TAG::SAVE:
		break;


	case BUTTON_TAG::LOAD:
		break;


	case BUTTON_TAG::SHOP:
		break;


	case BUTTON_TAG::REVIVE_GATHERER:
		break;


	case BUTTON_TAG::REVIVE_RANGED:
		break;


	case BUTTON_TAG::REVIVE_MELEE:
		break;


	case BUTTON_TAG::BUY_TURRET:
		break;


	case BUTTON_TAG::LEVEL_UP_TURRET:
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


	default:
		assert(true); //you forgot to add the case of the button tag :D
		break;
	}
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