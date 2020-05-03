#include "App.h"
#include "AI.h"
#include "EventManager.h"
#include "Collision.h"
#include "Base.h"
#include "Spawner.h"
#include "TestScene.h"
#include "Pathfinding.h"
#include "Map.h"

ModuleAI::ModuleAI() : Module()
{
	name.create("AI");
}


ModuleAI::~ModuleAI()
{}



bool ModuleAI::Start()
{
	app->eventManager->EventRegister(EVENT_ENUM::NIGHT_START, this);
	app->eventManager->EventRegister(EVENT_ENUM::DAY_START, this);
	app->eventManager->EventRegister(EVENT_ENUM::ENEMY_CONQUERED_A_BASE, this);

	return true;
}


bool ModuleAI::PostUpdate(float dt)
{
	CheckListener(this);

	return true;
}


bool ModuleAI::CleanUp()
{
	app->eventManager->EventUnRegister(EVENT_ENUM::NIGHT_START, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::DAY_START, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ENEMY_CONQUERED_A_BASE, this);

	baseVector.clear();
	spawnerVector.clear();

	return true;
}


void ModuleAI::OnCollision(Collider* c1, Collider* c2)
{
	
	if (c1->type == COLLIDER_BASE_ALERT && c2->type == COLLIDER_HERO)
	{


		fMPoint c1Pos = c1->thisEntity->position;
		fMPoint c2Pos = c2->thisEntity->position;

		bool isVisible = app->pathfinding->LineRayCast(app->map->WorldToMap(c1Pos.x, c1Pos.y), app->map->WorldToMap(c2Pos.x, c2Pos.y));

		if (c2->thisEntity)
		{
			CreateSelectionCollider(c1);
			objectivePos = c2->thisEntity->GetPosition();
		}
	}

}


void ModuleAI::CreateSelectionCollider(Collider* collider)
{
	collider->active = false;
	Collider* col = app->coll->AddCollider(collider->rect, COLLIDER_RECLUIT_IA, this);
	col->to_delete = true;
}


fMPoint* ModuleAI::GetObjective()
{
	if (objectivePos != fMPoint{ INT_MIN, INT_MIN })
	{
		return &objectivePos;
	}

	return nullptr;
}


void ModuleAI::ExecuteEvent(EVENT_ENUM eventId)
{
	int base = 0;

	switch (eventId)
	{
	case EVENT_ENUM::NIGHT_START:

		base = CheckBaseAligmentAttack();

		if (base != -1) //-1 means no player controlled bases were found
		{
			objectivePos = baseVector[base]->GetPosition() + baseVector[base]->GetCenter();

			//call nearest spawner and spawn x number of monsters
			CommandSpawners();
		}

		break;


	case EVENT_ENUM::DAY_START:
		objectivePos = { NULL, NULL };

		break;


	case EVENT_ENUM::ENEMY_CONQUERED_A_BASE:
		objectivePos = { NULL, NULL };
		break;
	}

}


int ModuleAI::CheckBaseAligmentAttack()
{
	int numBases = baseVector.size();


	for (int i = numBases - 1; i >= 0; i--)
	{
		if (ENTITY_ALIGNEMENT::PLAYER == baseVector[i]->GetAlignment())
		{
			return i;
		}
	}

	return -1;
}


void ModuleAI::PushBase(Base* building)
{
	baseVector.push_back(building);
}


void ModuleAI::PushSpawner(Spawner* building)
{
	spawnerVector.push_back(building);
}


void ModuleAI::CommandSpawners()
{
	std::multimap<int, Spawner*> spawners;
	FindNearestSpawners(&spawners);
	int spawnersAbaliable = spawners.size();

	int spawnersToActivate = CalculateSpawnersToActivate();
	int enemiesToSpawn = CalculateEnemiesToSpawn(spawnersToActivate);

	std::multimap<int, Spawner*>::iterator iterator = spawners.begin();

	for (int i = 0; i < spawnersToActivate && i < spawnersAbaliable; i++)
	{
		iterator->second->SetNumberToSpawn(enemiesToSpawn);
		iterator++;
	}

	spawners.clear();
}


void ModuleAI::FindNearestSpawners(std::multimap<int, Spawner*>* spawners)
{
	int numSpawners = spawnerVector.size();

	fMPoint pos;

	for (int i = 0; i < numSpawners; i++)
	{
		pos = spawnerVector[i]->GetPosition();
		spawners->insert(std::pair< int, Spawner*>(pos.DistanceNoSqrt(objectivePos), spawnerVector[i]));
	}
}


int ModuleAI::CalculateEnemiesToSpawn(int numberOfSpawners)
{
	int days = app->testScene->GetDayNumber();

	int ret = days * ENEMIES_PER_NIGHT / numberOfSpawners;
	return ret;
}


int ModuleAI::CalculateSpawnersToActivate()
{
	return SPAWNERS_TO_ACTIVATE;
}



void ModuleAI::ResetAI()
{
	baseVector.clear();
	spawnerVector.clear();
}