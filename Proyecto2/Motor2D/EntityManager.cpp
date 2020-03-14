#include "App.h"
#include "EntityManager.h"
#include "Entity.h"
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