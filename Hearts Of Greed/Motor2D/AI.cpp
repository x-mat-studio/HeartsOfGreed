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



bool ModuleAI::Awake(pugi::xml_node& data)
{
	//Load required doc

	P2SString filename = data.child("load").attribute("docnameNightSpawners").as_string();
	pugi::xml_document spwnDoc;
	spwnDoc.load_file(filename.GetString());
	pugi::xml_node spawnNight = spwnDoc.child("nightSpawn");

	//The nights
	std::vector<int> NightAux;
	
		//Loading nights

	//Remember, these numer codify for: (In order)	 wanamingo - gigamingo - snipermingo - speedomingo - nº of spawners activated that night

	//Night 1
	NightAux.push_back(		(int)spawnNight.child("night1").child("wana").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night1").child("giga").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night1").child("sniper").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night1").child("speed").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night1").child("spawners").attribute("number").as_int());
	nightEnemyInfo.push_back(NightAux);
	NightAux.clear();


	//Night 2
	NightAux.push_back(		(int)spawnNight.child("night2").child("wana").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night2").child("giga").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night2").child("sniper").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night2").child("speed").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night1").child("spawners").attribute("number").as_int());
	nightEnemyInfo.push_back(NightAux);
	NightAux.clear();
	
	//Night 3
	NightAux.push_back(		(int)spawnNight.child("night3").child("wana").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night3").child("giga").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night3").child("sniper").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night3").child("speed").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night1").child("spawners").attribute("number").as_int());
	nightEnemyInfo.push_back(NightAux);
	NightAux.clear();
	
	//Night 4
	NightAux.push_back(		(int)spawnNight.child("night4").child("wana").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night4").child("giga").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night4").child("sniper").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night4").child("speed").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night1").child("spawners").attribute("number").as_int());
	nightEnemyInfo.push_back(NightAux);
	NightAux.clear();
	
	//Night 5
	NightAux.push_back(		(int)spawnNight.child("night5").child("wana").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night5").child("giga").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night5").child("sniper").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night5").child("speed").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night1").child("spawners").attribute("number").as_int());
	nightEnemyInfo.push_back(NightAux);
	NightAux.clear();
	
	//Night 6
	NightAux.push_back(		(int)spawnNight.child("night6").child("wana").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night6").child("giga").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night6").child("sniper").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night6").child("speed").attribute("number").as_int());
	NightAux.push_back(		(int)spawnNight.child("night1").child("spawners").attribute("number").as_int());
	nightEnemyInfo.push_back(NightAux);
	NightAux.clear();

	spwnDoc.reset();
	return true;
}

bool ModuleAI::Start()
{
	app->eventManager->EventRegister(EVENT_ENUM::NIGHT_START, this);
	app->eventManager->EventRegister(EVENT_ENUM::DAY_START, this);
	app->eventManager->EventRegister(EVENT_ENUM::ENEMY_CONQUERED_A_BASE, this);
	app->eventManager->EventRegister(EVENT_ENUM::PLAYER_CONQUERED_A_BASE, this);

	return true;
}


bool ModuleAI::PostUpdate(float dt)
{
	alarmObjectivePos.clear();
	CheckListener(this);

	return true;
}


bool ModuleAI::CleanUp()
{
	app->eventManager->EventUnRegister(EVENT_ENUM::NIGHT_START, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::DAY_START, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ENEMY_CONQUERED_A_BASE, this);
	app->eventManager->EventRegister(EVENT_ENUM::PLAYER_CONQUERED_A_BASE, this);

	baseVector.clear();
	spawnerVector.clear();
	nightEnemyInfo.clear();

	return true;
}


void ModuleAI::OnCollision(Collider* c1, Collider* c2)
{

	if (c1->type == COLLIDER_BASE_ALERT && c2->type == COLLIDER_HERO)
	{
		if (!c1->thisEntity || !c2->thisEntity)
			return;

		fMPoint c1Pos = c1->thisEntity->position;
		fMPoint c2Pos = c2->thisEntity->position;

		bool isVisible = app->pathfinding->LineRayCast(app->map->WorldToMap(c1Pos.x, c1Pos.y), app->map->WorldToMap(c2Pos.x, c2Pos.y));

		if (c2->thisEntity)
		{
			alarmObjectivePos.push_back(c2->thisEntity->GetPosition());
			CreateSelectionCollider(c1);
		}
	}

}


void ModuleAI::CreateSelectionCollider(Collider* collider)
{
	collider->active = false;
	Collider* col = app->coll->AddCollider(collider->rect, COLLIDER_RECLUIT_IA, this);
	col->to_delete = true;
}


fMPoint* ModuleAI::GetObjective(fMPoint& pos)
{
	fMPoint* ret = nullptr;

	if (alarmObjectivePos.empty() == false)
	{
		ret = &alarmObjectivePos[0];

		for (int i = 1; i < alarmObjectivePos.size(); i++)
		{
			if (pos.DistanceNoSqrt(*ret) > pos.DistanceNoSqrt(alarmObjectivePos[i]))
			{
				ret = &alarmObjectivePos[i];
			}
		}
	}

	if (objectivePos != fMPoint{ INT_MIN, INT_MIN })
	{
		if (ret != nullptr)
		{
			if (pos.DistanceNoSqrt(*ret) > pos.DistanceNoSqrt(objectivePos))
			{
				ret = &objectivePos;
			}
		}
		else
			return &objectivePos;
	}

	return ret;
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
		objectivePos = { INT_MIN, INT_MIN };

		break;


	case EVENT_ENUM::ENEMY_CONQUERED_A_BASE:
		CheckEndGame(ENTITY_ALIGNEMENT::ENEMY);
		break;

	case EVENT_ENUM::PLAYER_CONQUERED_A_BASE:
		CheckEndGame(ENTITY_ALIGNEMENT::PLAYER);
		break;
	}

}


int ModuleAI::CheckBaseAligmentAttack()
{
	int numBases = baseVector.size();

	std::vector<int> basesToRandom;
	int counter = 0;

	for (int i = numBases - 1; i >= 0; i--)
	{
		if (ENTITY_ALIGNEMENT::PLAYER == baseVector[i]->GetAlignment())
		{
			basesToRandom.push_back(i);
			counter++;
		}
	}
	
	if (counter == 0)
	{
		return -1;
	}

	else
	{
		return basesToRandom[rand() % counter];
	}
	
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
	int wanamingosToSpawn = CalculateWanamingoToSpawn();
	int gigamingosToSpawn = CalculateGigamingoToSpawn();
	int snipermingosToSpawn = CalculateSnipermingoToSpawn();
	int speedomingosToSpawn = CalculateSpeedomingoToSpawn();

	std::multimap<int, Spawner*>::iterator iterator = spawners.begin();

	AssignWana(spawners, iterator, wanamingosToSpawn, spawnersToActivate);
	AssignGiga(spawners, iterator, gigamingosToSpawn, spawnersToActivate);
	AssignSpeed(spawners, iterator, speedomingosToSpawn, spawnersToActivate);
	AssignSniper(spawners, iterator, snipermingosToSpawn, spawnersToActivate);


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

int ModuleAI::CalculateWanamingoToSpawn()
{
	int ret = 0;

	switch (app->testScene->GetDayNumber())
	{
	case 0:
		//no enemies on first day
		break;
	case 1:
		ret = nightEnemyInfo[0].at(0);
		break;
	case 2:
		ret = nightEnemyInfo[1].at(0);
		break;
	case 3:
		ret = nightEnemyInfo[2].at(0);
		break;
	case 4:
		ret = nightEnemyInfo[3].at(0);
		break;
	case 5:
		ret = nightEnemyInfo[4].at(0);
		break;
	case 6:
		ret = nightEnemyInfo[5].at(0);
		break;
	default:
		ret = app->testScene->GetDayNumber()*2;
		break;
	}

	return ret;
}

int ModuleAI::CalculateGigamingoToSpawn()
{
	int ret = 0;

	switch (app->testScene->GetDayNumber())
	{
	case 0:
		//no enemies on first day
		break;
	case 1:
		ret = nightEnemyInfo[0].at(1);
		break;
	case 2:
		ret = nightEnemyInfo[1].at(1);
		break;
	case 3:
		ret = nightEnemyInfo[2].at(1);
		break;
	case 4:
		ret = nightEnemyInfo[3].at(1);
		break;
	case 5:
		ret = nightEnemyInfo[4].at(1);
		break;
	case 6:
		ret = nightEnemyInfo[5].at(1);
		break;
	default:
		ret = app->testScene->GetDayNumber();
		break;
	}

	return ret;
}


int ModuleAI::CalculateSnipermingoToSpawn()
{
	int ret = 0;

	switch (app->testScene->GetDayNumber())
	{
	case 0:
		//no enemies on first day
		break;
	case 1:
		ret = nightEnemyInfo[0].at(2);
		break;
	case 2:
		ret = nightEnemyInfo[1].at(2);
		break;
	case 3:
		ret = nightEnemyInfo[2].at(2);
		break;
	case 4:
		ret = nightEnemyInfo[3].at(2);
		break;
	case 5:
		ret = nightEnemyInfo[4].at(2);
		break;
	case 6:
		ret = nightEnemyInfo[5].at(2);
		break;
	default:
		ret = app->testScene->GetDayNumber();
		break;
	}

	return ret;
}

int ModuleAI::CalculateSpeedomingoToSpawn()
{
	int ret = 0;

	switch (app->testScene->GetDayNumber())
	{
	case 0:
		//no enemies on first day
		break;
	case 1:
		ret = nightEnemyInfo[0].at(3);
		break;
	case 2:
		ret = nightEnemyInfo[1].at(3);
		break;
	case 3:
		ret = nightEnemyInfo[2].at(3);
		break;
	case 4:
		ret = nightEnemyInfo[3].at(3);
		break;
	case 5:
		ret = nightEnemyInfo[4].at(3);
		break;
	case 6:
		ret = nightEnemyInfo[5].at(3);
		break;
	default:
		ret = app->testScene->GetDayNumber()*3;
		break;
	}

	return ret;
}

void ModuleAI::AssignWana(std::multimap<int, Spawner*> spawners, std::multimap<int, Spawner*>::iterator iterator, int wanamingosToSpawn, int spawnersToActivate)
{
	while (wanamingosToSpawn > 0) {

		int aux = rand() % spawnersToActivate;

		iterator = spawners.begin();

		for (int i = 0; i < aux; i++) {

			iterator++; //This does not please the nut
		}

		iterator->second->AddAWanamingo();
		wanamingosToSpawn--;
	}
}

void ModuleAI::AssignGiga(std::multimap<int, Spawner*> spawners, std::multimap<int, Spawner*>::iterator iterator, int gigamingosToSpawn, int spawnersToActivate)
{
	while (gigamingosToSpawn > 0) {

		int aux = rand() % spawnersToActivate;

		iterator = spawners.begin();

		for (int i = 0; i < aux; i++) {

			iterator++; //This does not please the nut
		}

		iterator->second->AddAGigamingo();
		gigamingosToSpawn--;
	}
}

void ModuleAI::AssignSpeed(std::multimap<int, Spawner*> spawners, std::multimap<int, Spawner*>::iterator iterator, int speedomingosToSpawn, int spawnersToActivate)
{
	while (speedomingosToSpawn > 0) {

		int aux = rand() % spawnersToActivate;

		iterator = spawners.begin();

		for (int i = 0; i < aux; i++) {

			iterator++; //This does not please the nut
		}

		iterator->second->AddASpeedamingo();
		speedomingosToSpawn--;
	}
}

void ModuleAI::AssignSniper(std::multimap<int, Spawner*> spawners, std::multimap<int, Spawner*>::iterator iterator, int snipermingosToSpawn, int spawnersToActivate)
{
	while (snipermingosToSpawn > 0) {

		int aux = rand() % spawnersToActivate;

		iterator = spawners.begin();

		for (int i = 0; i < aux; i++) {

			iterator++; //This does not please the nut
		}

		iterator->second->AddASnipermingo();
		snipermingosToSpawn--;
	}
}

int ModuleAI::CalculateSpawnersToActivate()
{
	int ret = 0;

	switch (app->testScene->GetDayNumber())
	{
	case 0:
		//no enemies on first day
		break;
	case 1:
		ret = nightEnemyInfo[0].at(Nº_TYPE_OF_ENEMIES);
		break;
	case 2:
		ret = nightEnemyInfo[1].at(Nº_TYPE_OF_ENEMIES);
		break;
	case 3:
		ret = nightEnemyInfo[2].at(Nº_TYPE_OF_ENEMIES);
		break;
	case 4:
		ret = nightEnemyInfo[3].at(Nº_TYPE_OF_ENEMIES);
		break;
	case 5:
		ret = nightEnemyInfo[4].at(Nº_TYPE_OF_ENEMIES);
		break;
	case 6:
		ret = nightEnemyInfo[5].at(Nº_TYPE_OF_ENEMIES);
		break;
	default:
		int x = app->testScene->GetDayNumber() * 2;
		ret = (x < MAX_Nº_SPAWNERS) ? x : MAX_Nº_SPAWNERS;
		break;
	}


	return ret;
}



void ModuleAI::ResetAI()
{
	baseVector.clear();
	spawnerVector.clear();
}


void ModuleAI::CheckEndGame(ENTITY_ALIGNEMENT align)
{
	int numBases = baseVector.size();

	for (int i = 0; i < numBases; i++)
	{
		if (baseVector[i]->GetAlignment() != align)
		{
			return;
		}
	}

	if (align == ENTITY_ALIGNEMENT::PLAYER)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::GAME_WIN, EVENT_ENUM::NULL_EVENT);
	}

	else if(align == ENTITY_ALIGNEMENT::ENEMY)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::GAME_LOSE, EVENT_ENUM::NULL_EVENT);
	}
}