#include "PopUpManager.h"
#include "UI.h"
#include "EventManager.h"

PopUpManager::PopUpManager() :
	Module(),
	popUpArray{}
{
	name.create("PopUpManager");
}

PopUpManager::~PopUpManager()
{

}


bool PopUpManager::Start()
{
	app->eventManager->EventRegister(EVENT_ENUM::HERO_LEVELED_UP, this);
	app->eventManager->EventRegister(EVENT_ENUM::HERO_DEAD, this);

	app->eventManager->EventRegister(EVENT_ENUM::REACHED_100_RED_RESOUCES, this);
	app->eventManager->EventRegister(EVENT_ENUM::GOT_PURPLE_RESOURCE, this);
	app->eventManager->EventRegister(EVENT_ENUM::NIGHT_START, this);

	app->eventManager->EventRegister(EVENT_ENUM::TURRET_CONSTRUCT, this);
	app->eventManager->EventRegister(EVENT_ENUM::TURRET_UPGRADED, this);

	app->eventManager->EventRegister(EVENT_ENUM::BARRICADE_CONSTRUCT, this);
	app->eventManager->EventRegister(EVENT_ENUM::BARRICADE_UPGRADED, this);

	app->eventManager->EventRegister(EVENT_ENUM::UPGRADE_CENTER_CONSTRUCT, this);

	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_PASSIVE1_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_PASSIVE1_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_PASSIVE1_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_PASSIVE1_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_ACTIVE1_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_ACTIVE1_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_ACTIVE1_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_ACTIVE1_UPGRADE, this);


	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_LIFE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_LIFE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_LIFE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_LIFE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_DAMAGE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_DAMAGE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_DAMAGE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_DAMAGE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_ENERGY_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_ENERGY_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_ENERGY_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_ENERGY_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_ATTACK_SPEED_UPGRADE, this);

	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_RESURRECT, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_RESURRECT, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_RESURRECT, this);
	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_RESURRECT, this);

	return true;
}


bool PopUpManager::Awake(pugi::xml_node& node)
{

	return true;
}

bool PopUpManager::Update(float dt)
{
	CheckListener(this);
	return true;
}


bool PopUpManager::CleanUp()
{
	app->eventManager->EventUnRegister(EVENT_ENUM::HERO_LEVELED_UP, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::HERO_DEAD, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::REACHED_100_RED_RESOUCES, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GOT_PURPLE_RESOURCE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::NIGHT_START, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::TURRET_CONSTRUCT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::TURRET_UPGRADED, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::BARRICADE_CONSTRUCT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::BARRICADE_UPGRADED, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::UPGRADE_CENTER_CONSTRUCT, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_PASSIVE1_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_PASSIVE1_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_PASSIVE1_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_PASSIVE1_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_ACTIVE1_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_ACTIVE1_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_ACTIVE1_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_ACTIVE1_UPGRADE, this);


	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_LIFE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_LIFE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_LIFE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_LIFE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_DAMAGE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_DAMAGE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_DAMAGE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_DAMAGE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_ENERGY_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_ENERGY_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_ENERGY_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_ENERGY_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_ATTACK_SPEED_UPGRADE, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_RESURRECT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_RESURRECT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_RESURRECT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_RESURRECT, this);

	return true;
}


bool PopUpManager::Load(pugi::xml_node&)
{

	return true;
}

bool PopUpManager::Save(pugi::xml_node&) const
{

	return true;
}


void PopUpManager::ExecuteEvent(EVENT_ENUM eventId)
{
	if (eventId == EVENT_ENUM::HERO_LEVELED_UP)
	{
		popUpArray[(int)POP_UPS::LEVEL_UP].activated = true;
	}
	else if (popUpArray[(int)POP_UPS::LEVEL_UP].displayed == true)
	{ 
		if (eventId == EVENT_ENUM::GATHERER_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::MELEE_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::RANGED_PASSIVE1_UPGRADE ||
			eventId == EVENT_ENUM::ROBOTTO_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::GATHERER_ACTIVE1_UPGRADE || eventId == EVENT_ENUM::MELEE_ACTIVE1_UPGRADE ||
			eventId == EVENT_ENUM::RANGED_ACTIVE1_UPGRADE || eventId == EVENT_ENUM::ROBOTTO_ACTIVE1_UPGRADE)
		{
			//Add logic to remove the pop up, or something, idfk yet, YET
		}
	}


	else if (eventId == EVENT_ENUM::HERO_DEAD)
	{
		popUpArray[(int)POP_UPS::HERO_DIED].activated = true;
	}
	else if (popUpArray[(int)POP_UPS::HERO_DIED].displayed == true)
	{
		if (eventId == EVENT_ENUM::GATHERER_RESURRECT || eventId == EVENT_ENUM::MELEE_RESURRECT ||
			eventId == EVENT_ENUM::RANGED_RESURRECT || eventId == EVENT_ENUM::ROBOTTO_RESURRECT)
		{
			//Add logic to remove the pop up, or something, idfk yet, YET
		}
	}


	else if (eventId == EVENT_ENUM::REACHED_100_RED_RESOUCES)
	{
		popUpArray[(int)POP_UPS::RED_RESOURCE_100].activated = true;
	}
	else if (popUpArray[(int)POP_UPS::RED_RESOURCE_100].displayed == true)
	{
		if (eventId == EVENT_ENUM::GATHERER_LIFE_UPGRADE || eventId == EVENT_ENUM::MELEE_LIFE_UPGRADE || eventId == EVENT_ENUM::RANGED_LIFE_UPGRADE ||
			eventId == EVENT_ENUM::ROBOTTO_LIFE_UPGRADE || eventId == EVENT_ENUM::GATHERER_DAMAGE_UPGRADE || eventId == EVENT_ENUM::MELEE_DAMAGE_UPGRADE ||
			eventId == EVENT_ENUM::RANGED_DAMAGE_UPGRADE || eventId == EVENT_ENUM::ROBOTTO_DAMAGE_UPGRADE || eventId == EVENT_ENUM::GATHERER_ENERGY_UPGRADE ||
			eventId == EVENT_ENUM::MELEE_ENERGY_UPGRADE || eventId == EVENT_ENUM::RANGED_ENERGY_UPGRADE || eventId == EVENT_ENUM::ROBOTTO_ENERGY_UPGRADE ||
			eventId == EVENT_ENUM::GATHERER_ATTACK_SPEED_UPGRADE || eventId == EVENT_ENUM::MELEE_ATTACK_SPEED_UPGRADE || eventId == EVENT_ENUM::RANGED_ATTACK_SPEED_UPGRADE ||
			eventId == EVENT_ENUM::ROBOTTO_ATTACK_SPEED_UPGRADE)
		{
			//Add logic to remove the pop up, or something, idfk yet, YET
		}
	}


	else if (eventId == EVENT_ENUM::GOT_PURPLE_RESOURCE)
	{
		popUpArray[(int)POP_UPS::PURPLE_ORB].activated = true;
	}
	else if (popUpArray[(int)POP_UPS::PURPLE_ORB].displayed == true)
	{
		if (eventId == EVENT_ENUM::GATHERER_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::MELEE_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::RANGED_PASSIVE1_UPGRADE ||
			eventId == EVENT_ENUM::ROBOTTO_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::GATHERER_ACTIVE1_UPGRADE || eventId == EVENT_ENUM::MELEE_ACTIVE1_UPGRADE ||
			eventId == EVENT_ENUM::RANGED_ACTIVE1_UPGRADE || eventId == EVENT_ENUM::ROBOTTO_ACTIVE1_UPGRADE)
		{
			//Add logic to remove the pop up, or something, idfk yet, YET
		}
	}


	else if (eventId == EVENT_ENUM::UPGRADE_CENTER_CONSTRUCT)
	{
		popUpArray[(int)POP_UPS::BUY_TURRETS_AND_BARRICADES].activated = true;
	}
	else if (popUpArray[(int)POP_UPS::BUY_TURRETS_AND_BARRICADES].displayed == true)
	{
		if (eventId == EVENT_ENUM::TURRET_CONSTRUCT || eventId == EVENT_ENUM::BARRICADE_CONSTRUCT)
		{
			//Add logic to remove the pop up, or something, idfk yet, YET
		}
	}


	else if (eventId == EVENT_ENUM::TURRET_CONSTRUCT || eventId == EVENT_ENUM::BARRICADE_CONSTRUCT)
	{
		popUpArray[(int)POP_UPS::UPGRADE_TURRETS_AND_BARRICADES].activated = true;
	}
	else if (popUpArray[(int)POP_UPS::UPGRADE_TURRETS_AND_BARRICADES].displayed == true)
	{
		if (eventId == EVENT_ENUM::TURRET_UPGRADED || eventId == EVENT_ENUM::BARRICADE_UPGRADED)
		{
			//Add logic to remove the pop up, or something, idfk yet, YET
		}
	}
}


PopUp::PopUp() :
	string(),
	activated(false),
	displayed(false),
	finished(false)
{
}

PopUp::~PopUp()
{
	string.Clear();
}