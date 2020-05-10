#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "Module.h"
#include "Collision.h"
#include "Quest.h"
#include <unordered_map>

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

	std::unordered_map <int , Quest*> ongoing;
	std::unordered_map <int, Quest*> finished;

	void OnCollision(Collider * c1, Collider* c2);

	int questSfx;

	SDL_Texture* questMarker;
	
private:

	void ExecuteEvent(EVENT_ENUM eventId);
};

#endif //__QUESTMANAGER_H__