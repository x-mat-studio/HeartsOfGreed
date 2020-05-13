#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "Module.h"

struct Collider;
struct SDL_Texture;

class Quest;
class Entity;
enum class ENTITY_TYPE;


struct QuestInfo
{
public:

	QuestInfo(int resourcesReward, int id);
	~QuestInfo();

	void StartQuest();
	bool CheckQuestStatus(Entity* entity);

	void PushEntityToSpawn(ENTITY_TYPE, float x, float y);


private:
	void GiveReward();


public:
	bool active;


private:
	std::vector<ENTITY_TYPE> entitysToSpawnVector;
	std::vector<fMPoint> positionsToSpawnVector;

	std::vector<Entity*> questEntitysVector;

	int resourcesReward;

	int id;

};


class ModuleQuestManager : public Module
{
public:

	ModuleQuestManager();
	virtual ~ModuleQuestManager();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate(float dt);
	bool CleanUp();

	void OnCollision(Collider * c1, Collider* c2);

	SDL_Texture* GetTexture();

	void QuestStarted(int questId);
	void CheckEntityDead(Entity* entity);

private:
	void LoadQuests(pugi::xml_node&);

	void ExecuteEvent(EVENT_ENUM eventId);

private:

	int questSfx;
	int questFailed;
	SDL_Texture* questMarker;
	
	std::vector<QuestInfo> questInfoVector;
};

#endif //__QUESTMANAGER_H__