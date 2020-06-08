#ifndef __POP_UP_MANAGER_H__
#define __POP_UP_MANAGER_H__

#include "Module.h"

enum class POP_UPS : int
{
		NONE = -1,

		RED_RESOURCE_100,
		PURPLE_ORB,

		LEVEL_UP,
		HERO_DIED,

		BUY_TURRETS_AND_BARRICADES,
		UPGRADE_TURRETS_AND_BARRICADES,

		MAX
};

struct PopUp
{
	PopUp();
	~PopUp();

	P2SString string;

	bool activated;
	bool displayed;
	bool finished;
};


class PopUpManager : public Module
{
public:
	PopUpManager();
	~PopUpManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	
	bool Update(float dt);
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:
	void ExecuteEvent(EVENT_ENUM eventId);

private:
	PopUp popUpArray[(int)POP_UPS::MAX];

};

#endif//__POP_UP_MANAGER_H__