#include "Player.h"
#include "Entity.h"
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

	ret = Select();

	ret = Check();


	return ret;
}