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
#include "DialogManager.h"



ModuleQuestManager::ModuleQuestManager() :
	questMarker(nullptr),
	questSfx(-1),
	questFailed(-1)
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
			questInfoVector[i].PushEntityToSpawn((ENTITY_TYPE)iterator2.attribute("type").as_int(), iterator2.attribute("posX").as_float(), iterator2.attribute("posY").as_float());
		}
	}
}


bool ModuleQuestManager::Start()
{
	bool ret = true;

	ret = app->eventManager->EventRegister(EVENT_ENUM::FINISH_QUEST, this);
	ret = app->eventManager->EventRegister(EVENT_ENUM::FAIL_QUEST, this);

	questSfx = app->audio->LoadFx("audio/sfx/Interface/questDone.wav");
	questFailed = app->audio->LoadFx("audio/sfx/Interface/MissionFailed.wav");

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

	ret = app->eventManager->EventUnRegister(EVENT_ENUM::FINISH_QUEST, this);
	ret = app->eventManager->EventUnRegister(EVENT_ENUM::FAIL_QUEST, this);

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
		app->player->AddResourcesSkill(2);
		break;

	case EVENT_ENUM::FAIL_QUEST:

		app->audio->PlayFx(questFailed, 0, -1);
		break;
	}
}


SDL_Texture* ModuleQuestManager::GetTexture()
{
	return questMarker;
}


void ModuleQuestManager::QuestStarted(int questId)
{
	if (questId == -1)
	{
		assert("Quest id not initialized");
	}
	questInfoVector[questId].StartQuest();

	switch (questId)
	{
	case 0:
		app->dialogManager->PushInput(DIALOG_INPUT::TUTORIAL_START);
		questInfoVector[questId].SetDialogInput((int)DIALOG_INPUT::TUTORIAL_END);
		break;

	case 1:
		app->dialogManager->PushInput(DIALOG_INPUT::MISSION_1_START);
		questInfoVector[questId].SetDialogInput((int)DIALOG_INPUT::MISSION_1_END);
		break;

	case 2:
		app->dialogManager->PushInput(DIALOG_INPUT::MISSION_2_START);
		questInfoVector[questId].SetDialogInput((int)DIALOG_INPUT::MISSION_2_END);
		break;

	case 3:
		app->dialogManager->PushInput(DIALOG_INPUT::MISSION_3_START);
		questInfoVector[questId].SetDialogInput((int)DIALOG_INPUT::MISSION_3_END);
		break;

	case 4:
		app->dialogManager->PushInput(DIALOG_INPUT::MISSION_4_START);
		questInfoVector[questId].SetDialogInput((int)DIALOG_INPUT::MISSION_4_END);
		break;
	default:
		break;
	}

	app->eventManager->GenerateEvent(EVENT_ENUM::CREATE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
}


void ModuleQuestManager::CheckEntityDead(Entity* entity)
{
	int questNumber = questInfoVector.size();

	for (int i = 0; i < questNumber; i++)
	{
		if (questInfoVector[i].active == true)
		{
			if (questInfoVector[i].CheckQuestStatus(entity))
			{
				app->eventManager->GenerateEvent(EVENT_ENUM::FINISH_QUEST, EVENT_ENUM::NULL_EVENT);
			}

		}
	}
}


bool ModuleQuestManager::Load(pugi::xml_node& data)
{
	questInfoVector.clear(); //Just in case :D

	Entity* entity = nullptr;
	int i = 0;

	for (pugi::xml_node iterator = data.first_child(); iterator != NULL; iterator = iterator.next_sibling(), i++)
	{
		questInfoVector.push_back(QuestInfo(iterator.attribute("reward").as_int(), iterator.attribute("id").as_int(), iterator.attribute("active").as_bool()));
		questInfoVector[i].SetDialogInput(iterator.attribute("dialogInput").as_int());

		for (pugi::xml_node iterator2 = iterator.first_child().first_child(); iterator2 != NULL; iterator2 = iterator2.next_sibling())
		{
			entity = app->entityManager->AddEntity((ENTITY_TYPE)iterator2.attribute("entityType").as_int(), iterator2.attribute("posX").as_float(), iterator2.attribute("posY").as_float());
			entity->SetCurrentHP(iterator2.attribute("hp").as_int());

			questInfoVector[i].PushEntity(entity);
		}
	}

	return true;
}


bool ModuleQuestManager::Save(pugi::xml_node& data) const
{
	int questNumber = questInfoVector.size();

	for (int i = 0; i < questNumber; i++)
	{
		pugi::xml_node iterator = data.append_child("quest");

		questInfoVector[i].Save(iterator);
	}

	return true;
}


//Struct QuestInfo

QuestInfo::QuestInfo(int resourcesReward, int id, bool active) :

	resourcesReward(resourcesReward),
	id(id),
	dialogInput(-1),

	active(active)
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
		entity->missionEntity = true;

		switch (entity->GetType())
		{
		case ENTITY_TYPE::HERO_GATHERER:
			app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_GATHERER, EVENT_ENUM::NULL_EVENT);
			break;

		case ENTITY_TYPE::HERO_MELEE:
			app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_MELEE, EVENT_ENUM::NULL_EVENT);
			break;

		case ENTITY_TYPE::HERO_RANGED:
			app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_RANGED, EVENT_ENUM::NULL_EVENT);
			break;

		case ENTITY_TYPE::HERO_ROBO:
			app->eventManager->GenerateEvent(EVENT_ENUM::FOCUS_HERO_ROBO, EVENT_ENUM::NULL_EVENT);
			break;
		default:
			break;
		}

		questEntitysVector.push_back(entity);
	}

	active = true;
}


bool QuestInfo::CheckQuestStatus(Entity* entity)
{
	int numberEntitys = questEntitysVector.size();

	for (int i = 0; i < numberEntitys; i++)
	{
		if (questEntitysVector[i] == entity)
		{
			questEntitysVector.erase(questEntitysVector.begin() + i);

			if (entity->GetAlignment() == ENTITY_ALIGNEMENT::PLAYER)
			{
				LoseQuest();
				return false;
			}

			else if (questEntitysVector.size() == 1 || questEntitysVector.size() == 0) //Only remains the hero
			{
				questEntitysVector[0]->missionEntity = false;
				WinQuest();
				return true;
			}
			
		}
	}

	return false;
}


void QuestInfo::PushEntityToSpawn(ENTITY_TYPE entity, float x, float y)
{
	entitysToSpawnVector.push_back(entity);
	positionsToSpawnVector.push_back(fMPoint(x, y));
}


void QuestInfo::PushEntity(Entity* entity)
{
	entity->missionEntity = true;
	questEntitysVector.push_back(entity);
}


void QuestInfo::WinQuest()
{
	questEntitysVector.clear();
	active = false;

	GiveReward();
	app->dialogManager->PushInput((DIALOG_INPUT)dialogInput);

	app->eventManager->GenerateEvent(EVENT_ENUM::FINISH_QUEST, EVENT_ENUM::NULL_EVENT);
	app->eventManager->GenerateEvent(EVENT_ENUM::CREATE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
}


void QuestInfo::LoseQuest()
{
	int size = questEntitysVector.size();

	for (int j = 0; j < size; j++)
	{
		questEntitysVector[j]->missionEntity = false;
	}

	questEntitysVector.clear();

	app->eventManager->GenerateEvent(EVENT_ENUM::FAIL_QUEST, EVENT_ENUM::NULL_EVENT);
	active = false;

}


void QuestInfo::GiveReward()
{
	app->player->AddResources(resourcesReward);
}


void QuestInfo::SetDialogInput(int input)
{
	dialogInput = input;
}


bool QuestInfo::Save(pugi::xml_node& node) const
{
	node.append_attribute("active") = active;
	node.append_attribute("id") = id;

	node.append_attribute("dialogInput") = dialogInput;
	node.append_attribute("reward") = resourcesReward;

	pugi::xml_node iterator1 = node.append_child("questEntitys");

	int numEntitys = questEntitysVector.size();

	for (int i = 0; i < numEntitys; i++)
	{
		pugi::xml_node iterator2 = iterator1.append_child("entity");

		iterator2.append_attribute("entityType") = (int)questEntitysVector[i]->GetType();
		iterator2.append_attribute("posX") = questEntitysVector[i]->GetPosition().x;
		iterator2.append_attribute("posY") = questEntitysVector[i]->GetPosition().y;

		iterator2.append_attribute("hp") = questEntitysVector[i]->GetCurrentHP();
	}


	return true;
}