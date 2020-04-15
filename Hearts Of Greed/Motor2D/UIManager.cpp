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
#include "Brofiler/Brofiler/Brofiler.h"

ModuleUIManager::ModuleUIManager() : atlas(nullptr)
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
	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_ON_CLICK, this);
	app->eventManager->EventRegister(EVENT_ENUM::CREATE_SHOP, this);


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

	CheckListener(this);

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

	for (int i = numElements - 1; i > -1; i--)
	{
		RELEASE(uiVector[i]);
		uiVector[i] = nullptr;
	}

	uiVector.clear();

	portraitPointer = nullptr;

	return true;
}

UI* ModuleUIManager::AddUIElement(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, Entity* entity, DRAGGABLE dragable, char* text, SDL_Color color, _TTF_Font* font)
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
	case UI_TYPE::UI_SCROLLBAR:
		newUI = new UI_Scrollbar(positionValue, father, uiType, rect, uiName, dragable);
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
	bool hiding, bool hoverMove, DRAGGABLE draggable, EVENT_ENUM eventTrigger)
{
	UI* newUI = new UI_Button(positionValue, father, uiType, rect, uiName, eventR, menuClosure, includeFather, hiding, hoverMove, draggable, eventTrigger);
	uiVector.push_back(newUI);
	return newUI;
}

void ModuleUIManager::ExecuteEvent(EVENT_ENUM eventId)
{

	int eventCheck = 0;
	UI_Portrait* portrait = nullptr;

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
		// CREATE CREDIT WINDOW		TODO
		break;

	case EVENT_ENUM::PAUSE_GAME:
		CreatePauseMenu();
		break;

	case EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU:
		app->eventManager->GenerateEvent(EVENT_ENUM::RETURN_TO_MAIN_MENU, EVENT_ENUM::NULL_EVENT);
		break;

	case EVENT_ENUM::ENTITY_ON_CLICK:
		CreateEntityPortrait(); 
		break;

	case EVENT_ENUM::CREATE_SHOP:
		CreateShopMenu();
		break;
	}
}

void ModuleUIManager::CreateBasicInGameUI()
{
	
	SDL_Rect rect = RectConstructor(0, 0, 0, 0);;
	uint w(app->win->width), h(app->win->height);
	UI* father;
	char resources;

	rect = RectConstructor(556, 35, 15, 14);
	father = AddButton(fMPoint(w / app->win->GetUIScale() - 87, 35), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"PortraitHideButton", EVENT_ENUM::NULL_EVENT, false, false, true);

	AddUIElement(fMPoint(w / app->win->GetUIScale() - 72, 35), father, UI_TYPE::UI_PORTRAIT, rect, (P2SString)"portraitVector", nullptr, DRAGGABLE::DRAG_OFF);

	rect = RectConstructor(540, 35, 15, 14);
	father= AddButton(fMPoint(162, h / app->win->GetUIScale() - 85), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"minimapHideButton", EVENT_ENUM::NULL_EVENT, false, false, true);

	rect = RectConstructor(221, 317, 162, 174);
	rect.h = app->minimap->height;
	AddUIElement(fMPoint(0, (h- rect.h-20) / app->win->GetUIScale() ), father, UI_TYPE::UI_IMG, rect, (P2SString)"minimapBackground");

	rect = RectConstructor(449, 24, 24, 24);
	AddButton(fMPoint(w / app->win->GetUIScale() - (1.25f) * rect.w, (1.25f) * rect.w - rect.w), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"pauseButton", EVENT_ENUM::PAUSE_GAME);

	rect = RectConstructor(415, 435, 65, 30);
	AddUIElement(fMPoint(w / app->win->GetUIScale() - 99, 0), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"resourceBackground");

	rect = RectConstructor(18, 209, 11, 19);
	AddUIElement(fMPoint(w / app->win->GetUIScale() - 93, 7), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"resourceIcon");

	sprintf_s(&resources, 10, "%d", app->player->GetResources());
	AddUIElement(fMPoint(w / app->win->GetUIScale() - 64, 3), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"resourceText", nullptr, DRAGGABLE::DRAG_OFF, &resources);

}

void ModuleUIManager::CreatePauseMenu()
{
	SDL_Rect rect = RectConstructor(15, 271, 194, 231);
	uint w(app->win->width), h(app->win->height);

	UI* father = AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), h / (app->win->GetUIScale() * 2) - (rect.h / 2)), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"pauseMenuBackground");

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
	UI* father = nullptr;

	father = AddUIElement(fMPoint((w / app->win->GetUIScale() / 2) - (rect.w / 2), (h / app->win->GetUIScale() / 2) - (rect.h / 2)), father, UI_TYPE::UI_IMG, rect, (P2SString)"optionBackground");

	AddUIElement(fMPoint((w / app->win->GetUIScale() / 2) - (rect.w / 2) + 30, (h / app->win->GetUIScale() / 2) - (rect.h / 2)), father, UI_TYPE::UI_TEXT, rect, (P2SString)"optionText", nullptr, DRAGGABLE::DRAG_OFF, "O P T I O N S");

	rect = RectConstructor(424, 25, 23, 23);
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) + (278 / 2) - (3* rect.w / 4), h / (app->win->GetUIScale() * 2) - (153 / 2) - (1 * rect.h / 4)), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"closeButton", EVENT_ENUM::NULL_EVENT, true, true);


}

void ModuleUIManager::CreateEntityPortrait()
{
	uint w(app->win->width), h(app->win->height);

	SDL_Rect rect = RectConstructor(391, 370, 275, 131);
	AddUIElement(fMPoint(w / app->win->GetUIScale() - rect.w / 2, h / app->win->GetUIScale() - rect.h), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"portraitBG");

	rect = RectConstructor(727, 203, 65, 51);
	AddUIElement(fMPoint(w / app->win->GetUIScale() - 2 * rect.w + 12, h / app->win->GetUIScale() - rect.h - 5), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"imgBG");

	rect = RectConstructor(0, 0, 100, 100);

	static char stats[40];

	Hero* hero;

	SDL_Color std{(255),(255), (255), (255)};

	switch (app->player->focusedEntity->GetType()) {
	
	case ENTITY_TYPE::BLDG_BASE:
		
		Base* base;
		base = (Base*)app->player->focusedEntity;

		//img portrait
		rect = RectConstructor(634, 78, 68, 62);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 2 * rect.w + 15, h / app->win->GetUIScale() - rect.h - 6), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"enemyImg");
		
		//hp bar
		rect = RectConstructor(312, 85, 60, 7);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 60)), nullptr, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"HPbar", base, DRAGGABLE::DRAG_OFF, "HPbar");

		//stats
		sprintf_s(stats, 40, "HP: %i", base->GetHP());
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 55)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"HP", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rsrc: %i", base->GetRsrc());
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 45)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"Rsrc", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);
		
//		if (base->GetAlignment() == ENTITY_ALIGNEMENT::PLAYER) {		TODO: TAKE COMMENTS OUT AFTER TESTING THE SHOP BUTTON
			//shop button
			rect = RectConstructor(480, 62, 33, 33);
			AddButton(fMPoint(w / app->win->GetUIScale() - rect.w - 5, (h / (app->win->GetUIScale())) - 35), nullptr, UI_TYPE::UI_BUTTON, rect, (P2SString)"S H O P", EVENT_ENUM::CREATE_SHOP);
//		}
		break;

	case ENTITY_TYPE::BLDG_TURRET:
		Turret* turret;
		turret = (Turret*)app->player->focusedEntity;

		//img portrait
		rect = RectConstructor(561, 77, 68, 62);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 2 * rect.w + 18, h / app->win->GetUIScale() - rect.h - 2), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"enemyImg");

		//stats

		sprintf_s(stats, 40, "LVL: %i", turret->GetLvl());
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 35, (h / (app->win->GetUIScale()) - 15)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"lvl", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AD: %i", turret->GetAD());
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 35, (h / (app->win->GetUIScale()) - 30)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"AD", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rng: %i", turret->GetRng());
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 35, (h / (app->win->GetUIScale()) - 45)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"Rng", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AS: %i", turret->GetAS());
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 35, (h / (app->win->GetUIScale()) - 60)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"AS", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		break;


	case ENTITY_TYPE::HERO_GATHERER:

		
		hero = (Hero*)app->player->focusedEntity;

		//img portrait
		rect = RectConstructor(352, 149, 66, 51);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 2 * rect.w + 10, h / app->win->GetUIScale() - rect.h - 2), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"heroImg");

		//health bar
		rect = RectConstructor(312, 85, 60, 7);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 60)), nullptr, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"HPbar", hero, DRAGGABLE::DRAG_OFF, "HPbar");

		rect = RectConstructor(374, 85, 60, 7);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 50)), nullptr, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"Ebar", hero, DRAGGABLE::DRAG_OFF, "Ebar");


		//stats
		sprintf_s(stats, 40, "HP: %i", hero->hitPointsCurrent);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 46)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"HP", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "E: %i", hero->energyPoints);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 30, (h / (app->win->GetUIScale()) - 46)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"E", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AD: %i", hero->attackDamage);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 36)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"AD", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AS: %f", hero->attackSpeed);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 30, (h / (app->win->GetUIScale()) - 36)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"AS", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rng: %i", hero->attackRange);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 26)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"Rng", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rec: %i", hero->recoveryHitPointsRate);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 30, (h / (app->win->GetUIScale()) - 26)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"Rec", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "EXP: %i / %i", hero->heroXP, hero->expToLevelUp);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 16)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"Exp", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		break;

	case ENTITY_TYPE::HERO_RANGED:
		//This does not exist in vertical slice
		break;

	case ENTITY_TYPE::HERO_MELEE:
		
		hero = (Hero*)app->player->focusedEntity;

		//img portrait
		rect = RectConstructor(562, 149, 66, 51);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 2 * rect.w + 10, h / app->win->GetUIScale() - rect.h - 2), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"heroImg");

		//health bar
		rect = RectConstructor(312, 85, 60, 7);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 60)), nullptr, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"HPbar", hero, DRAGGABLE::DRAG_OFF, "HPbar");

		rect = RectConstructor(374, 85, 60, 7);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 50)), nullptr, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"Ebar", hero, DRAGGABLE::DRAG_OFF, "Ebar");


		//stats
		sprintf_s(stats, 40, "HP: %i", hero->hitPointsCurrent);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 46)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"HP", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "E: %i", hero->energyPoints);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 30, (h / (app->win->GetUIScale()) - 46)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"E", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AD: %i", hero->attackDamage);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 36)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"AD", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AS: %f", hero->attackSpeed);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 30, (h / (app->win->GetUIScale()) - 36)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"AS", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rng: %i", hero->attackRange);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 26)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"Rng", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rec: %i", hero->recoveryHitPointsRate);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 30, (h / (app->win->GetUIScale()) - 26)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"Rec", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "EXP: %i / %i", hero->heroXP, hero->expToLevelUp);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 16)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"Exp", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		break;

	case ENTITY_TYPE::ENEMY:

		Enemy* enemy;
		enemy = (Enemy*)app->player->focusedEntity;

		//img portrait
		rect = RectConstructor(894, 116, 70, 79);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 2 * rect.w + 30, h / app->win->GetUIScale() - rect.h - 10), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"enemyImg");

		//health bar
		rect = RectConstructor(219, 83, 122, 16);
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 90, (h / (app->win->GetUIScale()) - 60)), nullptr, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"HPbar", enemy, DRAGGABLE::DRAG_OFF, "HPbar");

		//stats
		sprintf_s(stats, 40, "HP: %i", enemy->GetHP());
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 55)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"HP", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		AddUIElement(fMPoint(w / app->win->GetUIScale() - 25, (h / (app->win->GetUIScale()) - 55)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"E", nullptr, DRAGGABLE::DRAG_OFF, "E: 0",std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AD: %i", enemy->GetAD());
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 40)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"AD", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "AS: %i", enemy->GetAS());
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 25, (h / (app->win->GetUIScale()) - 40)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"AS", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rng: %i", enemy->GetVision());
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 60, (h / (app->win->GetUIScale()) - 25)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"Rng", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		sprintf_s(stats, 40, "Rec: %i", enemy->GetRecov());
		AddUIElement(fMPoint(w / app->win->GetUIScale() - 25, (h / (app->win->GetUIScale()) - 25)), nullptr, UI_TYPE::UI_TEXT, rect, (P2SString)"Rec", nullptr, DRAGGABLE::DRAG_OFF, stats, std, app->fonts->fonts[1]);

		break;

	default:
		break;
	}
}

void ModuleUIManager::CreateShopMenu()
{
	SDL_Rect rect = RectConstructor(15, 271, 194, 231);
	uint w(app->win->width), h(app->win->height);

	UI* father = AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2), h / (app->win->GetUIScale() * 2) - (rect.h / 2)), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"shopBackground");

	// Heroes
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (rect.w / 2) + 10, h / (app->win->GetUIScale() * 2) - (rect.h / 2) + 5), father, UI_TYPE::UI_TEXT, rect, (P2SString)"heroUpgradeText", nullptr, DRAGGABLE::DRAG_OFF, "H E R O   U P G R A D E");

	rect = RectConstructor(581, 24, 36, 27);
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 30, h / (app->win->GetUIScale() * 2) - (231 / 2) + 35), father, UI_TYPE::UI_IMG, rect, (P2SString)"heroGathererPortrait");

	rect.x = 619;
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 80, h / (app->win->GetUIScale() * 2) - (231 / 2) + 35), father, UI_TYPE::UI_IMG, rect, (P2SString)"heroRangedPortrait");

	rect.x = 658;
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 130, h / (app->win->GetUIScale() * 2) - (231 / 2) + 35), father, UI_TYPE::UI_IMG, rect, (P2SString)"heroMeleePortrait");

	rect = RectConstructor(653, 54, 46, 14);	// TODO Actually read the event of level up in the player / entity manager; also spend the resource (do it only if you have enough)
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 25, h / (app->win->GetUIScale() * 2) - (231 / 2) + 65), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"heroGathererLevelButton", EVENT_ENUM::GATHERER_LEVEL_UP);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 75, h / (app->win->GetUIScale() * 2) - (231 / 2) + 65), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"heroRangedLevelButton", EVENT_ENUM::RANGED_LEVEL_UP);

	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 125, h / (app->win->GetUIScale() * 2) - (231 / 2) + 65), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"heroMeleeLevelButton", EVENT_ENUM::MELEE_LEVEL_UP);

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 30, h / (app->win->GetUIScale() * 2) - (231 / 2) + 57), father, UI_TYPE::UI_TEXT, rect, (P2SString)"heroGathererText", nullptr, DRAGGABLE::DRAG_OFF, "Lvl up");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 80, h / (app->win->GetUIScale() * 2) - (231 / 2) + 57), father, UI_TYPE::UI_TEXT, rect, (P2SString)"heroRangedText", nullptr, DRAGGABLE::DRAG_OFF, "Lvl up");

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 130, h / (app->win->GetUIScale() * 2) - (231 / 2) + 57), father, UI_TYPE::UI_TEXT, rect, (P2SString)"heroMeleeText", nullptr, DRAGGABLE::DRAG_OFF, "Lvl up");

	// TODO: add the amount of resources that have to be spent for the levelling up under the correspondant button (-x gem icon)

	// Turrets
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 10, h / (app->win->GetUIScale() * 2) - (231 / 2) + 85), father, UI_TYPE::UI_TEXT, rect, (P2SString)"turretBuildingText", nullptr, DRAGGABLE::DRAG_OFF, "T U R R E T   B U I L D I N G");

	rect = RectConstructor(696, 12, 34, 40);
	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 120, h / (app->win->GetUIScale() * 2) - (231 / 2) + 135), father, UI_TYPE::UI_IMG, rect, (P2SString)"turretPortrait");

	rect = RectConstructor(653, 54, 46, 14);	// TODO Actually read the event of enabling the turret building mode; also spend the resource (do it only if you have enough)
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 40, h / (app->win->GetUIScale() * 2) - (231 / 2) + 120), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"turretPurchaseButton", EVENT_ENUM::TURRET_PURCHASED);

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 50, h / (app->win->GetUIScale() * 2) - (231 / 2) + 112), father, UI_TYPE::UI_TEXT, rect, (P2SString)"turretPurchaseText", nullptr, DRAGGABLE::DRAG_OFF, "Buy");

	rect = RectConstructor(653, 54, 46, 14);	// TODO Actually read the event of enabling the turret building mode; also spend the resource (do it only if you have enough)
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 40, h / (app->win->GetUIScale() * 2) - (231 / 2) + 160), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"turretLevelButton", EVENT_ENUM::TURRET_PURCHASED);

	AddUIElement(fMPoint(w / (app->win->GetUIScale() * 2) - (194 / 2) + 45, h / (app->win->GetUIScale() * 2) - (231 / 2) + 152), father, UI_TYPE::UI_TEXT, rect, (P2SString)"turretLevelText", nullptr, DRAGGABLE::DRAG_OFF, "Lvl up");

	rect = RectConstructor(424, 25, 23, 23);
	AddButton(fMPoint(w / (app->win->GetUIScale() * 2) + (194 / 2) - (rect.w / 2), h / (app->win->GetUIScale() * 2) - (231 / 2) - (rect.h / 2)), father, UI_TYPE::UI_BUTTON, rect, (P2SString)"closeButton", EVENT_ENUM::NULL_EVENT, true, true);

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