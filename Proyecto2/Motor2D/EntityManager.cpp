#include "App.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Entity.h"
#include "Collision.h"
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
	BROFILER_CATEGORY("Entity Manager Awake", Profiler::Color::DarkCyan);
	
	bool ret = true;

	return ret;
}


// Called before the first frame
bool ModuleEntityManager::Start()
{
	bool ret = true;
	SDL_Point pos{ 100, 200 };


	AddEntity(ENTITY_TYPE::HERO_MELEE, pos.x, pos.y);


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
		entityVector[i]->PreUpdate(dt);
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
		entityVector[i]->Update(dt);
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
		entityVector[i]->PostUpdate(dt);
	}

	return ret;
}

//// Called before quitting
bool ModuleEntityManager::CleanUp()
{
	int numEntities = entityVector.size();

	//Iterate though all the entitie's PreUpdates
	for (int i = 0; i < numEntities; i++)
	{
		RELEASE(entityVector[i]);
		entityVector[i] = nullptr;
		entityVector.erase(entityVector.begin() + i);
	}

	entityVector.clear();

	return true;
}

void ModuleEntityManager::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->entCallback != nullptr)
	{
		c1->entCallback->OnCollision(c2);
	}
}

//Add an entity
Entity* ModuleEntityManager::AddEntity(ENTITY_TYPE type, int x, int y)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case ENTITY_TYPE::PARTICLE:
		break;
	case ENTITY_TYPE::EMITER:
		break;
	case ENTITY_TYPE::PARTICLE_SYSTEM:
		break;
	case ENTITY_TYPE::HERO_MELEE:
	{
		//Test Things :)
		Animation animation;
		animation.PushBack(SDL_Rect{ 100, 100, 100, 100 }, 50, 0, 0);
		SDL_Texture* texture = app->tex->Load("spritesheets/characters/suitmale.png");


		Hero* tmpHero = new Hero(SDL_Point{ x,y }, ENTITY_TYPE::HERO_MELEE, texture, { 0,0,100,100 }, COLLIDER_HERO, this, animation, 1, 100, 1, 50, 1, 20, 20, 20, 20, 20, 20, 20, 20, 20);
		heroVector.push_back(tmpHero);
		ret = tmpHero;
	}
		break;
	case ENTITY_TYPE::HERO_RANGED:
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		break;
	case ENTITY_TYPE::BLDG_TURRET:
		break;
	case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
		break;
	case ENTITY_TYPE::BLDG_BASE:
		break;
	case ENTITY_TYPE::BLDG_BARRICADE:
		break;
	}

	if (ret != nullptr)
	{
		ret->vectorPosition = entityVector.size();
		entityVector.push_back(ret);
	}

	return ret;
}


// Checks if there is an entity in the mouse Click position 
Entity* ModuleEntityManager::CheckEntityOnClick(SDL_Point mousePos)
{
	int numEntities = entityVector.size();

	Collider* col;
	//Iterate though all the entitiies
	for (int i = 0; i < numEntities; i++)
	{
		col = entityVector[i]->GetCollider();

		if (col != nullptr) 
		{
			if (SDL_PointInRect(&mousePos, &col->rect))
			{
				return entityVector[i];
			}
		}
	}

	return nullptr;
}


void ModuleEntityManager::CheckEntityOnSelection(SDL_Rect &selection, std::vector<Hero*>* heroPlayerVector)
{
	int numEntities = heroVector.size();

	heroPlayerVector->clear();

	Collider* col;
	//Iterate though all the entitiies
	for (int i = 0; i < numEntities; i++)
	{
		col = heroVector[i]->GetCollider();

		if (col != nullptr)
		{
			if (col->CheckCollision(selection))
			{
				heroPlayerVector->push_back(heroVector[i]);
			}
		}
	}
}

bool ModuleEntityManager::DeleteEntity(Entity* toDelete)
{
	if (toDelete == nullptr || toDelete->vectorPosition == NULL)
		return false;

	int vectorIndex = toDelete->vectorPosition;

	RELEASE(entityVector[vectorIndex]);
	entityVector[vectorIndex] = nullptr;
	entityVector.erase(entityVector.begin() + vectorIndex);
}