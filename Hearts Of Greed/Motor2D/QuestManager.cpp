#include "App.h"
#include "QuestManager.h"
#include "EventManager.h"
#include "Collision.h"


ModuleQuestManager::ModuleQuestManager()
{
	name.create("QuestManager");
}

ModuleQuestManager::~ModuleQuestManager()
{}

bool ModuleQuestManager::Awake(pugi::xml_node&)
{
	bool ret = true;

	return ret;
}

bool ModuleQuestManager::Start()
{
	bool ret = true;

	app->eventManager->EventRegister(EVENT_ENUM::START_QUEST, this);
	app->eventManager->EventRegister(EVENT_ENUM::FINISH_QUEST, this);

	return ret;
}

bool ModuleQuestManager::PreUpdate(float dt)
{
	bool ret = true;

	return ret;
}

bool ModuleQuestManager::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool ModuleQuestManager::PostUpdate(float dt)
{
	bool ret = true;

	return ret;
}

bool ModuleQuestManager::CleanUp()
{
	bool ret = true;

	for (std::vector<Quest*>::iterator it = ongoing.begin(); it != ongoing.end(); it++)
		ongoing.erase(it);

	for (std::vector<Quest*>::iterator it = finished.begin(); it != finished.end(); it++)
		finished.erase(it);

	return ret;
}


void ModuleQuestManager::AddQuest(Collider* activationCol)
{

	int id = ongoing.size() + finished.size();

	Quest* newQuest = new Quest();

	newQuest->myState = QUEST_STATE::ACTIVE;

	ongoing.push_back(newQuest);
}

void ModuleQuestManager::ExecuteEvent(EVENT_ENUM eventId)
{
	switch (eventId)
	{

	case EVENT_ENUM::START_QUEST:

		break;

	case EVENT_ENUM::FINISH_QUEST:

		break;
	}
}


