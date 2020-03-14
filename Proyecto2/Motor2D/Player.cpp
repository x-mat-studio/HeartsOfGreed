#include "Player.h"
#include "Entity.h"
#include "App.h"
#include "Input.h"
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

	bool ret = true;

	return ret;
}


// Called before the first frame
bool ModulePlayer::Start()
{
	bool ret = true;

	return ret;
}


// Called each loop iteration
bool ModulePlayer::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Player Pre-Update", Profiler::Color::Blue)

	bool ret = true;

	HandleInput();

	return ret;
}


// Called each loop iteration
bool ModulePlayer::Update(float dt)
{
	BROFILER_CATEGORY("Player Update", Profiler::Color::Blue)

	bool ret = true;

	return ret;
}

// Called each loop iteration
bool ModulePlayer::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Player Post-Update", Profiler::Color::Blue)

	bool ret = true;



	return ret;
}


//Handles Player Input
bool ModulePlayer::HandleInput()
{
	bool ret = true;

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN)
	ret = Click();
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT)
	ret = Select();

	return ret;
}


bool ModulePlayer::Click()
{
	bool ret = false;

	app->input->GetMousePosition(clickPosition.x, clickPosition.y);

	focusedEntity = app->entityManager->CheckEntityOnClick(clickPosition);

	if (focusedEntity != nullptr)
	{
		ret = true;
	}

	return ret;
}


bool ModulePlayer::Select()
{
	SDL_Point mousePosition;
	app->input->GetMousePosition(mousePosition.x, mousePosition.y);

	int rectX;
	int rectY;
	int rectW;
	int rectH;

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

	app->entityManager->CheckEntityOnSelection(SDL_Rect{ rectX, rectY, rectW, rectH }, &heroesVector);

	return true;
}