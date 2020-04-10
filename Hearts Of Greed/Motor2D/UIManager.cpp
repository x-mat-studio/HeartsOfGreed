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
#include "Brofiler/Brofiler/Brofiler.h"


ModuleUIManager::ModuleUIManager()
{
	name.create("UIManager");
}


// Destructor
ModuleUIManager::~ModuleUIManager()
{
	
	app->tex->UnLoad(atlas);
	atlas = nullptr;

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
	app->eventManager->EventRegister(EVENT_ENUM::PAUSE_GAME, this);
	app->eventManager->EventRegister(EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU, this);


	return ret;
}


// Called before the first frame
bool ModuleUIManager::Start()
{
	bool ret = true;

	LoadAtlas();

	hoverSound = app->audio->LoadFx("audio/sfx/Interface/BotonSimple.wav");
	clickSound = app->audio->LoadFx("audio/sfx/Interface/BotonClick.wav");

	return ret;
}

// Called each loop iteration
bool ModuleUIManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("UI Manager Pre-Update", Profiler::Color::Purple)

		bool ret = true;
	
	CheckListener(this);

	int numEntities = uiVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		uiVector[i]->PreUpdate(dt);
	}

	return true;
}

// Called each loop iteration
bool ModuleUIManager::Update(float dt)
{
	BROFILER_CATEGORY("UI Manager Update", Profiler::Color::Purple)

		bool ret = true;

	int numEntities = uiVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		uiVector[i]->Update(dt);
	}

	return ret;
}

// Called each loop iteration
bool ModuleUIManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("UI Manager Post Update", Profiler::Color::Purple)

		bool ret = true;

	int numEntities = uiVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		uiVector[i]->PostUpdate(dt);
	}

	return ret;
}

//// Called before quitting
bool ModuleUIManager::CleanUp()
{
	int numElements = uiVector.size();

	for (int i = 0; i < numElements; i++)
	{
		RELEASE(uiVector[i]);
		uiVector[i] = nullptr;
	}

	uiVector.clear();

	portraitPointer = nullptr;

	return true;
}

UI* ModuleUIManager::AddUIElement(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE dragable, char* text, SDL_Color color)
{
	UI* newUI = nullptr;

	switch (uiType)
	{
	case UI_TYPE::UI_TEXT:
		newUI = new UI_Text(positionValue, father, uiType, rect, uiName, dragable, text, color);
		break;
	case UI_TYPE::UI_IMG:
		newUI = new UI_Image(positionValue, father, uiType, rect, uiName, dragable);
		break;
	case UI_TYPE::UI_SCROLLBAR:
		newUI = new UI_Scrollbar(positionValue, father, uiType, rect, uiName, dragable);
		break;
	case UI_TYPE::UI_HEALTHBAR:
		newUI = new UI_Healthbar(positionValue, father, uiType, rect, uiName, dragable);
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

UI* ModuleUIManager::AddButton(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, EVENT_ENUM eventR, bool menuClosure, bool includeFather, DRAGGABLE draggable, EVENT_ENUM eventTrigger)
{
	UI* newUI = new UI_Button(positionValue, father, uiType, rect, uiName, menuClosure, includeFather, draggable, eventR, eventTrigger);
	uiVector.push_back(newUI);
	return newUI;
}

void ModuleUIManager::ExecuteEvent(EVENT_ENUM eventId)
{

	int eventCheck = 0;

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
		// DELETE PORTRAIT			TODO
		break;

	case EVENT_ENUM::OPTION_MENU:
		CreateOptionsMenu();
		break;

	case EVENT_ENUM::CREDIT_MENU:
		// CREATE CREDIT WINDOW		TODO
		break;

	case EVENT_ENUM::PAUSE_GAME:
		CreatePauseMenu();
		break;

	case EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU:
		app->eventManager->GenerateEvent(EVENT_ENUM::RETURN_TO_MAIN_MENU, EVENT_ENUM::NULL_EVENT);
		break;
	}
}

void ModuleUIManager::CreateBasicInGameUI()
{
	
	SDL_Rect rect = RectConstructor(0, 0, 0, 0);;
	uint w(app->win->width), h(app->win->height);

	AddUIElement(fMPoint(w / app->win->GetUIScale() - 72, 35), nullptr, UI_TYPE::UI_PORTRAIT, rect, (P2SString)"portraitVector", DRAGGABLE::DRAG_OFF);

	rect = RectConstructor(221, 317, 162, 174);
	AddUIElement(fMPoint(0, h / app->win->GetUIScale() - rect.h), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"minimapBackground");

	rect = RectConstructor(449, 24, 24, 24);
	AddButton(fMPoint(w / app->win->GetUIScale() - (1.25f) * rect.w, (1.25f) * rect.w - rect.w), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"pauseButton", EVENT_ENUM::PAUSE_GAME);

}

void ModuleUIManager::CreatePauseMenu()
{
	SDL_Rect rect = RectConstructor(15, 271, 194, 231);;
	uint w(app->win->width), h(app->win->height);

	UI* father = AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), h / (app->win->GetUIScale() * 2) - (rect.h / 2)), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"pauseMenuBackground");

	int height = h / (app->win->GetUIScale() * 2) - (rect.h / 2) + 8;

	rect = RectConstructor(17, 12, 195, 36);
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), height), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"resumeButton", EVENT_ENUM::UNPAUSE_GAME, true, true);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), height + 44), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"saveButton", EVENT_ENUM::SAVE_GAME);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), height + 89), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"loadButton", EVENT_ENUM::LOAD_GAME);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), height + 134), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"optionButton", EVENT_ENUM::OPTION_MENU);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), height + 179), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"mainMenuButton", EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU, true, true);

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 32, height + 5), father, UI_TYPE::UI_TEXT, rect, (P2SString)"resumeText", DRAGGABLE::DRAG_OFF, "R E S U M E    G A M E");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 43, height + 49), father, UI_TYPE::UI_TEXT, rect, (P2SString)"saveText", DRAGGABLE::DRAG_OFF, "S A V E    G A M E");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 43, height + 94), father, UI_TYPE::UI_TEXT, rect, (P2SString)"loadText", DRAGGABLE::DRAG_OFF, "L O A D    G A M E");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 58, height + 139), father, UI_TYPE::UI_TEXT, rect, (P2SString)"optionText", DRAGGABLE::DRAG_OFF, "O P T I O N S");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 48, height + 184), father, UI_TYPE::UI_TEXT, rect, (P2SString)"mainMenuText", DRAGGABLE::DRAG_OFF, "M A I N    M E N U");

}

void ModuleUIManager::CreateMainMenu()
{
	SDL_Rect rect = RectConstructor(17, 12, 195, 36);
	uint w(app->win->width), h(app->win->height);
	
	// TODO Charge an image here that serves as father

//	AddButton(fMPoint(w / app->win->GetUIScale() - rect.w - 20, (h / (app->win->GetUIScale() * 4))), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"continueButton", true, EVENT_ENUM::START_GAME_FROM_CONTINUE, DRAGGABLE::DRAG_OFF);

	AddButton(fMPoint(w / app->win->GetUIScale() - rect.w - 20, (h / (app->win->GetUIScale() * 4)) + 40), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"newGameButton", EVENT_ENUM::START_GAME, true, true);

	AddButton(fMPoint(w / app->win->GetUIScale() - rect.w - 20, (h / (app->win->GetUIScale() * 4)) + 80), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"optionsButton", EVENT_ENUM::OPTION_MENU);

	AddButton(fMPoint(w / app->win->GetUIScale() - rect.w - 20, (h / (app->win->GetUIScale() * 4)) + 120), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"creditsButton", EVENT_ENUM::CREDIT_MENU);

	AddButton(fMPoint(w / app->win->GetUIScale() - rect.w - 20, (h / (app->win->GetUIScale() * 4)) + 160), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"exitGameButton", EVENT_ENUM::EXIT_GAME, true, true);

//	AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w + 5, (h / (app->win->GetUIScale() * 4)) + 5), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"continueText", DRAGGABLE::DRAG_OFF, "C O N T I N U E    G A M E");
	
	AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w + 35, (h / (app->win->GetUIScale() * 4)) + 45), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"newGameText", DRAGGABLE::DRAG_OFF, "N E W    G A M E");
	
	AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w + 40, (h / (app->win->GetUIScale() * 4)) + 85), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"optionsText", DRAGGABLE::DRAG_OFF, "O P T I O N S");
	
	AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w + 42, (h / (app->win->GetUIScale() * 4)) + 125), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"creditsText", DRAGGABLE::DRAG_OFF, "C R E D I T S");
	
	AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w + 30, (h / (app->win->GetUIScale() * 4)) + 165), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"exitGameText", DRAGGABLE::DRAG_OFF, "E X I T    G A M E");

}

void ModuleUIManager::CreateOptionsMenu()
{



}

SDL_Texture* ModuleUIManager::GetAtlasTexture() const
{ return atlas; }

SDL_Rect ModuleUIManager::RectConstructor(int x, int y, int w, int h)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}

void ModuleUIManager::LoadAtlas()
{
	if(!atlas)
	atlas = app->tex->Load("spritesheets/atlas.png");
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
}

void ModuleUIManager::DeleteUI(UI* father, bool includeFather)
{
	int numEntities = uiVector.size();
	
	int parentId = -1;

	for (int i = 0; i < numEntities; i++)
	{
		if (uiVector[i]->parent == father)
		{
			uiVector.erase(uiVector.begin() + i);
			i--;
			numEntities = uiVector.size();
		}
		else if (uiVector[i] == father && includeFather == true)
		{
			parentId = i;
		}
	}

	if (parentId != -1)
	{
		uiVector.erase(uiVector.begin() + parentId);
	}
}
