#include "App.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Entity.h"
#include "Hero.h"
#include "Brofiler/Brofiler/Brofiler.h"


ModuleEntityManager::ModuleEntityManager()
{
	name.create("entityManager");
}


// Destructor
ModuleEntityManager::~ModuleEntityManager()
{}


// Called before render is available
bool ModuleEntityManager::Awake(pugi::xml_node& config)
{
	//BROFILER_CATEGORY("Entity Manager Awake", Profiler::Color::DarkCyan);
	std::vector<int> pos{ 100, 200 };

	Animation animation;
	animation.PushBack(SDL_Rect{ 100, 100, 100, 100 }, 50, 0, 0);
	test = new Hero(pos, ENTITY_TYPE::HERO_MELEE, app->tex->Load("/textures/POINTERS/pngs/left_right_up_down/circle.png"), animation, 1, 100, 1, 50, 1, 20, 20, 20, 20, 20, 20, 20, 20, 20);

	bool ret = true;

	return ret;
}


// Called before the first frame
bool ModuleEntityManager::Start()
{
	bool ret = true;

	return ret;
}

// Called each loop iteration
bool ModuleEntityManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Entity Manager Pre-Update", Profiler::Color::Blue)

	bool ret = true;

	int numEntities = entityVector.size();

	//Iterate though all the entitie's PreUpdates
	for (int i = 0; i < numEntities; i++)
	{
		entityVector[i].PreUpdate(dt);
	}
	return ret;
}


// Called each loop iteration
bool ModuleEntityManager::Update(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::Blue)

	bool ret = true;


	int numEntities = entityVector.size();

	//Iterate though all the entitie's PreUpdates
	for (int i = 0; i < numEntities; i++)
	{
		entityVector[i].Update(dt);
	}
	return ret;
}

// Called each loop iteration
bool ModuleEntityManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::Blue)

	bool ret = true;

	int numEntities = entityVector.size();

	//Iterate though all the entitie's PreUpdates
	for (int i = 0; i < numEntities; i++)
	{
		entityVector[i].PostUpdate(dt);
	}
	return ret;
}