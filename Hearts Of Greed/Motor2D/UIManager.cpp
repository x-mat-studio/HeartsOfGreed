#include "App.h"
#include "Textures.h"
#include "UIManager.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Text.h"
#include "UI_Healthbar.h"
#include "UI_Portrait.h"
#include "UI_Scrollbar.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "Audio.h"
#include "Window.h"
#include "Player.h"
#include "Enemy.h"
#include "Minimap.h"
#include "Turret.h"
#include "Base.h"
#include "UI_Healthbar.h"
#include "Base.h"
#include "Brofiler/Brofiler/Brofiler.h"

ModuleUIManager::ModuleUIManager() : atlas(nullptr), focusedEnt(nullptr), focusedPortrait(nullptr), currResources(nullptr), screenResources(0),
lastShop(nullptr), portraitPointer(nullptr), createdInGameMenu(nullptr)
{
	name.create("UIManager");
}


// Destructor
ModuleUIManager::~ModuleUIManager()
{
	app->tex->UnLoad(atlas);
	atlas = nullptr;
	UnregisterEvents();
}


// Called before render is available
bool ModuleUIManager::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("UI Manager Awake", Profiler::Color::MediumPurple);

	bool ret = true;

	app->eventManager->EventRegister(EVENT_ENUM::HERO_MELEE_CREATED, this);
	app->eventManager->EventRegister(EVENT_ENUM::HERO_GATHERER_CREATED, this);
	app->eventManager->EventRegister(EVENT_ENUM::HERO_RANGED_CREATED, this);
	app->eventManager->EventRegister(EVENT_ENUM::HERO_MELEE_OUT, this);
	app->eventManager->EventRegister(EVENT_ENUM::HERO_GATHERER_OUT, this);
	app->eventManager->EventRegister(EVENT_ENUM::HERO_RANGED_OUT, this);
	app->eventManager->EventRegister(EVENT_ENUM::OPTION_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::CREDIT_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::PAUSE_GAME, this);
	app->eventManager->EventRegister(EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_ON_CLICK, this);
	app->eventManager->EventRegister(EVENT_ENUM::CREATE_SHOP, this);
	app->eventManager->EventRegister(EVENT_ENUM::MUSIC_ADJUSTMENT, this);
	app->eventManager->EventRegister(EVENT_ENUM::SFX_ADJUSTMENT, this);
	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_DEAD, this);
	app->eventManager->EventRegister(EVENT_ENUM::DELETE_MENU, this);


	app->eventManager->EventRegister(EVENT_ENUM::HIDE_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::UNHIDE_MENU, this);
	app->eventManager->EventRegister(EVENT_ENUM::EXIT_MENUS, this);


	return ret;
}


// Called before the first frame
bool ModuleUIManager::Start()
{
	bool ret = true;

	hoverSound = app->audio->LoadFx("audio/sfx/Interface/BotonSimple.wav");
	clickSound = app->audio->LoadFx("audio/sfx/Interface/BotonClick.wav");

	return ret;
}

// Called each loop iteration
bool ModuleUIManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("UI Manager Pre-Update", Profiler::Color::Purple);

	bool ret = true;

	CheckListener(this);

	int numEntities = uiVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		uiVector[i]->PreUpdate(dt);
	}

	if (currResources != nullptr)
	{
		int newResources = app->player->GetResources();

		if (screenResources != newResources)
		{
			UpdateResources(newResources);
		}
	}

	return true;
}

// Called each loop iteration
bool ModuleUIManager::Update(float dt)
{
	BROFILER_CATEGORY("UI Manager Update", Profiler::Color::Purple);

	bool ret = true;

	CheckListener(this);

	for (uint i = 0; i < uiVector.size(); i++)
	{
		uiVector[i]->Update(dt);
	}

	return ret;
}

// Called each loop iteration
bool ModuleUIManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("UI Manager Post Update", Profiler::Color::Purple);

	bool ret = true;

	for (uint i = 0; i < uiVector.size(); i++)
	{
		uiVector[i]->PostUpdate(dt);
	}

	if (focusedPortrait != nullptr)
	{
		UpdateFocusPortrait();
	}

	return ret;
}

//// Called before quitting
bool ModuleUIManager::CleanUp()
{
	for (int i = 0; i < uiVector.size(); i++)
	{
		RELEASE(uiVector[i]);
		uiVector[i] = nullptr;
		uiVector.erase(uiVector.begin() + i);
		i--;
	}

	uiVector.clear();

	DeleteUIChilds(focusedPortrait, true);

	portraitPointer = nullptr;
	focusedEnt = nullptr;
	focusedPortrait = nullptr;
	currResources = nullptr;
	createdInGameMenu = nullptr;


	return true;
}

UI* ModuleUIManager::AddUIElement(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, Entity* entity, DRAGGABLE dragable, char* text, SDL_Color color, TTF_Font* font)
{
	UI* newUI = nullptr;

	switch (uiType)
	{
	case UI_TYPE::UI_TEXT:
		newUI = new UI_Text(positionValue, father, uiType, rect, uiName, dragable, text, color, font);
		break;
	case UI_TYPE::UI_IMG:
		newUI = new UI_Image(positionValue, father, uiType, rect, uiName, dragable);
		break;
	case UI_TYPE::UI_HEALTHBAR:
		newUI = new UI_Healthbar(positionValue, father, uiType, rect, uiName, entity, dragable);
		break;
	case UI_TYPE::UI_PORTRAIT:
		portraitPointer = new UI_Portrait(positionValue, father, uiType, rect, uiName, dragable);
		newUI = portraitPointer;
		break;
	}

	if (newUI != nullptr)
	{
		uiVector.push_back(newUI);
	}

	return newUI;
}

UI* ModuleUIManager::AddButton(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, EVENT_ENUM eventR, bool menuClosure, bool includeFather,
	bool hiding, bool hoverMove, DRAGGABLE draggable, EVENT_ENUM eventTrigger, bool interactable)
{
	UI* newUI = new UI_Button(positionValue, father, uiType, rect, uiName, eventR, menuClosure, includeFather, hiding, hoverMove, draggable, eventTrigger, interactable);
	uiVector.push_back(newUI);
	return newUI;
}

UI* ModuleUIManager::AddScrollbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, EVENT_ENUM eventR, int maxValue, EVENT_ENUM eventTrigger, DRAGGABLE draggable)
{
	UI* newUI = new  UI_Scrollbar(positionValue, father, uiType, rect, uiName, draggable, eventR, eventTrigger, maxValue);
	uiVector.push_back(newUI);
	return newUI;
}

void ModuleUIManager::ExecuteEvent(EVENT_ENUM eventId)
{
	int eventCheck = 0;
	UI_Portrait* portrait = nullptr;
	UI_Scrollbar* scrollbar = nullptr;

	switch (eventId)
	{

		// When adding a Hero to these enum, add it to the checking function below
	case EVENT_ENUM::HERO_MELEE_CREATED:
	case EVENT_ENUM::HERO_GATHERER_CREATED:
	case EVENT_ENUM::HERO_RANGED_CREATED:

		if (portraitPointer != nullptr)
		{
			while (eventCheck == 0)
			{
				Hero* hero = app->entityManager->CheckUIAssigned(eventCheck);
				if (hero != nullptr)
				{
					portraitPointer->CreatePortrait(hero);
					portraitPointer->nextVectorPosition += 60;
				}
			}
		}

		break;
	case EVENT_ENUM::HERO_MELEE_OUT:
	case EVENT_ENUM::HERO_GATHERER_OUT:
	case EVENT_ENUM::HERO_RANGED_OUT:
		portrait = (UI_Portrait*)FindUIByName("portraitVector");
		portrait->DeletePortrait();
		portrait = nullptr;
		break;

	case EVENT_ENUM::OPTION_MENU:
		CreateOptionsMenu();
		break;

	case EVENT_ENUM::CREDIT_MENU:
		CreateCreditMenu();
		break;

	case EVENT_ENUM::PAUSE_GAME:
		CreatePauseMenu();
		break;

	case EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU:
		app->eventManager->GenerateEvent(EVENT_ENUM::RETURN_TO_MAIN_MENU, EVENT_ENUM::NULL_EVENT);
		break;

	case EVENT_ENUM::ENTITY_ON_CLICK:
		if(focusedPortrait != nullptr)
		DeleteUIChilds(focusedPortrait, false);
		focusedEnt = nullptr;
		CreateEntityPortrait();
		break;

	case EVENT_ENUM::CREATE_SHOP:
		app->audio->PlayFx(clickSound, 0, -1);
		CreateShopMenu();
		break;

	case EVENT_ENUM::MUSIC_ADJUSTMENT:
		scrollbar = (UI_Scrollbar*)FindUIByName("musicScrollbar");
		eventCheck = scrollbar->GetScrollValue();
		Mix_VolumeMusic(eventCheck);
		app->audio->musicVolume = eventCheck;
		scrollbar = nullptr;
		break;

	case EVENT_ENUM::SFX_ADJUSTMENT:
		scrollbar = (UI_Scrollbar*)FindUIByName("sfxScrollbar");
		app->audio->volumeAdjustment = scrollbar->GetScrollValue() - 255;
		scrollbar = nullptr;
		break;

	case EVENT_ENUM::ENTITY_DEAD:
		DisableHealthBars();
		CheckFocusEntity();
		break;

	case EVENT_ENUM::HIDE_MENU:
		if (createdInGameMenu != nullptr)
		{
			createdInGameMenu->enabled = false;
		}
		break;

	case EVENT_ENUM::UNHIDE_MENU:
		if (createdInGameMenu != nullptr)
		{
			createdInGameMenu->enabled = true;
		}
		break;

	case EVENT_ENUM::EXIT_MENUS:
		if (createdInGameMenu != nullptr)
		{
			if (!app->player->IsBuilding())
			{
				if (createdInGameMenu->parent == createdInGameMenu)
				{
					DeleteUIChilds(createdInGameMenu, true);
					createdInGameMenu = nullptr;
					app->SetPause(false);
				}
				else
				{
					UI* newMenu = createdInGameMenu->parent;
					DeleteUIChilds(createdInGameMenu, true);
					createdInGameMenu = newMenu;
					newMenu = nullptr;
				}
			}
		}
		else if (app->player->IsEnabled())
		{
			CreatePauseMenu();
		}

		break;

	}
}

void ModuleUIManager::CreateBasicInGameUI()
{

	SDL_Rect rect = RectConstructor(0, 0, 0, 0);;
	uint w(app->win->width), h(app->win->height);
	UI* father;
	char resources[10];

	//	  rect = RectConstructor(556, 35, 15, 14);
	//    father = AddButton(fMPoint(w / app->win->GetUIScale() - 87, 35), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"PortraitHideButton", EVENT_ENUM::NULL_EVENT, false, false, true, false);

	AddUIElement(fMPoint(w / app->win->GetUIScale() - 72, 35), nullptr, UI_TYPE::UI_PORTRAIT, rect, (P2SString)"portraitVector", nullptr, DRAGGABLE::DRAG_OFF);
	
	rect = RectConstructor(540, 35, 15, 14);
	father = AddButton(fMPoint(162, h / app->win->GetUIScale() - 85), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"minimapHideButton", EVENT_ENUM::NULL_EVENT, false, false, true, false);

	rect = RectConstructor(221, 317, 162, 174);
	rect.h = app->minimap->height;
	AddUIElement(fMPoint(0, (h - rect.h - 20) / app->win->GetUIScale()), father, UI_TYPE::UI_IMG, rect, (P2SString)"minimapBackground");

	rect = RectConstructor(449, 24, 24, 24);
	AddButton(fMPoint(w / app->win->GetUIScale() - (1.25f) * rect.w, (1.25f) * rect.w - rect.w), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"pauseButton", EVENT_ENUM::PAUSE_GAME, false, false, false, false);

	rect = RectConstructor(415, 435, 65, 30);
	AddUIElement(fMPoint(w / app->win->GetUIScale() - 65, h / app->win->GetUIScale() - 97), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"resourceBackground");

	rect = RectConstructor(18, 209, 11, 19);
	AddUIElement(fMPoint(w / app->win->GetUIScale() - 59, h / app->win->GetUIScale() - 90), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"resourceIcon");

	screenResources = app->player->GetResources();
	sprintf_s(resources, 10, "%d", screenResources);
	currResources = AddUIElement(fMPoint(w / app->win->GetUIScale() - 41, h / app->win->GetUIScale() - 94), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"resourceText", nullptr, DRAGGABLE::DRAG_OFF, resources);

	rect = RectConstructor(400, 435, 139, 67);
	focusedPortrait = AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w, h / app->win->GetUIScale() - rect.h), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"portraitBG");

	rect = RectConstructor(727, 203, 65, 51);
	AddUIElement(fMPoint(w / app->win->GetUIScale() - 2 * rect.w + 12, h / app->win->GetUIScale() - rect.h - 5), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"imgBG");

}

void ModuleUIManager::CreatePauseMenu()
{
	SDL_Rect rect = RectConstructor(15, 271, 194, 231);
	uint w(app->win->width), h(app->win->height);

	UI* father = AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), h / (app->win->GetUIScale() * 2) - (rect.h / 2)), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"pauseMenuBackground");
	createdInGameMenu = father;

	int height = h / (app->win->GetUIScale() * 2) - (rect.h / 2) + 8;

	rect = RectConstructor(17, 12, 195, 36);
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), height), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"resumeButton", EVENT_ENUM::UNPAUSE_GAME, true, true);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), height + 44), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"saveButton", EVENT_ENUM::SAVE_GAME);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), height + 89), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"loadButton", EVENT_ENUM::LOAD_GAME);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), height + 134), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"optionButton", EVENT_ENUM::OPTION_MENU);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), height + 179), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"mainMenuButton", EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU, true, true);

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 32, height + 5), father, UI_TYPE::UI_TEXT, rect, (P2SString)"resumeText", nullptr, DRAGGABLE::DRAG_OFF, "R E S U M E    G A M E");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 43, height + 49), father, UI_TYPE::UI_TEXT, rect, (P2SString)"saveText", nullptr, DRAGGABLE::DRAG_OFF, "S A V E    G A M E");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 43, height + 94), father, UI_TYPE::UI_TEXT, rect, (P2SString)"loadText", nullptr, DRAGGABLE::DRAG_OFF, "L O A D    G A M E");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 58, height + 139), father, UI_TYPE::UI_TEXT, rect, (P2SString)"optionText", nullptr, DRAGGABLE::DRAG_OFF, "O P T I O N S");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 48, height + 184), father, UI_TYPE::UI_TEXT, rect, (P2SString)"mainMenuText", nullptr, DRAGGABLE::DRAG_OFF, "M A I N    M E N U");

	app->SetPause(true);
}

void ModuleUIManager::CreateMainMenu()
{
	SDL_Rect rect = RectConstructor(17, 12, 195, 36);
	uint w(app->win->width), h(app->win->height);

	// TODO Charge background image here that serves as father

//	AddButton(fMPoint(w / app->win->GetUIScale() - rect.w - 20, (h / (app->win->GetUIScale() * 4))), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"continueButton", true, EVENT_ENUM::START_GAME_FROM_CONTINUE, DRAGGABLE::DRAG_OFF);

	AddButton(fMPoint(w / app->win->GetUIScale() - rect.w - 20, (h / (app->win->GetUIScale() * 4)) + 40), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"newGameButton", EVENT_ENUM::START_GAME, true, true);

	AddButton(fMPoint(w / app->win->GetUIScale() - rect.w - 20, (h / (app->win->GetUIScale() * 4)) + 80), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"optionsButton", EVENT_ENUM::OPTION_MENU);

	AddButton(fMPoint(w / app->win->GetUIScale() - rect.w - 20, (h / (app->win->GetUIScale() * 4)) + 120), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"creditsButton", EVENT_ENUM::CREDIT_MENU);

	AddButton(fMPoint(w / app->win->GetUIScale() - rect.w - 20, (h / (app->win->GetUIScale() * 4)) + 160), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"exitGameButton", EVENT_ENUM::EXIT_GAME, true, true);

	//	AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w + 5, (h / (app->win->GetUIScale() * 4)) + 5), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"continueText", DRAGGABLE::DRAG_OFF, "C O N T I N U E    G A M E");

	AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w + 35, (h / (app->win->GetUIScale() * 4)) + 45), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"newGameText", nullptr, DRAGGABLE::DRAG_OFF, "N E W    G A M E");

	AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w + 40, (h / (app->win->GetUIScale() * 4)) + 85), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"optionsText", nullptr, DRAGGABLE::DRAG_OFF, "O P T I O N S");

	AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w + 42, (h / (app->win->GetUIScale() * 4)) + 125), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"creditsText", nullptr, DRAGGABLE::DRAG_OFF, "C R E D I T S");

	AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w + 30, (h / (app->win->GetUIScale() * 4)) + 165), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"exitGameText", nullptr, DRAGGABLE::DRAG_OFF, "E X I T    G A M E");

}

void ModuleUIManager::CreateOptionsMenu()
{

	SDL_Rect rect = RectConstructor(677, 369, 278, 153);
	uint w(app->win->width), h(app->win->height);
	UI* father = nullptr;		// TODO: make event and functionality happen		Also, change the button ON / OFF image depending on fullscreen mode

	createdInGameMenu = father = AddUIElement(fMPoint((w / app->win->GetUIScale() / 2) - (rect.w / 2), (h / app->win->GetUIScale() / 2) - (rect.h / 2)), createdInGameMenu, UI_TYPE::UI_IMG, rect, (P2SString)"optionBackground");

	AddUIElement(fMPoint((w / app->win->GetUIScale() / 2) - (rect.w / 2) + 30, (h / app->win->GetUIScale() / 2) - (rect.h / 2)), father, UI_TYPE::UI_TEXT, rect, (P2SString)"optionText", nullptr, DRAGGABLE::DRAG_OFF, "O P T I O N S");

	AddUIElement(fMPoint((w / app->win->GetUIScale() / 2) - (rect.w / 2) + 20, (h / app->win->GetUIScale() / 2) - (rect.h / 2) + 25), father, UI_TYPE::UI_TEXT, rect, (P2SString)"musicText", nullptr, DRAGGABLE::DRAG_OFF, "Music");

	AddUIElement(fMPoint((w / app->win->GetUIScale() / 2) - (rect.w / 2) + 140, (h / app->win->GetUIScale() / 2) - (rect.h / 2) + 25), father, UI_TYPE::UI_TEXT, rect, (P2SString)"sfxText", nullptr, DRAGGABLE::DRAG_OFF, "SFX");

	AddUIElement(fMPoint((w / app->win->GetUIScale() / 2) - (rect.w / 2) + 20, (h / app->win->GetUIScale() / 2) - (rect.h / 2) + 75), father, UI_TYPE::UI_TEXT, rect, (P2SString)"fullscreenModeText", nullptr, DRAGGABLE::DRAG_OFF, "Fullscreen mode");

	rect = RectConstructor(739, 27, 23, 23);
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (278 / 2) + 20, h / (app->win->GetUIScale() * 2) - (153 / 2) + 100), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"fullscreenButton", EVENT_ENUM::FULLSCREEN_INPUT, false, false, false, false);

	rect = RectConstructor(424, 25, 23, 23);
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) + (278 / 2) - (3 * rect.w / 4), h / (app->win->GetUIScale() * 2) - (153 / 2) - (1 * rect.h / 4)), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"closeButton", EVENT_ENUM::NULL_EVENT, true, true, false, false);

	rect = RectConstructor(273, 45, 90, 4);
	AddScrollbar(fMPoint((w / app->win->GetUIScale() / 2) - (278 / 2) + 20, (h / app->win->GetUIScale() / 2) - (153 / 2) + 60), father, UI_TYPE::UI_SCROLLBAR, rect, (P2SString)"musicScrollbar", EVENT_ENUM::MUSIC_ADJUSTMENT, 128.0f);

	AddScrollbar(fMPoint((w / app->win->GetUIScale() / 2) - (278 / 2) + 140, (h / app->win->GetUIScale() / 2) - (153 / 2) + 60), father, UI_TYPE::UI_SCROLLBAR, rect, (P2SString)"sfxScrollbar", EVENT_ENUM::SFX_ADJUSTMENT, 455.0f);

}

void ModuleUIManager::CreateCreditMenu()
{
	//TODO ADRI: FLESH OUT

	SDL_Rect rect = RectConstructor(15, 271, 194, 231);
	uint w(app->win->width), h(app->win->height);
	uint originX = w / (app->win->GetUIScale() * 2) - (rect.w / 2); uint originY = h / (app->win->GetUIScale() * 2) - (rect.h / 2);

	UI* father = AddUIElement(fMPoint(originX, originY), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"pauseMenuBackground");

	rect = RectConstructor(424, 25, 23, 23);
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) + (194 / 2) - (3 * rect.w / 4), h / (app->win->GetUIScale() * 2) - (231 / 2) - (1 * rect.h / 4)), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"closeButton", EVENT_ENUM::NULL_EVENT, true, true, false, false);


	//logo
	rect = RectConstructor(563, 237, 117, 122);
	AddUIElement(fMPoint(originX + 40, originY + 50), father, UI_TYPE::UI_IMG, rect, (P2SString)"logocredit");

	//names

	AddUIElement(fMPoint(originX + 5, originY + 0), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits1", nullptr, DRAGGABLE::DRAG_OFF, "Aaron Guerrero Cruz");
	AddUIElement(fMPoint(originX + 15, originY + 10), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits1.2", nullptr, DRAGGABLE::DRAG_OFF, "Lead");

	AddUIElement(fMPoint(originX + 5, originY + 35), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits2", nullptr, DRAGGABLE::DRAG_OFF, "Jose Luis Redondo Tello");
	AddUIElement(fMPoint(originX + 15, originY + 45), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits2.2", nullptr, DRAGGABLE::DRAG_OFF, "Code");

	AddUIElement(fMPoint(originX + 5, originY + 70), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits3", nullptr, DRAGGABLE::DRAG_OFF, "Ferran-Roger Basart i Bosch");
	AddUIElement(fMPoint(originX + 15, originY + 80), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits3.2", nullptr, DRAGGABLE::DRAG_OFF, "Management + UI");

	AddUIElement(fMPoint(originX + 5, originY + 105), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits4", nullptr, DRAGGABLE::DRAG_OFF, "Alex Melenchon Maza");
	AddUIElement(fMPoint(originX + 15, originY + 115), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits4.2", nullptr, DRAGGABLE::DRAG_OFF, "Design");

	AddUIElement(fMPoint(originX + 5, originY + 140), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits5", nullptr, DRAGGABLE::DRAG_OFF, "Adria Serrano Lopez");
	AddUIElement(fMPoint(originX + 15, originY + 150), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits5.2", nullptr, DRAGGABLE::DRAG_OFF, "Audio + Art");

	AddUIElement(fMPoint(originX + 5, originY + 175), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits6", nullptr, DRAGGABLE::DRAG_OFF, "Oscar Perez Martin");
	AddUIElement(fMPoint(originX + 15, originY + 185), father, UI_TYPE::UI_TEXT, rect, (P2SString)"credits6.2", nullptr, DRAGGABLE::DRAG_OFF, "QA");
}

void ModuleUIManager::CreateEntityPortrait()
{
	if (portraitPointer == nullptr)
		return;

	focusedEnt = app->player->GetFocusedEntity();


	if (focusedEnt != nullptr)
		CreateEntityPortraitChilds();
	else
	{
		focusedEnt = nullptr;
		DeleteUIChilds(focusedPortrait, false);
	}

}

void ModuleUIManager::CreateEntityPortraitChilds()
{
	BROFILER_CATEGORY("Create UI Childs", Profiler::Color::Green);

	uint w(app->win->width / app->win->GetUIScale()), h(app->win->height / app->win->GetUIScale());
	SDL_Color std{ (255),(255), (255), (255) };
	SDL_Rect rect = { 0, 0, 100, 100 };
	static char stats[40];

	Hero* hero = nullptr;

	switch (focusedEnt->GetType())
	{
	case ENTITY_TYPE::BLDG_BASE:
	{
		Base* base = (Base*)focusedEnt;

		//img portrait
		rect = RectConstructor(634, 78, 68, 62);
		AddUIElement(fMPoint(w - 2 * rect.w + 15, h - rect.h - 6), focusedPortrait, UI_TYPE::UI_IMG, rect, (P2SString)"enemyImg");

		//hp bar
		rect = RectConstructor(312, 85, 60, 7);
		AddUIElement(fMPoint(w - 60, (h - 60)), focusedPortrait, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"HPbar", base, DRAGGABLE::DRAG_OFF, "HPbar");

		//stats
		sprintf_s(stats, 40, "HP: %i", base->GetHP());
		AddUIElement(fMPoint(w - 60, (h - 55)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"HP", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rsrc: %i", base->GetRsrc());
		AddUIElement(fMPoint(w - 60, (h - 45)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"Rsrc", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		if (base->GetAlignment() == ENTITY_ALIGNEMENT::PLAYER)
		{
			rect = { 480, 62, 33, 33 };
			AddButton(fMPoint(w - rect.w - 5, (h)-35), focusedPortrait, UI_TYPE::UI_BUTTON, rect, (P2SString)"S H O P", EVENT_ENUM::CREATE_SHOP, false, false, false, false);
			lastShop = base;
		}
	}
	break;

	case ENTITY_TYPE::BLDG_TURRET:
		Turret* turret;
		turret = (Turret*)focusedEnt;

		//img portrait
		rect = { 561, 77, 68, 62 };
		AddUIElement(fMPoint(w - 2 * rect.w + 10, h - rect.h - 1), focusedPortrait, UI_TYPE::UI_IMG, rect, (P2SString)"enemyImg");

		//hp bar
		rect = RectConstructor(312, 85, 60, 7);
		AddUIElement(fMPoint(w - 62, (h - 64)), focusedPortrait, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"HPbar", turret, DRAGGABLE::DRAG_OFF, "HPbar");

		//stats

		sprintf_s(stats, 40, "LVL: %i", turret->GetLvl());
		AddUIElement(fMPoint(w - 45, (h - 15)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"lvl", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AD: %i", turret->GetAD());
		AddUIElement(fMPoint(w - 45, (h - 30)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"AD", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rng: %i", turret->GetRng());
		AddUIElement(fMPoint(w - 45, (h - 45)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"Rng", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AS: %i", turret->GetAS());
		AddUIElement(fMPoint(w - 45, (h - 60)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"AS", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		break;


	case ENTITY_TYPE::HERO_GATHERER:


		hero = (Hero*)focusedEnt;

		//img portrait
		rect = RectConstructor(352, 149, 66, 51);
		AddUIElement(fMPoint(w - 2 * rect.w + 10, h - rect.h - 2), focusedPortrait, UI_TYPE::UI_IMG, rect, "");

		//health bar
		rect = RectConstructor(312, 85, 60, 7);
		AddUIElement(fMPoint(w - 60, (h - 60)), focusedPortrait, UI_TYPE::UI_HEALTHBAR, rect, "", hero, DRAGGABLE::DRAG_OFF, "HPbar");

		rect = RectConstructor(374, 85, 60, 7);
		AddUIElement(fMPoint(w - 60, (h - 50)), focusedPortrait, UI_TYPE::UI_HEALTHBAR, rect, "Ebar", hero, DRAGGABLE::DRAG_OFF, "Ebar");


		//stats
		sprintf_s(stats, 40, "HP: %i", hero->hitPointsCurrent);
		AddUIElement(fMPoint(w - 60, (h - 46)), focusedPortrait, UI_TYPE::UI_TEXT, rect, "", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "E: %i", hero->energyPoints);
		AddUIElement(fMPoint(w - 30, (h - 46)), focusedPortrait, UI_TYPE::UI_TEXT, rect, "", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AD: %i", hero->attackDamage);
		AddUIElement(fMPoint(w - 60, (h - 36)), focusedPortrait, UI_TYPE::UI_TEXT, rect, "", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AS: %f", hero->attackSpeed);
		AddUIElement(fMPoint(w - 30, (h - 36)), focusedPortrait, UI_TYPE::UI_TEXT, rect, "", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rng: %i", hero->attackRange);
		AddUIElement(fMPoint(w - 60, (h - 26)), focusedPortrait, UI_TYPE::UI_TEXT, rect, "", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rec: %i", hero->recoveryHitPointsRate);
		AddUIElement(fMPoint(w - 30, (h - 26)), focusedPortrait, UI_TYPE::UI_TEXT, rect, "", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "EXP: %i / %i", hero->heroXP, hero->expToLevelUp);
		AddUIElement(fMPoint(w - 60, (h - 16)), focusedPortrait, UI_TYPE::UI_TEXT, rect, "", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		break;

	case ENTITY_TYPE::HERO_RANGED:
		//This does not exist in vertical slice
		break;

	case ENTITY_TYPE::HERO_MELEE:

		hero = (Hero*)focusedEnt;

		//img portrait
		rect = RectConstructor(562, 149, 66, 51);
		AddUIElement(fMPoint(w - 2 * rect.w + 10, h - rect.h - 2), focusedPortrait, UI_TYPE::UI_IMG, rect, (P2SString)"heroImg");

		//health bar
		rect = RectConstructor(312, 85, 60, 7);
		AddUIElement(fMPoint(w - 60, (h - 60)), focusedPortrait, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"HPbar", hero, DRAGGABLE::DRAG_OFF, "HPbar");

		rect = RectConstructor(374, 85, 60, 7);
		AddUIElement(fMPoint(w - 60, (h - 50)), focusedPortrait, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"Ebar", hero, DRAGGABLE::DRAG_OFF, "Ebar");


		//stats
		sprintf_s(stats, 40, "HP: %i", hero->hitPointsCurrent);
		AddUIElement(fMPoint(w - 60, (h - 46)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"HP", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "E: %i", hero->energyPoints);
		AddUIElement(fMPoint(w - 30, (h - 46)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"E", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AD: %i", hero->attackDamage);
		AddUIElement(fMPoint(w - 60, (h - 36)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"AD", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AS: %f", hero->attackSpeed);
		AddUIElement(fMPoint(w - 30, (h - 36)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"AS", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rng: %i", hero->attackRange);
		AddUIElement(fMPoint(w - 60, (h - 26)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"Rng", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rec: %i", hero->recoveryHitPointsRate);
		AddUIElement(fMPoint(w - 30, (h - 26)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"Rec", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "EXP: %i / %i", hero->heroXP, hero->expToLevelUp);
		AddUIElement(fMPoint(w - 60, (h - 16)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"Exp", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		break;

	case ENTITY_TYPE::ENEMY:

		Enemy* enemy;
		enemy = (Enemy*)focusedEnt;

		//img portrait
		rect = RectConstructor(894, 116, 70, 79);
		AddUIElement(fMPoint(w - 2 * rect.w + 30, h - rect.h - 10), focusedPortrait, UI_TYPE::UI_IMG, rect, (P2SString)"enemyImg");

		//health bar
		rect = RectConstructor(219, 83, 122, 16);
		AddUIElement(fMPoint(w - 90, (h - 60)), focusedPortrait, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"HPbar", enemy, DRAGGABLE::DRAG_OFF, "HPbar");

		//stats
		sprintf_s(stats, 40, "HP: %i", enemy->GetHP());
		AddUIElement(fMPoint(w - 60, (h - 55)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"HP", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AD: %i", enemy->GetAD());
		AddUIElement(fMPoint(w - 60, (h - 40)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"AD", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AS: %i", enemy->GetAS());
		AddUIElement(fMPoint(w - 25, (h - 40)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"AS", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rng: %i", enemy->GetVision());
		AddUIElement(fMPoint(w - 60, (h - 25)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"Rng", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rec: %i", enemy->GetRecov());
		AddUIElement(fMPoint(w - 25, (h - 25)), focusedPortrait, UI_TYPE::UI_TEXT, rect, (P2SString)"Rec", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		break;

	default:
		break;
	}

}

void ModuleUIManager::CreateShopMenu()
{
	SDL_Rect rect = RectConstructor(15, 271, 194, 231);
	uint w(app->win->width), h(app->win->height);
	static char cost[40];

	UI* father = AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), h / (app->win->GetUIScale() * 2) - (rect.h / 2)), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"shopBackground");
	createdInGameMenu = father;
	// Heroes
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 3, h / (app->win->GetUIScale() * 2) - (rect.h / 2) + 5), father, UI_TYPE::UI_TEXT, rect, (P2SString)"heroResurrectionText", nullptr, DRAGGABLE::DRAG_OFF, "H E R O   R E S U R R E C T I O N");

	rect = RectConstructor(581, 24, 36, 27);
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 30, h / (app->win->GetUIScale() * 2) - (231 / 2) + 35), father, UI_TYPE::UI_IMG, rect, (P2SString)"heroGathererPortrait");

	rect.x = 619;
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 80, h / (app->win->GetUIScale() * 2) - (231 / 2) + 35), father, UI_TYPE::UI_IMG, rect, (P2SString)"heroRangedPortrait");

	rect.x = 658;
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 130, h / (app->win->GetUIScale() * 2) - (231 / 2) + 35), father, UI_TYPE::UI_IMG, rect, (P2SString)"heroMeleePortrait");

	rect = RectConstructor(653, 54, 46, 14);	// TODO Actually read the event of resurrecting in the player / entity manager; also spend the resource (do it only if you have enough)
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 25, h / (app->win->GetUIScale() * 2) - (231 / 2) + 65), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"heroGathererResurrectButton", EVENT_ENUM::GATHERER_RESURRECT, false, false, false, false, DRAGGABLE::DRAG_OFF, EVENT_ENUM::NULL_EVENT, false);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 75, h / (app->win->GetUIScale() * 2) - (231 / 2) + 65), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"heroRangedResurrectButton", EVENT_ENUM::RANGED_RESURRECT, false, false, false, false, DRAGGABLE::DRAG_OFF, EVENT_ENUM::NULL_EVENT, false);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 125, h / (app->win->GetUIScale() * 2) - (231 / 2) + 65), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"heroMeleeResurrectButton", EVENT_ENUM::MELEE_RESURRECT, false, false, false, false, DRAGGABLE::DRAG_OFF, EVENT_ENUM::NULL_EVENT, false);

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 28, h / (app->win->GetUIScale() * 2) - (231 / 2) + 57), father, UI_TYPE::UI_TEXT, rect, (P2SString)"heroGathererResurrectText", nullptr, DRAGGABLE::DRAG_OFF, "Revive");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 78, h / (app->win->GetUIScale() * 2) - (231 / 2) + 57), father, UI_TYPE::UI_TEXT, rect, (P2SString)"heroRangedResurrectText", nullptr, DRAGGABLE::DRAG_OFF, "Revive");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 128, h / (app->win->GetUIScale() * 2) - (231 / 2) + 57), father, UI_TYPE::UI_TEXT, rect, (P2SString)"heroMeleeResurrectText", nullptr, DRAGGABLE::DRAG_OFF, "Revive");

	// Turrets
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 10, h / (app->win->GetUIScale() * 2) - (231 / 2) + 85), father, UI_TYPE::UI_TEXT, rect, (P2SString)"turretBuildingText", nullptr, DRAGGABLE::DRAG_OFF, "T U R R E T   B U I L D I N G");

	rect = RectConstructor(696, 12, 34, 40);
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 120, h / (app->win->GetUIScale() * 2) - (231 / 2) + 135), father, UI_TYPE::UI_IMG, rect, (P2SString)"turretPortrait");

	rect = RectConstructor(653, 54, 46, 14);	// TODO ONLY SPEND RESOURCES IF YOU HAVE THEM
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 40, h / (app->win->GetUIScale() * 2) - (231 / 2) + 120), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"turretPurchaseButton", EVENT_ENUM::TURRET_CONSTRUCT, false, false, false, false);

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 50, h / (app->win->GetUIScale() * 2) - (231 / 2) + 112), father, UI_TYPE::UI_TEXT, rect, (P2SString)"turretPurchaseText", nullptr, DRAGGABLE::DRAG_OFF, "Buy");

	if (lastShop != nullptr)
	{
		sprintf_s(cost, 40, "Max. %i",lastShop->GetmaxTurrets());
		AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 95, h / (app->win->GetUIScale() * 2) - (231 / 2) + 112), father, UI_TYPE::UI_TEXT, rect, (P2SString)"turretPurchaseText", nullptr, DRAGGABLE::DRAG_OFF, cost);
	}
	// TODO: read the actual amount of resources that turret prize costs when the variable is added				// It'd be cool if text got gray if the option was not usable (maybe add a variable to text constructor that is a condition, not a bool, since it may be dynamic, like resources)
	sprintf_s(cost, 40, "- %i", app->player->GetTurretCost());
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 45, h / (app->win->GetUIScale() * 2) - (231 / 2) + 130), father, UI_TYPE::UI_TEXT, rect, (P2SString)"turretPriceText", nullptr, DRAGGABLE::DRAG_OFF, cost);

	rect = RectConstructor(653, 54, 46, 14);	// TODO Actually read the event of enabling the turret building mode; also spend the resource (do it only if you have enough)
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 40, h / (app->win->GetUIScale() * 2) - (231 / 2) + 170), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"turretLevelButton", EVENT_ENUM::TURRET_UPGRADED, false, false, false, false, DRAGGABLE::DRAG_OFF, EVENT_ENUM::NULL_EVENT, false);

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 45, h / (app->win->GetUIScale() * 2) - (231 / 2) + 162), father, UI_TYPE::UI_TEXT, rect, (P2SString)"turretLevelText", nullptr, DRAGGABLE::DRAG_OFF, "Lvl up");

	rect = RectConstructor(424, 25, 23, 23);
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) + (194 / 2) - (rect.w / 2), h / (app->win->GetUIScale() * 2) - (231 / 2) - (rect.h / 2)), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"closeButton", EVENT_ENUM::NULL_EVENT, true, true, false, false);

}

SDL_Texture* ModuleUIManager::GetAtlasTexture() const
{
	return atlas;
}

SDL_Rect ModuleUIManager::RectConstructor(int x, int y, int w, int h)
{
	return { x,y,w,h };
}

void ModuleUIManager::LoadAtlas()
{
	if (atlas == nullptr)
	{
		atlas = app->tex->Load("spritesheets/atlas.png");
	}
}

UI* ModuleUIManager::FindUIByName(char* name)
{
	int numEntities = uiVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		if (uiVector[i]->name == name)
		{
			return uiVector[i];
		}
	}

	return nullptr;
}

UI* ModuleUIManager::FindbyParent(UI* parent)
{
	int numEntities = uiVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		if (uiVector[i]->parent == parent)
		{
			return uiVector[i];
		}
	}

	return nullptr;
}



void ModuleUIManager::HideElements(UI* father, float dt)
{

	int numEntities = uiVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		if (uiVector[i]->parent == father)
		{
			uiVector[i]->hiding_unhiding = true;
		}
	}

}

void ModuleUIManager::DeleteUIChilds(UI* father, bool includeFather)
{
	BROFILER_CATEGORY("Delete UI Childs", Profiler::Color::Green);

	if (father == nullptr)
		return;

	int parentId = -1;

	for (int i = 0; i < uiVector.size(); i++)
	{
		if (uiVector[i]->parent == father)
		{
			if(includeFather)
			app->uiManager->DeleteUIChilds(uiVector[i], false);


			RELEASE(uiVector[i]);
			uiVector[i] = nullptr;

			uiVector.erase(uiVector.begin() + i);
			i--;
		}
		else if (uiVector[i] == father && includeFather == true)
		{
			parentId = i;
		}
	}

	if (parentId != -1)
	{
		if (createdInGameMenu == uiVector[parentId])
			createdInGameMenu = createdInGameMenu->parent;

		RELEASE(uiVector[parentId]);
		uiVector[parentId] = nullptr;
		uiVector.erase(uiVector.begin() + parentId);
	}
}


void ModuleUIManager::StopAll(UI* element, bool reposition, bool hidden, bool hidden_unhiding)
{
	int numEntities = uiVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		if (element->parent != nullptr)
		{
			if (uiVector[i]->parent == element->parent || uiVector[i] == element->parent || uiVector[i] == element->parent->parent || uiVector[i]->parent == element)
			{
				uiVector[i]->hidden = hidden;
				uiVector[i]->hiding_unhiding = hidden_unhiding;
				if (reposition == true)
				{
					uiVector[i]->worldPosition.x = uiVector[i]->defaultPosition;
				}
			}
		}

		else
		{
			if (uiVector[i]->parent == element || (uiVector[i]->parent != nullptr && uiVector[i]->parent->parent == element))
			{
				uiVector[i]->hidden = hidden;
				uiVector[i]->hiding_unhiding = hidden_unhiding;
				if (reposition == true)
				{
					uiVector[i]->worldPosition.x = uiVector[i]->defaultPosition;
				}
			}
		}

		if (uiVector[i]->type == UI_TYPE::UI_PORTRAIT)
		{

			UI_Portrait* portrait = (UI_Portrait*)uiVector[i];

			int numElem = portrait->portraitVector.size();

			for (int i = 0; i < numElem; i++)
			{
				portrait->portraitVector[i].background->hidden = hidden;
				portrait->portraitVector[i].background->hiding_unhiding = hidden_unhiding;
				if (reposition == true)
				{
					portrait->portraitVector[i].background->worldPosition.x = portrait->portraitVector[i].background->defaultPosition;
				}
				portrait->portraitVector[i].backgroundLevel->hidden = hidden;
				portrait->portraitVector[i].backgroundLevel->hiding_unhiding = hidden_unhiding;
				if (reposition == true)
				{
					portrait->portraitVector[i].backgroundLevel->worldPosition.x = portrait->portraitVector[i].backgroundLevel->defaultPosition;
				}
				portrait->portraitVector[i].backgroundHealthbar->hidden = hidden;
				portrait->portraitVector[i].backgroundHealthbar->hiding_unhiding = hidden_unhiding;
				if (reposition == true)
				{
					portrait->portraitVector[i].backgroundHealthbar->worldPosition.x = portrait->portraitVector[i].backgroundHealthbar->defaultPosition;
				}
				portrait->portraitVector[i].healthbar->hidden = hidden;
				portrait->portraitVector[i].healthbar->hiding_unhiding = hidden_unhiding;
				if (reposition == true)
				{
					portrait->portraitVector[i].healthbar->worldPosition.x = portrait->portraitVector[i].healthbar->defaultPosition;
				}
				portrait->portraitVector[i].portrait->hidden = hidden;
				portrait->portraitVector[i].portrait->hiding_unhiding = hidden_unhiding;
				if (reposition == true)
				{
					portrait->portraitVector[i].portrait->worldPosition.x = portrait->portraitVector[i].portrait->defaultPosition;
				}
				portrait->portraitVector[i].level->hidden = hidden;
				portrait->portraitVector[i].level->hiding_unhiding = hidden_unhiding;
				if (reposition == true)
				{
					portrait->portraitVector[i].level->worldPosition.x = portrait->portraitVector[i].level->defaultPosition;
				}
			}
		}
	}
}

void ModuleUIManager::DisableHealthBars()
{
	int numEntities = uiVector.size();
	UI_Healthbar* currHB = nullptr;

	for (int i = 0; i < numEntities; i++)
	{
		if (uiVector[i]->type == UI_TYPE::UI_HEALTHBAR)
		{
			currHB = (UI_Healthbar*)uiVector[i];

			if (currHB != nullptr)
				currHB->EntityDeath();
		}
	}
}

void ModuleUIManager::CheckFocusEntity()
{
	if (focusedEnt != nullptr && focusedEnt->toDelete == true)
	{
		DeleteUIChilds(focusedPortrait, false);
		focusedEnt = nullptr;
	}
}

bool ModuleUIManager::MouseOnUI(iMPoint& mouse)
{
	int numEntities = uiVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		if (uiVector[i]->parent == nullptr && uiVector[i]->enabled)
		{
			if (uiVector[i]->worldPosition.x * app->win->GetUIScale() <= mouse.x && (uiVector[i]->worldPosition.x + uiVector[i]->box.w) * app->win->GetUIScale() >= mouse.x &&
				uiVector[i]->worldPosition.y * app->win->GetUIScale() <= mouse.y && (uiVector[i]->worldPosition.y + uiVector[i]->box.h) * app->win->GetUIScale() >= mouse.y)
			{
				return true;
			}
		}
	}

	return false;
}

void ModuleUIManager::UpdateFocusPortrait()
{
	BROFILER_CATEGORY("Update Focus Portrait", Profiler::Color::Green);

	DeleteUIChilds(focusedPortrait, false);

	focusedEnt = app->player->GetFocusedEntity();

	if (focusedEnt != nullptr )
	{
		if(focusedEnt->toDelete == false)
		CreateEntityPortraitChilds();
	}

}

void ModuleUIManager::UpdateResources(int newResources)
{
	this->screenResources = newResources;
	char bufferText[10];

	sprintf_s(bufferText, 10, "%d", newResources);

	UI_Text* updateResources = (UI_Text*)currResources;
	updateResources->LoadNewTexture(bufferText, nullptr);

}

void ModuleUIManager::UnregisterEvents()
{
	app->eventManager->EventUnRegister(EVENT_ENUM::HERO_MELEE_CREATED, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::HERO_GATHERER_CREATED, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::HERO_RANGED_CREATED, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::HERO_MELEE_OUT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::HERO_GATHERER_OUT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::HERO_RANGED_OUT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::OPTION_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::CREDIT_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::PAUSE_GAME, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ENTITY_ON_CLICK, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::CREATE_SHOP, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MUSIC_ADJUSTMENT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SFX_ADJUSTMENT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ENTITY_DEAD, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::DELETE_MENU, this);


	app->eventManager->EventUnRegister(EVENT_ENUM::HIDE_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::UNHIDE_MENU, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::EXIT_MENUS, this);
}