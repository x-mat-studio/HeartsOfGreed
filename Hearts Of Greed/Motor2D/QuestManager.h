#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "Module.h"
#include "Collision.h"
#include "Quest.h"

class ModuleQuestManager : public Module
{
public:

	ModuleQuestManager();
	virtual ~ModuleQuestManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void AddQuest(Collider* activation);

	std::vector <Quest*> ongoing;
	std::vector <Quest*> finished;

private:

	void ExecuteEvent(EVENT_ENUM eventId);
};

#endif //__QUESTMANAGER_H__