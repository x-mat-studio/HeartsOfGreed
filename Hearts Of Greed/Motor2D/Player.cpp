#include "App.h"

#include "Player.h"

#include "Hero.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "EntityManager.h"
#include "Brofiler/Brofiler/Brofiler.h"
#include "Map.h"
#include "EventManager.h"

ModulePlayer::ModulePlayer() :
	
Module(), 

focusedEntity(nullptr), 

resources(0),
selectRect{ 0,0,0,0 },

selectUnits(false),
entityComand(false),
entityInteraction(false),
buildMode(false),

buildingToBuild(ENTITY_TYPE::UNKNOWN)

{
	name.create("player");
}


ModulePlayer::~ModulePlayer()
{}


// Called before render is available
bool ModulePlayer::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("Player Awake", Profiler::Color::DarkCyan);



	return true;
}


// Called before the first frame
bool ModulePlayer::Start()
{
	//register the module as a listener to the following events
	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_COMMAND, this);
	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_INTERACTION, this);
	app->eventManager->EventRegister(EVENT_ENUM::SELECT_UNITS, this);
	app->eventManager->EventRegister(EVENT_ENUM::STOP_SELECTING_UNITS, this);


	return true;
}


// Called each loop iteration
bool ModulePlayer::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Player Pre-Update", Profiler::Color::Blue);

	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_STATE::KEY_DOWN) // For debug purposes
	{
		ActivateBuildMode(ENTITY_TYPE::BUILDING);
	}
	else if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_STATE::KEY_DOWN) {
		DesactivateBuildMode();
	}

	CheckListener(this);

	HandleInput();


	return true;
}


// Called each loop iteration
bool ModulePlayer::Update(float dt)
{
	BROFILER_CATEGORY("Player Update", Profiler::Color::Blue)

	CheckListener(this);

	return true;
}

// Called each loop iteration
bool ModulePlayer::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Player Post-Update", Profiler::Color::Blue)

	CheckListener(this);
	
	if (buildMode == true)
	{
		app->entityManager->PlayerBuildPreview((-app->render->currentCamX + clickPosition.x) / app->win->GetScale(), (-app->render->currentCamY + clickPosition.y) / app->win->GetScale(), buildingToBuild);
	}

	DrawSelectQuad();
	return true;
}


//Handles Player Input
bool ModulePlayer::HandleInput()
{
	if (buildMode == false)
	{
		if (entityComand)
		{
			entityComand = false;
			RightClick();
		}
		if (entityInteraction)
		{
			entityInteraction = false;
			Click();
		}
		else if (selectUnits)
		{
			Select();
		}
	}

	else
	{
		app->input->GetMousePositionRaw(clickPosition.x, clickPosition.y);


		if (entityInteraction)
		{
			entityInteraction = false;
			BuildClick();
		}
	}

	return true;
}


bool ModulePlayer::Click()
{
	focusedEntity = nullptr;

	app->input->GetMousePositionRaw(clickPosition.x, clickPosition.y);

	clickPosition.x = (-app->render->currentCamX + clickPosition.x) / app->win->GetScale();
	clickPosition.y = (-app->render->currentCamY + clickPosition.y) / app->win->GetScale();

	focusedEntity = app->entityManager->CheckEntityOnClick(clickPosition);

	if (focusedEntity != nullptr)
	{
		return true;
	}

	return false;
}


void ModulePlayer::Select()
{
	iMPoint mousePosition;

	app->input->GetMousePositionRaw(mousePosition.x, mousePosition.y);

	int rectX;
	int rectY;
	int rectW;
	int rectH;

	mousePosition.x = (-app->render->currentCamX + mousePosition.x) / app->win->GetScale();
	mousePosition.y = (-app->render->currentCamY + mousePosition.y) / app->win->GetScale();

	if (mousePosition.x > clickPosition.x)
	{
		rectX = clickPosition.x;
	}
	else
	{
		rectX = mousePosition.x;
	}

	rectW = abs(mousePosition.x - clickPosition.x);

	if (mousePosition.y > clickPosition.y)
	{
		rectY = clickPosition.y;
	}
	else
	{
		rectY = mousePosition.y;
	}

	rectH = abs(mousePosition.y - clickPosition.y);

	selectRect = { rectX,rectY, rectW,rectH };

	app->entityManager->CheckHeroOnSelection(selectRect, &heroesVector);

}


void ModulePlayer::RightClick()
{
	if (heroesVector.empty())
		return;

	if (Click())
	{
		int numHeroes = heroesVector.size();

		for (int i = 0; i < numHeroes; i++)
		{
			if (heroesVector[i]->LockOn(focusedEntity) == false)
			{
				heroesVector[i]->MoveTo(clickPosition.x, clickPosition.y);
			}
		}
	}

	else
	{
		int numHeroes = heroesVector.size();

		for (int i = 0; i < numHeroes; i++)
		{
			heroesVector[i]->MoveTo(clickPosition.x, clickPosition.y);
		}
	}
}


bool ModulePlayer::BuildClick()
{
	//Needs more work
	app->entityManager->AddEntity(buildingToBuild, (-app->render->currentCamX + clickPosition.x) / app->win->GetScale(), (-app->render->currentCamY + clickPosition.y) / app->win->GetScale());

	return true;
}


void ModulePlayer::ExecuteEvent(EVENT_ENUM eventId)
{
	
	switch (eventId)
	{
	
	case EVENT_ENUM::SELECT_UNITS:
		selectUnits = true;
		break;
	case EVENT_ENUM::STOP_SELECTING_UNITS:
		selectUnits = false;
		break;
	case EVENT_ENUM::ENTITY_COMMAND:
		entityComand = true;
		break;
	case EVENT_ENUM::ENTITY_INTERACTION:
		entityInteraction = true;
		break;
	}


}


void ModulePlayer::DrawSelectQuad()
{
	if (selectUnits)
	{
		app->render->DrawQuad(selectRect, 0, 200, 0, 50, false);
		app->render->DrawQuad(selectRect, 0, 200, 0, 100);
	}
	else
	{
		selectRect = { 0,0,0,0 };
	}
}


void ModulePlayer::AddResources(int gain)
{
	resources += gain;
}


bool ModulePlayer::UseResources(int cost)
{
	if (cost > resources)
		return false;


	else
	{
		resources -= cost;
		return true;
	}
}


bool ModulePlayer::ActivateBuildMode(ENTITY_TYPE building)
{
	if (buildMode == false || (building != ENTITY_TYPE::BLDG_TURRET && building != ENTITY_TYPE::BLDG_UPGRADE_CENTER && building != ENTITY_TYPE::BLDG_BARRICADE && building != ENTITY_TYPE::BUILDING))
	{
		buildMode = true;
		buildingToBuild = building;

		return true;
	}

	else
		return false;

}


void ModulePlayer::DesactivateBuildMode()
{
	buildMode = false;
	buildingToBuild = ENTITY_TYPE::UNKNOWN;
}