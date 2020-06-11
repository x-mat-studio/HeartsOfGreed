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

		TRY_FOCUS_KEYS,

		MAX
};

struct PopUp
{
public:
	PopUp();
	~PopUp();

	void Activate();
	void Deactivate();

public:
	P2SString string;

	bool activated;
	bool displayed;
	bool finished;
};


class ModulePopUpManager : public Module
{
public:
	ModulePopUpManager();
	~ModulePopUpManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	
	bool Update(float dt);
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:
	void ExecuteEvent(EVENT_ENUM eventId);

	void CheckPopUpsToDisplay();
	void ClosePopUp();

private:
	PopUp popUpArray[(int)POP_UPS::MAX];

	bool displayingPopUp;

	//pop up sound
	int popUp;

};

#endif//__POP_UP_MANAGER_H__