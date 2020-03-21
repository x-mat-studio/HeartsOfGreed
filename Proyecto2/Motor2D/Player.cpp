#include "Player.h"
#include "Entity.h"
#include "Hero.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "EntityManager.h"
#include "Brofiler/Brofiler/Brofiler.h"

ModulePlayer::ModulePlayer() : Module(), focusedEntity(nullptr)
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
	

	return true;
}


// Called each loop iteration
bool ModulePlayer::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Player Pre-Update", Profiler::Color::Blue)

	

	HandleInput();

	return true;
}


// Called each loop iteration
bool ModulePlayer::Update(float dt)
{
	BROFILER_CATEGORY("Player Update", Profiler::Color::Blue)

	

	return true;
}

// Called each loop iteration
bool ModulePlayer::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Player Post-Update", Profiler::Color::Blue)


	return true;
}


//Handles Player Input
bool ModulePlayer::HandleInput()
{
	if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_DOWN)
	{
		RightClick();
	}

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN) 
	{
		Click();
	}
	
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT)
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

	SDL_Point mousePosition;

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

	app->entityManager->CheckHeroOnSelection(SDL_Rect{ rectX, rectY, rectW, rectH }, &heroesVector);

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
			heroesVector[i]->LockOn(focusedEntity);
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
