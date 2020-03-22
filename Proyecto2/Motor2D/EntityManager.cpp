#include "App.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Entity.h"
#include "Map.h"
#include "Collision.h"
#include "Hero.h"
#include "Enemy.h"
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

	iMPoint pos;
	pos.create(100, 200);

	Animation walkLeft = walkLeft.PushAnimation(config.child("suitmale"), "walk_left");
	Animation walkLeftUp = walkLeftUp.PushAnimation(config.child("suitmale"), "walk_left_up");
	Animation walkLeftDown = walkLeftDown.PushAnimation(config.child("suitmale"), "walk_left_down");
	Animation walkRightUp = walkRightUp.PushAnimation(config.child("suitmale"), "walk_right_up");
	Animation walkRightDown = walkRightDown.PushAnimation(config.child("suitmale"), "walk_right_down");
	Animation walkRight = walkRight.PushAnimation(config.child("suitmale"), "walk_right");
	Animation idleRight = idleRight.PushAnimation(config.child("suitmale"), "idle_right");
	Animation idleRightUp = idleRightUp.PushAnimation(config.child("suitmale"), "idle_right_up");
	Animation idleRightDown = idleRightDown.PushAnimation(config.child("suitmale"), "idle_right_down");
	Animation idleLeft = idleLeft.PushAnimation(config.child("suitmale"), "idle_left");
	Animation idleLeftUp = idleLeftUp.PushAnimation(config.child("suitmale"), "idle_right_up");
	Animation idleLeftDown = idleLeftDown.PushAnimation(config.child("suitmale"), "idle_right_down");

	Collider* collider = new Collider({ 0,0,100,100 }, COLLIDER_HERO, nullptr);

	tmpHero = new Hero(iMPoint{ pos.x, pos.y }, ENTITY_TYPE::HERO_MELEE, collider, walkLeft, walkLeftUp,
		walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightUp, idleRightDown, idleLeft,
		idleLeftUp, idleLeftDown, 1, 100, 1, 50, 1, 20, 20, 20, 20, 20, 20, 20, 20, 20, 15, 15, 15);

	//AddEntity(ENTITY_TYPE::HERO_MELEE, pos.x, pos.y);

	return ret;
}


// Called before the first frame
bool ModuleEntityManager::Start()
{
	bool ret = true;

	texture = app->tex->Load("spritesheets/characters/suitmale.png");

	return ret;
}

// Called each loop iteration
bool ModuleEntityManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Entity Manager Pre-Update", Profiler::Color::Blue)

	CheckListener();

	CheckIfStarted();

	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		entityVector[i]->PreUpdate(dt);
	}

	return true;
}

void ModuleEntityManager::CheckIfStarted() {

	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		if (entityVector[i]->started == false)
			entityVector[i]->Start(texture);
	}
}


// Called each loop iteration
bool ModuleEntityManager::Update(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::Blue)

		bool ret = true;

	CheckListener();

	int numEntities = entityVector.size();
	float posX;
	float posY;

	for (int i = 0; i < numEntities; i++)
	{
		posX = entityVector[i]->GetPosition().x;
		posY = entityVector[i]->GetPosition().y;

		if (app->map->InsideCamera(posX, posY) == true){
			
			assert((int)ENTITY_TYPE::MAX_TYPE == 13);
			switch (entityVector[i]->GetType())
			{
			case ENTITY_TYPE::BUILDING:
			case ENTITY_TYPE::BLDG_BARRICADE:
			case ENTITY_TYPE::BLDG_BASE:
			case ENTITY_TYPE::BLDG_TURRET:
			case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
				buildingVector.push_back(entityVector[i]);
				break;
			case ENTITY_TYPE::ENEMY:
			case ENTITY_TYPE::HERO_GATHERER:
			case ENTITY_TYPE::HERO_MELEE:
			case ENTITY_TYPE::HERO_RANGED:
				movableEntitiesVector.push_back(entityVector[i]);
				break;
			}
		}
	}
	return ret;
}

// Called each loop iteration
bool ModuleEntityManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::Blue)

		bool ret = true;

	int numEntities = entityVector.size();
	float posX;
	float posY;

	for (int i = 0; i < numEntities; i++)
	{
		posX = entityVector[i]->GetPosition().x;
		posY = entityVector[i]->GetPosition().y;

		if (app->map->InsideCamera(posX, posY) == true)
		{
			renderVector.push_back(entityVector[i]);
		}
	}

	numEntities = renderVector.size();

	// SORTING
	
	for (int i = 0; i < numEntities; i++)
	{
		renderVector[i]->PostUpdate(dt);
	}

	renderVector.clear();

	RemoveDeletedEntitys();

	return ret;
}

//// Called before quitting
bool ModuleEntityManager::CleanUp()
{
	int numEntities = entityVector.size();


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
	if (c1->thisEntity != nullptr)
	{
		c1->thisEntity->OnCollision(c2);
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

		/*Hero* tmpHero = new Hero(SDL_Point{ x,y }, ENTITY_TYPE::HERO_MELEE, { 0,0,100,100 }, COLLIDER_HERO, this, animation, 1, 100, 1, 50, 1, 20, 20, 20, 20, 20, 20, 20, 20, 20);
		heroVector.push_back(tmpHero);
		ret = tmpHero;*/
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
		entityVector.push_back(ret);
	}

	return ret;
}


// Checks if there is an entity in the mouse Click position 
Entity* ModuleEntityManager::CheckEntityOnClick(iMPoint mousePos)
{
	int numEntitys = entityVector.size();

	Collider* col;

	for (int i = 0; i < numEntitys; i++)
	{
		col = entityVector[i]->GetCollider();

		if (col != nullptr)
		{
			if (mousePos.PointInRect(&col->rect))
			{
				return entityVector[i];
			}
		}
	}

	return nullptr;
}


void ModuleEntityManager::CheckHeroOnSelection(SDL_Rect& selection, std::vector<Hero*>* heroPlayerVector)
{
	int numHeroes = heroVector.size();

	heroPlayerVector->clear();

	Collider* col;

	for (int i = 0; i < numHeroes; i++)
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


bool ModuleEntityManager::CheckEntityExists(Entity* entity)
{
	int numEntitys = entityVector.size();

	for (int i = 0; i < numEntitys; i++)
	{
		if (entityVector[i] == entity)
		{
			return true;
		}
	}

	return false;
}


void ModuleEntityManager::RemoveDeletedEntitys()
{
	int numEntitys = entityVector.size();

	for (int i = 0; i < numEntitys; i++)
	{
		if (entityVector[i]->toDelete == true)
		{
			delete entityVector[i];
			entityVector.erase(entityVector.begin() + i);
			entityVector[i] = nullptr;
		}
	}

}

void ModuleEntityManager::ExecuteEvent(EVENT_ENUM& eventId) const
{}