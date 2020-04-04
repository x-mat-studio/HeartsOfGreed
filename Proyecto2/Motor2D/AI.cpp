#include "App.h"
#include "AI.h"
#include "EventManager.h"
#include "Collision.h"
#include "Building.h"

ModuleAI::ModuleAI() : Module()
{
	name.create("player");
}


ModuleAI::~ModuleAI()
{}


bool ModuleAI::Awake(pugi::xml_node& config)
{
	app->eventManager->EventRegister(EVENT_ENUM::NIGHT_START, this);
	app->eventManager->EventRegister(EVENT_ENUM::DAY_START, this);
	app->eventManager->EventRegister(EVENT_ENUM::ENEMY_CONQUERED_A_BASE, this);

	return true;
}


void ModuleAI::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_BASE_ALERT && c2->type == COLLIDER_HERO)
	{
		CreateSelectionCollider(c1);
		objectivePos.x = c2->rect.x;
		objectivePos.y = c2->rect.y;
	}
}


void ModuleAI::CreateSelectionCollider(Collider* collider)
{
	app->coll->AddCollider(collider->rect, COLLIDER_RECLUIT_IA, this);
}


fMPoint* ModuleAI::GetObjective()
{
	if (objectivePos != fMPoint{NULL, NULL})
	{
		return &objectivePos;
	}

	return nullptr;
}


void ModuleAI::ExecuteEvent(EVENT_ENUM eventId)
{
	switch (eventId)
	{
	case EVENT_ENUM::NIGHT_START:

		int base = CheckBaseAligmentAttack();

		if (base != -1) //-1 means no player controlled bases were found
		{
			objectivePos = baseVector[base]->GetPosition();

			//call random spawners and spawning x number of monsters
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
	int i = 0;

	for (i; i < numBases; i++)
	{
		if (ENTITY_ALIGNEMENT::PLAYER == baseVector[i]->GetAlignment())
		{
			return i;
		}
	}

	return -1;
}


void ModuleAI::PushBase(Building* building)
{
	baseVector.push_back(building);
}