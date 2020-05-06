#include "App.h"
#include "QuestManager.h"
#include "EventManager.h"
#include "Collision.h"
#include "Audio.h"


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

	ret = app->eventManager->EventRegister(EVENT_ENUM::FINISH_QUEST, this);

	questSfx = app->audio->LoadFx("audio/sfx/Interface/questDone.wav");

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

	CheckListener(this);

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

	
	//for (std::vector<Quest*>::iterator it = ongoing.begin(); it != ongoing.end(); it++)
	//	ongoing.erase(it);

	//for (std::vector<Quest*>::iterator it = finished.begin(); it != finished.end(); it++)
	//	finished.erase(it);
	
	return ret;
}

void ModuleQuestManager::OnCollision(Collider* c1, Collider* c2)
{
	
	if (c1->thisEntity != nullptr && c1->type == COLLIDER_TYPE::COLLIDER_QUEST) 
	{
		
		c1->thisEntity->OnCollision(c2);
	} 
	else if (c2->thisEntity != nullptr && c2->type == COLLIDER_TYPE::COLLIDER_QUEST)
	{
	
		c2->thisEntity->OnCollision(c1);
	}

}


void ModuleQuestManager::ExecuteEvent(EVENT_ENUM eventId)
{
	switch (eventId)
	{
	case EVENT_ENUM::FINISH_QUEST:

		app->audio->PlayFx(questSfx, 0, -1);

		break;
	}
}


