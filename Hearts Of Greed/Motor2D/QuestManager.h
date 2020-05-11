#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "Module.h"

struct Collider;
struct SDL_Texture;

class Quest;

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

private:

	void ExecuteEvent(EVENT_ENUM eventId);


private:

	int questSfx;
	SDL_Texture* questMarker;
	

};

#endif //__QUESTMANAGER_H__