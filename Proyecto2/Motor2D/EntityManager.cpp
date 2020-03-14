#include "App.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Collision.h"
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

//// Called before quitting
bool ModuleEntityManager::CleanUp()
{
	entityVector.clear();

	return true;
}


//Add an entity
bool ModuleEntityManager::AddEntity(ENTITY_TYPE type, int x, int y)
{
	bool ret = false;

	switch (type)
	{
	case ENTITY_TYPE::PARTICLE:
		ret = true;
		break;
	case ENTITY_TYPE::EMITER:
		ret = true;
		break;
	case ENTITY_TYPE::PARTICLE_SYSTEM:
		ret = true;
		break;
	case ENTITY_TYPE::HERO_MELEE:
		ret = true;
		break;
	case ENTITY_TYPE::HERO_RANGED:
		ret = true;
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		ret = true;
		break;
	case ENTITY_TYPE::BLDG_TURRET:
		ret = true;
		break;
	case ENTITY_TYPE::BLDG_UPGRADE:
		ret = true;
		break;
	case ENTITY_TYPE::BLDG_BASE:
		ret = true;
		break;
	case ENTITY_TYPE::BLDG_BARRICADE:
		ret = true;
		break;
	}

	return ret;
}

// Checks if there is an entity in the mouse Click position 
Entity* ModuleEntityManager::CheckEntityOnClick(SDL_Point mousePos)
{
	int numEntities = entityVector.size();

	//Iterate though all the entitiies
	for (int i = 0; i < numEntities; i++)
	{
		if (SDL_PointInRect(&mousePos, &entityVector[i].GetCollider()->rect))
		{
			return &entityVector[i];
		}
	}
}