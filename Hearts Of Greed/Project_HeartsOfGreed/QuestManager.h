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

	QuestInfo(int resourcesReward, int id, bool noCombat = false, bool active = false);
	~QuestInfo();

	void StartQuest();
	bool CheckQuestStatus(Entity* entity);

	void PushEntityToSpawn(ENTITY_TYPE, float x, float y);
	void PushEntity(Entity* entity);

	void SetDialogInput(int input);

	bool Save(pugi::xml_node&) const;

	int GetId() const;

	void ClearQuestEntities();

private:
	void WinQuest();
	void LoseQuest();
	void GiveReward();


public:
	bool active;
	bool noCombat;


private:
	std::vector<ENTITY_TYPE> entitysToSpawnVector;
	std::vector<fMPoint> positionsToSpawnVector;

	std::vector<Entity*> questEntitysVector;

	int resourcesReward;

	int id;
	int dialogInput;
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

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void AddQuest(Quest* questo);
	ENTITY_TYPE RequestCharacter1();
	ENTITY_TYPE RequestCharacter2();

private:
	void LoadQuests(pugi::xml_node&);

	void ExecuteEvent(EVENT_ENUM eventId);

public:
	std::vector<Quest*> questColliderVector;

private:

	int questSfx;
	int questFailed;
	SDL_Texture* questMarker;
	ENTITY_TYPE character1;
	ENTITY_TYPE character2;
	
	std::vector<QuestInfo> questInfoVector;
};

#endif //__QUESTMANAGER_H__