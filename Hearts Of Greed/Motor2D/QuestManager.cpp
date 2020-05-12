#include "App.h"
#include "QuestManager.h"
#include "Quest.h"


#include "EventManager.h"
#include "Collision.h"
#include "Audio.h"
#include "Textures.h"
#include "Brofiler/Brofiler/Brofiler.h"
#include "Player.h"



ModuleQuestManager::ModuleQuestManager():
	questMarker(nullptr),
	questSfx(-1)
{
	name.create("QuestManager");
}


ModuleQuestManager::~ModuleQuestManager()
{}


bool ModuleQuestManager::Awake(pugi::xml_node& config)
{
	bool ret = true;
	BROFILER_CATEGORY("Quest Manager Awake", Profiler::Color::RosyBrown);

	return ret;
}


bool ModuleQuestManager::Start()
{
	bool ret = true;

	ret = app->eventManager->EventRegister(EVENT_ENUM::FINISH_QUEST, this);

	questSfx = app->audio->LoadFx("audio/sfx/Interface/questDone.wav");

	questMarker = app->tex->Load("spritesheets/VFX/questMarker.png");

	return ret;
}


bool ModuleQuestManager::PreUpdate(float dt)
{
	bool ret = true;

	CheckListener(this);

	return ret;
}


bool ModuleQuestManager::CleanUp()
{
	bool ret = true;

	app->tex->UnLoad(questMarker);		
	
	questMarker = nullptr;
	//Quests are entities. We delete them at entity manager, not here.

	return ret;
}


void ModuleQuestManager::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->thisEntity != nullptr && c1->type == COLLIDER_TYPE::COLLIDER_QUEST) 
	{
		c1->thisEntity->OnCollision(c2);
	}
}


void ModuleQuestManager::ExecuteEvent(EVENT_ENUM eventId)
{
	switch (eventId)
	{
	case EVENT_ENUM::FINISH_QUEST:

		app->audio->PlayFx(questSfx, 0, -1);

		app->player->AddResources(400);

		break;
	}
}


SDL_Texture* ModuleQuestManager::GetTexture()
{
	return questMarker;
}


void ModuleQuestManager::QuestStarted(int questId)
{
	//Do something, idfk yet, YET
}