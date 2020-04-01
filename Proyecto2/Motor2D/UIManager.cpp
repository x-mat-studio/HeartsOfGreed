#include "App.h"
#include "Textures.h"
#include "UIManager.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Text.h"
#include "EventManager.h"
#include "Window.h"
#include "Brofiler/Brofiler/Brofiler.h"


ModuleUIManager::ModuleUIManager()
{
	name.create("UIManager");
}


// Destructor
ModuleUIManager::~ModuleUIManager()
{}


// Called before render is available
bool ModuleUIManager::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("UI Manager Awake", Profiler::Color::MediumPurple);

	bool ret = true;

	app->eventManager->EventRegister(EVENT_ENUM::HERO_MELEE_ON_BATTLE, this);
	app->eventManager->EventRegister(EVENT_ENUM::HERO_GATHERER_ON_BATTLE, this);
	app->eventManager->EventRegister(EVENT_ENUM::HERO_RANGED_ON_BATTLE, this);
	app->eventManager->EventRegister(EVENT_ENUM::HERO_MELEE_OUT, this);
	app->eventManager->EventRegister(EVENT_ENUM::HERO_GATHERER_OUT, this);
	app->eventManager->EventRegister(EVENT_ENUM::HERO_RANGED_OUT, this);


	return ret;
}


// Called before the first frame
bool ModuleUIManager::Start()
{
	bool ret = true;

	atlas = app->tex->Load("spritesheets/atlas.png");

	// THIS SHOULDN'T GO HERE; IT'S FOR TESTIN; TODO
	app->eventManager->GenerateEvent(EVENT_ENUM::HERO_MELEE_ON_BATTLE, EVENT_ENUM::NULL_EVENT);
	app->eventManager->GenerateEvent(EVENT_ENUM::HERO_GATHERER_ON_BATTLE, EVENT_ENUM::NULL_EVENT);
	app->eventManager->GenerateEvent(EVENT_ENUM::HERO_RANGED_ON_BATTLE, EVENT_ENUM::NULL_EVENT);

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
		uiVector[i]->PostUpdate(dt, atlas);
	}

	return ret;
}

//// Called before quitting
bool ModuleUIManager::CleanUp()
{
	

	return true;
}

UI* ModuleUIManager::AddUIElement(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool dragable)
{
	UI* newUI = nullptr;

	switch (uiType)
	{
	case UI_TYPE::UI_IMG:
		newUI = new UI_Image(positionValue, father, uiType, rect, uiName, dragable);
		break;
	case UI_TYPE::UI_TEXT:
		newUI = new UI_Text(positionValue, father, uiType, rect, uiName, dragable);
		break;
	case UI_TYPE::UI_BUTTON:
		newUI = new UI_Button(positionValue, father, uiType, rect, uiName, dragable);
		break;
	}

	if (newUI != nullptr)
	{
		uiVector.push_back(newUI);
	}

	return newUI;
}

void ModuleUIManager::RemoveDeletedUI()
{
	int numEntitys = uiVector.size();

	for (int i = 0; i < numEntitys; i++)
	{
		if (uiVector[i]->toDelete == true)
		{
			uiVector.erase(uiVector.begin() + i);
		}
	}

}

void ModuleUIManager::ExecuteEvent(EVENT_ENUM eventId)
{
	SDL_Rect rect;
	uint w (app->win->width), h(app->win->height);

	switch (eventId)
	{

	case EVENT_ENUM::HERO_MELEE_ON_BATTLE:
		
		rect = RectConstructor(889, 201, 100, 78);
		AddUIElement(fMPoint (w / app->win->GetScale() - rect.w, 60 / app->win->GetScale()), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"meleeHeroMark", false);
		rect = RectConstructor(776, 206, 94, 73); 
		AddUIElement(fMPoint(w / app->win->GetScale() - rect.w - 3, 65 / app->win->GetScale()), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"meleeHero", false);

		break;
	case EVENT_ENUM::HERO_GATHERER_ON_BATTLE:
		
		rect = RectConstructor(889, 201, 100, 78);
		AddUIElement(fMPoint(w / app->win->GetScale() - rect.w, 60 / app->win->GetScale() + rect.h + 5), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"gathererHeroMark", false);
		rect = RectConstructor(486, 206, 94, 73);
		AddUIElement(fMPoint(w / app->win->GetScale() - rect.w - 3, 60 / app->win->GetScale() + rect.h + 12), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"gathererHero", false);
		
		break;
	case EVENT_ENUM::HERO_RANGED_ON_BATTLE:
		
		rect = RectConstructor(889, 201, 100, 78);
		AddUIElement(fMPoint(w / app->win->GetScale() - rect.w, 60 / app->win->GetScale() + 2 * rect.h + 10), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"rangedHeroMark", false);
		rect = RectConstructor(204, 206, 94, 73);
		AddUIElement(fMPoint(w / app->win->GetScale() - rect.w - 3, 60 / app->win->GetScale() + 2 * rect.h + 22), nullptr, UI_TYPE::UI_IMG, rect, (P2SString)"rangedHero", false);
		
		break;
	case EVENT_ENUM::HERO_MELEE_OUT:

		break;
	case EVENT_ENUM::HERO_GATHERER_OUT:

		break;
	case EVENT_ENUM::HERO_RANGED_OUT:

		break;

	}
}

SDL_Rect ModuleUIManager::RectConstructor(int x, int y, int w, int h)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}