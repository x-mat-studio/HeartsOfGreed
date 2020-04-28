#include "App.h"
#include "Textures.h"
#include "UIManager.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "UI.h"
#include "UI_Group.h"
#include "Audio.h"
#include "Window.h"
#include "Minimap.h"
#include "Base.h"

#include "Brofiler/Brofiler/Brofiler.h"

ModuleUIManager::ModuleUIManager() : atlas(nullptr), lastShop(nullptr)
{
	name.create("UIManager");
}


// Destructor
ModuleUIManager::~ModuleUIManager()
{
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



	return ret;
}

// Called each loop iteration
bool ModuleUIManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("UI Manager Pre-Update", Profiler::Color::Purple);

	bool ret = true;

	CheckListener(this);

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

	//SOMETHING RELATED TO THE BUY MENU
	/*if (app->player != nullptr)
		app->player->(false);*/
	return true;
}


void ModuleUIManager::AddUIGroup(UI_Group* element)
{
	GROUP_TAG tag = element->GetTag();

	assert(tag == GROUP_TAG::NONE || CheckGroupTag(tag) == false); // You shouldn't have to ui groups with the same tag, something bad is happening

	uiGroupVector.push_back(element);
}



void ModuleUIManager::ExecuteEvent(EVENT_ENUM eventId)
{

	switch (eventId)
	{
		// When adding a Hero to these enum, add it to the checking function below
	case EVENT_ENUM::HERO_MELEE_CREATED:		break;
	case EVENT_ENUM::HERO_GATHERER_CREATED:		break;
	case EVENT_ENUM::HERO_RANGED_CREATED:		break;


	case EVENT_ENUM::HERO_MELEE_OUT:			break;
	case EVENT_ENUM::HERO_GATHERER_OUT:			break;
	case EVENT_ENUM::HERO_RANGED_OUT:			break;


	case EVENT_ENUM::OPTION_MENU:				break;

	case EVENT_ENUM::CREDIT_MENU:				break;

	case EVENT_ENUM::PAUSE_GAME:				break;

	case EVENT_ENUM::UNPAUSE_GAME_AND_RETURN_TO_MAIN_MENU:	break;

	case EVENT_ENUM::ENTITY_ON_CLICK:			break;

	case EVENT_ENUM::CREATE_SHOP:				break;

	case EVENT_ENUM::MUSIC_ADJUSTMENT:			break;

	case EVENT_ENUM::SFX_ADJUSTMENT:			break;

	case EVENT_ENUM::ENTITY_DEAD:				break;

	case EVENT_ENUM::HIDE_MENU:					break;

	case EVENT_ENUM::UNHIDE_MENU:				break;

	case EVENT_ENUM::EXIT_MENUS:				break;

	case EVENT_ENUM::FULLSCREEN_INPUT:			break;

	}
}


SDL_Texture* ModuleUIManager::GetAtlasTexture() const
{
	return atlas;
}


void ModuleUIManager::LoadAtlas()
{
	if (atlas == nullptr)
	{
		atlas = app->tex->Load("spritesheets/atlas.png");
	}
}


//Remember to aply changes
bool ModuleUIManager::MouseOnUI(iMPoint& mouse)
{
	int numEntities = uiGroupVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		/*if (uiGroupVector[i]->parent == nullptr && uiGroupVector[i]->enabled)
		{
			if (uiGroupVector[i]->worldPosition.x * app->win->GetUIScale() <= mouse.x && (uiGroupVector[i]->worldPosition.x + uiGroupVector[i]->box.w) * app->win->GetUIScale() >= mouse.x &&
				uiGroupVector[i]->worldPosition.y * app->win->GetUIScale() <= mouse.y && (uiGroupVector[i]->worldPosition.y + uiGroupVector[i]->box.h) * app->win->GetUIScale() >= mouse.y)
			{
				return true;
			}
		}*/
	}

	return false;
}


void ModuleUIManager::CheckFocusEntity()
{

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


