#include "App.h"
#include "QuestManager.h"
#include "Quest.h"


#include "EventManager.h"
#include "EntityManager.h"
#include "Collision.h"
#include "Audio.h"
#include "Textures.h"
#include "Brofiler/Brofiler/Brofiler.h"
#include "Player.h"



ModuleQuestManager::ModuleQuestManager() :
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

	P2SString filename = config.child("load").attribute("docnameQuests").as_string();
	pugi::xml_document questsdoc;
	questsdoc.load_file(filename.GetString());
	pugi::xml_node node = questsdoc.first_child();

	LoadQuests(node);

	questsdoc.reset();

	return ret;
}


void ModuleQuestManager::LoadQuests(pugi::xml_node& node)
{
	int i = 0;
	
	for (pugi::xml_node iterator = node.first_child(); iterator != NULL; iterator = iterator.next_sibling(), i++)
	{
		questInfoVector.push_back(QuestInfo(iterator.attribute("reward").as_int(), iterator.attribute("id").as_int()));
		
		for (pugi::xml_node iterator2 = iterator.first_child().first_child(); iterator2 != NULL; iterator2 = iterator2.next_sibling())
		{
			questInfoVector[i].PushEntityToSpawn((ENTITY_TYPE)iterator2.attribute("type").as_int(), iterator2.attribute("posX").as_int(), iterator2.attribute("posY").as_int());
		}
	}
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
	
}



//Struct QuestInfo

QuestInfo::QuestInfo(int resourcesReward, int id) :

	resourcesReward(resourcesReward),
	id(id),

	active(false)
{}


QuestInfo::~QuestInfo()
{}


void QuestInfo::StartQuest()
{
	Entity* entity = nullptr;

	int numberToSpawn = entitysToSpawnVector.size();

	for (int i = 0; i < numberToSpawn; i++)
	{
		entity = app->entityManager->AddEntity(entitysToSpawnVector[i], positionsToSpawnVector[i].x, positionsToSpawnVector[i].y);
		entity->selectedByPlayer = true; //maiby this causes problems, maiby not, ill put it here and see later

		switch (entity->GetType())
		{
		case ENTITY_TYPE::HERO_GATHERER:
			app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_GATHERER, EVENT_ENUM::NULL_EVENT);
			break;

		case ENTITY_TYPE::HERO_MELEE:
			app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_GATHERER, EVENT_ENUM::NULL_EVENT);
			break;

		case ENTITY_TYPE::HERO_RANGED:
			app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_GATHERER, EVENT_ENUM::NULL_EVENT);
			break;

		case ENTITY_TYPE::HERO_ROBO:
			app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_GATHERER, EVENT_ENUM::NULL_EVENT);
			break;
		default:
			break;
		}

		questEntitysVector.push_back(entity);
	}

}


void QuestInfo::PushEntityToSpawn(ENTITY_TYPE entity, float x, float y)
{
	entitysToSpawnVector.push_back(entity);
	positionsToSpawnVector.push_back(fMPoint(x, y));
}