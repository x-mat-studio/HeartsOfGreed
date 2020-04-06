#include "App.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "Textures.h"
#include "Entity.h"
#include "Map.h"
#include "Collision.h"
#include "AI.h"

#include "DynamicEntity.h"
#include "GathererHero.h"
#include "MeleeHero.h"
#include "RangedHero.h"
#include "Enemy.h"

#include "Spawner.h"

#include "Building.h"

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


	// Sample Hero Melee---------------------
	fMPoint pos;
	pos.create(100, 600);

	P2SString filename = config.child("load").attribute("docnameSuitman").as_string();
	pugi::xml_document suitmandoc;
	suitmandoc.load_file(filename.GetString());
	pugi::xml_node suitman = suitmandoc.child("suitman");

	Animation walkLeft = walkLeft.PushAnimation(suitman, "walk_left");
	Animation walkLeftUp = walkLeftUp.PushAnimation(suitman, "walk_left_up");
	Animation walkLeftDown = walkLeftDown.PushAnimation(suitman, "walk_left_down");
	Animation walkRightUp = walkRightUp.PushAnimation(suitman, "walk_right_up");
	Animation walkRightDown = walkRightDown.PushAnimation(suitman, "walk_right_down");
	Animation walkRight = walkRight.PushAnimation(suitman, "walk_right");

	Animation idleRight = idleRight.PushAnimation(suitman, "idle_right");
	Animation idleRightUp = idleRightUp.PushAnimation(suitman, "idle_right_up");
	Animation idleRightDown = idleRightDown.PushAnimation(suitman, "idle_right_down");
	Animation idleLeft = idleLeft.PushAnimation(suitman, "idle_left");
	Animation idleLeftUp = idleLeftUp.PushAnimation(suitman, "idle_left_up");
	Animation idleLeftDown = idleLeftDown.PushAnimation(suitman, "idle_left_down");


	// Hero collider
	Collider* collider = new Collider({ 0,0,30,65 }, COLLIDER_HERO, this);
	sampleMelee = new Hero(fMPoint{ pos.x, pos.y }, ENTITY_TYPE::HERO_GATHERER, collider, walkLeft, walkLeftUp,
		walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightUp, idleRightDown, idleLeft,
		idleLeftUp, idleLeftDown, 1, 100, 1, 50, 1, 20, 20, 100, 20, 20, 20, 20, 20, 20, 15, 15, 15);


	// Sample Enemy---------------------
	filename = config.child("load").attribute("docnameWanamingo").as_string();
	pugi::xml_document wanamingodoc;
	wanamingodoc.load_file(filename.GetString());
	pugi::xml_node wanamingo = wanamingodoc.child("wanamingo");

	Animation enemyWalkLeft = enemyWalkLeft.PushAnimation(wanamingo, "wanamingoLeftWalk"); // looks good
	Animation enemyWalkLeftUp = enemyWalkLeftUp.PushAnimation(wanamingo, "wanamingoUpLeftWalk");// looks good
	Animation enemyWalkLeftDown = enemyWalkLeftDown.PushAnimation(wanamingo, "wanamingoDownLeftWalk"); // last frame teleports to the left
	Animation enemyWalkRightUp = enemyWalkRightUp.PushAnimation(wanamingo, "wanamingoUpRightWalk"); // looks good
	Animation enemyWalkRightDown = enemyWalkRightDown.PushAnimation(wanamingo, "wanamingoDownRightWalk"); // looks good
	Animation enemyWalkRight = enemyWalkRight.PushAnimation(wanamingo, "wanamingoRightWalk");// looks good

	Animation enemyIdleRight = enemyIdleRight.PushAnimation(wanamingo, "wanamingoRightIdle"); //goes up then bumps right
	Animation enemyIdleRightUp = enemyIdleRightUp.PushAnimation(wanamingo, "wanamingoUpRightIdle"); //bumps left
	Animation enemyIdleRightDown = enemyIdleRightDown.PushAnimation(wanamingo, "wanamingoDownRightIdle"); //bumps right
	Animation enemyIdleLeft = enemyIdleLeft.PushAnimation(wanamingo, "wanamingoLeftIdle"); //bumps left
	Animation enemyIdleLeftUp = enemyIdleLeftUp.PushAnimation(wanamingo, "wanamingoUpLeftIdle"); //bumps right
	Animation enemyIdleLeftDown = enemyIdleLeftDown.PushAnimation(wanamingo, "wanamingoDownLeftIdle"); //bumps right

	Animation enemyPunchRight = enemyPunchRight.PushAnimation(wanamingo, "wanamingoRightPunch"); //looks good
	Animation enemyPunchRightUp = enemyPunchRightUp.PushAnimation(wanamingo, "wanamingoUpRightPunch"); //jesus christ 
	Animation enemyPunchRightDown = enemyPunchRightDown.PushAnimation(wanamingo, "wanamingoDownRightPunch"); //goes back and forth
	Animation enemyPunchLeft = enemyPunchLeft.PushAnimation(wanamingo, "wanamingoLeftPunch"); //It should bump to the other side!
	Animation enemyPunchLeftUp = enemyPunchLeftUp.PushAnimation(wanamingo, "wanamingoUpLeftPunch"); //jesus christ 
	Animation enemyPunchLeftDown = enemyPunchLeftDown.PushAnimation(wanamingo, "wanamingoDownLeftPunch"); //jesus christ 


	//Enemy collider and spawner
	Collider* enemyCollider = new Collider({ 0,0,50,50 }, COLLIDER_ENEMY, this);
	sampleEnemy = new Enemy(fMPoint{ 150, 250 }, ENTITY_TYPE::ENEMY, enemyCollider, enemyWalkRightDown, 5, 0, 250, 1, 120, 25, 5, 0);
	sampleSpawner = new Spawner(fMPoint{ 150, 250 }, ENTITY_TYPE::ENEMY);

	//Test building
	Collider* buildingCollider = new Collider({ -150,130,350,280 }, COLLIDER_VISIBILITY, this);
	testBuilding = new Building(fMPoint{ 0,0 }, 100, 100, 100, 100, 100, buildingCollider);


	return ret;
}



bool ModuleEntityManager::Start()
{
	bool ret = true;

	suitManTexture = app->tex->Load("spritesheets/characters/suitmale.png");
	armorMaleTexture = app->tex->Load("spritesheets/characters/armormale.png");
	combatFemaleTexture = app->tex->Load("spritesheets/characters/combatfemale.png");

	enemyTexture = app->tex->Load("spritesheets/Enemies/WanamingoAlien.png");

	buildingTexture = app->tex->Load("maps/base03.png");
	base1Texture = app->tex->Load("maps/base01.png");
	base2Texture = app->tex->Load("maps/base02.png");

	debugPathTexture = app->tex->Load("maps/path.png");

	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_DEAD, this);

	testBuilding->SetTexture(base1Texture);
	
	return ret;
}


bool ModuleEntityManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Entity Manager Pre-Update", Profiler::Color::Blue)

		CheckListener(this);

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
		if (entityVector[i]->started == false) {

			switch (entityVector[i]->GetType())
			{
			case ENTITY_TYPE::PARTICLE:
				break;

			case ENTITY_TYPE::HERO_MELEE:
				entityVector[i]->Start(armorMaleTexture);
				app->eventManager->GenerateEvent(EVENT_ENUM::HERO_MELEE_CREATED, EVENT_ENUM::NULL_EVENT);
				break;

			case ENTITY_TYPE::HERO_RANGED:
				entityVector[i]->Start(combatFemaleTexture);
				app->eventManager->GenerateEvent(EVENT_ENUM::HERO_RANGED_CREATED, EVENT_ENUM::NULL_EVENT);
				break;

			case ENTITY_TYPE::HERO_GATHERER:
				entityVector[i]->Start(suitManTexture);
				app->eventManager->GenerateEvent(EVENT_ENUM::HERO_GATHERER_CREATED, EVENT_ENUM::NULL_EVENT);
				break;

			case ENTITY_TYPE::ENEMY:
				entityVector[i]->Start(enemyTexture);
				break;

			case ENTITY_TYPE::BUILDING:
				entityVector[i]->Start(base1Texture);
				break;

			case ENTITY_TYPE::BLDG_TURRET:
				break;

			case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
				break;

			case ENTITY_TYPE::BLDG_BASE:
				break;

			case ENTITY_TYPE::BLDG_BARRICADE:
				break;

			default:
				entityVector[i]->started = true;
				break;
			}
		}
	}
}



bool ModuleEntityManager::Update(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::Blue)

		CheckListener(this);

	int numEntities = entityVector.size();


	for (int i = 0; i < numEntities; i++)
	{
		entityVector[i]->Update(dt);
	}

	return true;
}

// Called each loop iteration
bool ModuleEntityManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::Blue)

	int numEntities = entityVector.size();


	for (int i = 0; i < numEntities; i++)
	{
		entityVector[i]->PostUpdate(dt);
	}

	SpriteOrdering(dt);

	return true;
}


bool ModuleEntityManager::CleanUp()
{
	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		RELEASE(entityVector[i]);
		entityVector[i] = nullptr;
	}

	entityVector.clear();
	
	//destroy sample entites
	RELEASE(sampleMelee);
	RELEASE(sampleEnemy);
	sampleMelee = nullptr;
	sampleEnemy = nullptr;

	app->tex->UnLoad(suitManTexture);
	app->tex->UnLoad(buildingTexture);

	suitManTexture = nullptr;
	buildingTexture = nullptr;

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
Entity* ModuleEntityManager::AddEntity(ENTITY_TYPE type, int x, int y, ENTITY_ALIGNEMENT alignement)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case ENTITY_TYPE::SPAWNER:
		ret = new Spawner({ (float)x,(float)y }, sampleSpawner);
		app->ai->PushSpawner((Spawner*)ret);
		break;
	case ENTITY_TYPE::PARTICLE:
		break;

	case ENTITY_TYPE::EMITER:
		break;

	case ENTITY_TYPE::PARTICLE_SYSTEM:
		break;

	case ENTITY_TYPE::HERO_MELEE:
		ret = new Hero({ (float)x,(float)y }, sampleMelee, alignement);
		break;

	case ENTITY_TYPE::HERO_RANGED:
		break;

	case ENTITY_TYPE::HERO_GATHERER:
		break;

	case ENTITY_TYPE::BUILDING:
		ret = new Building({ (float)x,(float)y }, testBuilding, alignement);
		break;

	case ENTITY_TYPE::BLDG_TURRET:
		break;

	case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
		break;

	case ENTITY_TYPE::BLDG_BASE:

		app->ai->PushBase((Base*)ret);
		break;

	case ENTITY_TYPE::BLDG_BARRICADE:
		break;

	case ENTITY_TYPE::ENEMY:
		ret = new Enemy({ (float)x,(float)y }, sampleEnemy, alignement);
		break;

	default:
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
	int numHeroes = entityVector.size();

	heroPlayerVector->clear();

	Collider* col;

	for (int i = 0; i < numHeroes; i++)
	{
		if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_GATHERER || entityVector[i]->GetType() == ENTITY_TYPE::HERO_RANGED || entityVector[i]->GetType() == ENTITY_TYPE::HERO_MELEE)
		{
			col = entityVector[i]->GetCollider();

			if (col != nullptr)
			{
				if (col->CheckCollision(selection))
				{
					heroPlayerVector->push_back((Hero*)entityVector[i]);
				}
			}
		}
	}
}


void ModuleEntityManager::CheckDynamicEntitysObjectives(Entity* entity)
{
	int numEntitys = entityVector.size();

	ENTITY_TYPE type;

	for (int i = 0; i < numEntitys; i++)
	{
		type = entityVector[i]->GetType();

		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED)
		{
			Hero* hero = (Hero*)entityVector[i];
			hero->CheckObjecive(entity);
		}

		else if (type == ENTITY_TYPE::ENEMY)
		{
			Enemy* enemy = (Enemy*)entityVector[i];
			enemy->CheckObjecive(entity);
		}
	}


}


Entity* ModuleEntityManager::SearchEntityRect(SDL_Rect* rect, ENTITY_ALIGNEMENT alig)
{
	ENTITY_ALIGNEMENT alignementToSearch = ENTITY_ALIGNEMENT::UNKNOWN;
	ENTITY_ALIGNEMENT alignement;

	Collider* col;

	if (alig == ENTITY_ALIGNEMENT::ENEMY)
	{
		alignementToSearch = ENTITY_ALIGNEMENT::PLAYER;
	}

	else if (alig == ENTITY_ALIGNEMENT::PLAYER)
	{
		alignementToSearch = ENTITY_ALIGNEMENT::ENEMY;
	}


	int numEntitys = entityVector.size();

	for (int i = 0; i < numEntitys; i++)
	{
		alignement = entityVector[i]->GetAlignment();

		if (alignement != alignementToSearch) 
		{
			continue;
		}

		col = entityVector[i]->GetCollider();

		if (col != nullptr)
		{
			if (col->CheckCollision(*rect))
			{
				return entityVector[i];
			}
		}
	}
}


void ModuleEntityManager::RemoveDeletedEntitys()
{
	int numEntitys = entityVector.size();

	for (int i = 0; i < numEntitys; i++)
	{
		if (entityVector[i]->toDelete == true)
		{
			CheckDynamicEntitysObjectives(entityVector[i]);

			delete entityVector[i];
			entityVector.erase(entityVector.begin() + i);
			entityVector[i] = nullptr;
		}
	}

}


void ModuleEntityManager::SpriteOrdering(float dt)
{
	int numEntities = entityVector.size();

	float posX, posY, w, h;

	for (int i = 0; i < numEntities; i++)
	{
		posX = entityVector[i]->GetPosition().x;
		posY = entityVector[i]->GetPosition().y;
		w = entityVector[i]->GetCollider()->rect.w;
		h = entityVector[i]->GetCollider()->rect.h;

		if (app->map->EntityInsideCamera(posX, posY, w, h) == true) {

			assert((int)ENTITY_TYPE::MAX_TYPE == MAX_ENTITY_TYPES);

			switch (entityVector[i]->GetType())
			{
			case ENTITY_TYPE::BUILDING:
			case ENTITY_TYPE::BLDG_BARRICADE:
			case ENTITY_TYPE::BLDG_BASE:
			case ENTITY_TYPE::BLDG_TURRET:
			case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
				//case ENTITY_TYPE::BLDG_CORE:	CORE_CONSTRUCTOR_NEEDED
				buildingVector.push_back(entityVector[i]);
				break;
			case ENTITY_TYPE::ENEMY:
			case ENTITY_TYPE::HERO_GATHERER:
			case ENTITY_TYPE::HERO_MELEE:
			case ENTITY_TYPE::HERO_RANGED:
				movableEntityVector.push_back(entityVector[i]);
				break;

			default:
				break;
			}
		}
	}

	EntityQuickSort(movableEntityVector, 0, movableEntityVector.size());
	EntityQuickSort(buildingVector, 0, buildingVector.size());

	while (buildingVector.size() != 0 || movableEntityVector.size() != 0)
	{
		SPRITE_POSITION pivotEnum = SPRITE_POSITION::BOTH_NULL;

		if (buildingVector.size() == 0)
		{
			pivotEnum = SPRITE_POSITION::NULL_BUILDING;
		}

		else if (movableEntityVector.size() == 0)
		{
			pivotEnum = SPRITE_POSITION::NULL_MOVABLE_ENTITY;
		}

		else
		{
			pivotEnum = CheckSpriteHeight(movableEntityVector.front(), buildingVector.front());
		}

		switch (pivotEnum)
		{
		case SPRITE_POSITION::BEHIND_BUILDING:
		case SPRITE_POSITION::HIGHER_THAN_BUILDING:
		case SPRITE_POSITION::NULL_BUILDING:
			renderVector.push_back(movableEntityVector.front());
			movableEntityVector.erase(movableEntityVector.cbegin());
			break;
		case SPRITE_POSITION::LOWER_THAN_BUILDING:
		case SPRITE_POSITION::NULL_MOVABLE_ENTITY:
			renderVector.push_back(buildingVector.front());
			buildingVector.erase(buildingVector.cbegin());
			break;
		case SPRITE_POSITION::BOTH_NULL:
			break;
		}

	}

	numEntities = renderVector.size();


	for (int i = 0; i < numEntities; i++)
	{
		if (renderVector[i]->visionEntity != nullptr)
		{
			if (renderVector[i]->visionEntity->isVisible)
			{
				renderVector[i]->Draw(dt);
			}
		}
		else
		{
			renderVector[i]->Draw(dt);
		}
	}

	renderVector.clear();

}


void ModuleEntityManager::EntityQuickSort(std::vector<Entity*>& vector, int low, int high)
{
	if (low < high)
	{
		int numElem = vector.size();
		int pivotLocation = EntityPartition(vector, low, high);
		EntityQuickSort(vector, low, pivotLocation);
		EntityQuickSort(vector, pivotLocation + 1, high);
	}
}


int ModuleEntityManager::EntityPartition(std::vector<Entity*>& vector, int low, int high)
{
	Entity* pivot = vector[low];
	Entity* auxVec = nullptr;
	int left = low;

	for (int i = low + 1; i < high; i++)
	{
		if (vector[i]->GetPosition().y + (float)vector[i]->GetCollider()->rect.h <= pivot->GetPosition().y + (float)pivot->GetCollider()->rect.h)
		{
			auxVec = vector[i];
			vector[i] = vector[left];
			vector[left] = auxVec;
			left++;
		}
	}

	auxVec = pivot;
	pivot = vector[left];

	return left;
}


void ModuleEntityManager::ExecuteEvent(EVENT_ENUM eventId)
{
	switch (eventId)
	{
	case EVENT_ENUM::ENTITY_DEAD:
		RemoveDeletedEntitys();
		break;
	}

}


void ModuleEntityManager::GetEntityNeighbours(std::vector<DynamicEntity*>* close_entity_list, std::vector<DynamicEntity*>* colliding_entity_list, DynamicEntity* thisUnit)
{
	close_entity_list->clear();
	colliding_entity_list->clear();

	DynamicEntity* it;

	for (int i = 0; i < entityVector.size(); ++i)
	{
		if (!entityVector[i]->dynamic)
		{
			continue;
		}
		else
		{
			it = (DynamicEntity*)entityVector[i];

			if (it != thisUnit && it->GetAlignment() == thisUnit->GetAlignment())
			{
				fMPoint pos = it->GetPosition();

				float distance = pos.DistanceTo(thisUnit->GetPosition());
				if (distance < it->moveRange2)
				{
					colliding_entity_list->push_back(it);

				}
				if (distance < thisUnit->moveRange1)
				{
					close_entity_list->push_back(it);
				}
			}
		}
	}

}


SPRITE_POSITION ModuleEntityManager::CheckSpriteHeight(Entity* movEntity, Entity* building) const
{
	if (movEntity->GetPosition().y + movEntity->GetCollider()->rect.h < building->GetPosition().y)
	{
		return SPRITE_POSITION::HIGHER_THAN_BUILDING;
	}

	else if ((movEntity->GetPosition().y < building->GetPosition().y && movEntity->GetPosition().y + movEntity->GetCollider()->rect.h > building->GetPosition().y)
		|| (movEntity->GetPosition().y > building->GetPosition().y&& movEntity->GetPosition().y + movEntity->GetCollider()->rect.h < building->GetPosition().y + building->GetCollider()->rect.h))
	{
		return SPRITE_POSITION::BEHIND_BUILDING;
	}

	else
	{
		return SPRITE_POSITION::LOWER_THAN_BUILDING;
	}
}


void ModuleEntityManager::PlayerBuildPreview(int x, int y, ENTITY_TYPE type)
{
	switch (type)
	{
	case ENTITY_TYPE::BUILDING:
		testBuilding->ActivateTransparency();
		testBuilding->SetPosition(x, y);
		testBuilding->Draw(0);
		break;
	case ENTITY_TYPE::BLDG_TURRET:
		break;
	case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
		break;
	case ENTITY_TYPE::BLDG_BASE:
		break;
	case ENTITY_TYPE::BLDG_BARRICADE:
		break;

	default:

		break;
	}

}