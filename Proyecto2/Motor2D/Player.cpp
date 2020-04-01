#include "Player.h"
#include "Entity.h"
#include "Hero.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "EntityManager.h"
#include "Brofiler/Brofiler/Brofiler.h"
#include "Map.h"
#include "EventManager.h"

ModulePlayer::ModulePlayer() : Module(), focusedEntity(nullptr), selectRect{ 0,0,0,0 }
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

	DrawSelectQuad();
	return true;
}


//Handles Player Input
bool ModulePlayer::HandleInput()
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