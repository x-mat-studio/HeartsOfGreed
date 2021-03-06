#include "PopUpManager.h"

#include "UI.h"
#include "EventManager.h"
#include "UI_Group.h"
#include "Audio.h"
#include "Input.h"

ModulePopUpManager::ModulePopUpManager() :
	Module(),
	popUpArray{},
	displayingPopUp(false)
{
	name.create("popUpManager");
}

ModulePopUpManager::~ModulePopUpManager()
{

}


bool ModulePopUpManager::Start()
{
	app->eventManager->EventRegister(EVENT_ENUM::CLOSE_POP_UP, this);

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

	app->eventManager->EventRegister(EVENT_ENUM::FOCUS_HERO_GATHERER, this);
	app->eventManager->EventRegister(EVENT_ENUM::FOCUS_HERO_MELEE, this);
	app->eventManager->EventRegister(EVENT_ENUM::FOCUS_HERO_RANGED, this);
	app->eventManager->EventRegister(EVENT_ENUM::FOCUS_HERO_ROBO, this);

	app->eventManager->EventRegister(EVENT_ENUM::HERO_RESQUED, this);
	app->eventManager->EventRegister(EVENT_ENUM::PLAYER_CONQUERED_A_BASE, this);

	app->eventManager->EventRegister(EVENT_ENUM::GAME_STARTED, this);

	popUp = app->audio->LoadFx("Assets/audio/sfx/Interface/PopUp.wav");


	return true;
}


bool ModulePopUpManager::Awake(pugi::xml_node& node)
{
	P2SString filename = node.attribute("docnamePopUps").as_string();
	pugi::xml_document popupsdoc;
	popupsdoc.load_file(filename.GetString());
	pugi::xml_node iterator = popupsdoc.first_child().first_child();

	int i = 0;
	for (iterator; iterator != NULL; iterator = iterator.next_sibling(), i++)
	{
		popUpArray[i].string = iterator.attribute("string").as_string();
	}
	return true;
}

bool ModulePopUpManager::Update(float dt)
{
	CheckListener(this);
	CheckPopUpsToDisplay();

	return true;
}


bool ModulePopUpManager::CleanUp()
{
	app->eventManager->EventUnRegister(EVENT_ENUM::CLOSE_POP_UP, this);

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

	app->eventManager->EventUnRegister(EVENT_ENUM::HERO_RESQUED, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::GAME_STARTED, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::PLAYER_CONQUERED_A_BASE, this);

	displayingPopUp = false;

	return true;
}


bool ModulePopUpManager::Load(pugi::xml_node& node)
{
	int i = 0;
	for (pugi::xml_node iterator = node.first_child().first_child(); iterator != NULL; iterator = iterator.next_sibling(), i++)
	{
		popUpArray[i].activated = iterator.attribute("activated").as_bool();
		popUpArray[i].finished = iterator.attribute("finished").as_bool();
	}

	return true;
}

bool ModulePopUpManager::Save(pugi::xml_node& node) const
{
	node = node.append_child("pop_ups_record");
	pugi::xml_node iterator;

	for (int i = 0; i < (int)POP_UPS::MAX; i++)
	{
		iterator = node.append_child("pop_up");

		iterator.append_attribute("activated") = popUpArray[i].activated;
		iterator.append_attribute("finished") = popUpArray[i].finished;
	}
	return true;
}


void ModulePopUpManager::ExecuteEvent(EVENT_ENUM eventId)
{
	if (eventId == EVENT_ENUM::CLOSE_POP_UP)
	{
		ClosePopUp();
	}

	if (eventId == EVENT_ENUM::HERO_LEVELED_UP)
	{
		popUpArray[(int)POP_UPS::LEVEL_UP].Activate();
	}
	else if (eventId == EVENT_ENUM::GATHERER_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::MELEE_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::RANGED_PASSIVE1_UPGRADE ||
		eventId == EVENT_ENUM::ROBOTTO_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::GATHERER_ACTIVE1_UPGRADE || eventId == EVENT_ENUM::MELEE_ACTIVE1_UPGRADE ||
		eventId == EVENT_ENUM::RANGED_ACTIVE1_UPGRADE || eventId == EVENT_ENUM::ROBOTTO_ACTIVE1_UPGRADE)
	{
		if (popUpArray[(int)POP_UPS::LEVEL_UP].displayed == true)
		{
			app->uiManager->SetPopUpClosingBool(true);
			app->uiManager->popupPosX.NewEasing(EASING_TYPE::EASE_IN_EXPO, 10, -1000, 1.0);
		}
		else
			popUpArray[(int)POP_UPS::LEVEL_UP].finished = true;
	}


	if (eventId == EVENT_ENUM::HERO_DEAD)
	{
		popUpArray[(int)POP_UPS::HERO_DIED].Activate();
	}
	else if (eventId == EVENT_ENUM::GATHERER_RESURRECT || eventId == EVENT_ENUM::MELEE_RESURRECT ||
		eventId == EVENT_ENUM::RANGED_RESURRECT || eventId == EVENT_ENUM::ROBOTTO_RESURRECT)
	{
		if (popUpArray[(int)POP_UPS::HERO_DIED].displayed == true)
		{
			app->uiManager->SetPopUpClosingBool(true);
			app->uiManager->popupPosX.NewEasing(EASING_TYPE::EASE_IN_EXPO, 10, -1000, 1.0);
		}
		else
			popUpArray[(int)POP_UPS::HERO_DIED].finished = true;
	}


	if (eventId == EVENT_ENUM::REACHED_100_RED_RESOUCES)
	{
		popUpArray[(int)POP_UPS::RED_RESOURCE_100].Activate();
	}
	else if (eventId == EVENT_ENUM::GATHERER_LIFE_UPGRADE || eventId == EVENT_ENUM::MELEE_LIFE_UPGRADE || eventId == EVENT_ENUM::RANGED_LIFE_UPGRADE ||
		eventId == EVENT_ENUM::ROBOTTO_LIFE_UPGRADE || eventId == EVENT_ENUM::GATHERER_DAMAGE_UPGRADE || eventId == EVENT_ENUM::MELEE_DAMAGE_UPGRADE ||
		eventId == EVENT_ENUM::RANGED_DAMAGE_UPGRADE || eventId == EVENT_ENUM::ROBOTTO_DAMAGE_UPGRADE || eventId == EVENT_ENUM::GATHERER_ENERGY_UPGRADE ||
		eventId == EVENT_ENUM::MELEE_ENERGY_UPGRADE || eventId == EVENT_ENUM::RANGED_ENERGY_UPGRADE || eventId == EVENT_ENUM::ROBOTTO_ENERGY_UPGRADE ||
		eventId == EVENT_ENUM::GATHERER_ATTACK_SPEED_UPGRADE || eventId == EVENT_ENUM::MELEE_ATTACK_SPEED_UPGRADE || eventId == EVENT_ENUM::RANGED_ATTACK_SPEED_UPGRADE ||
		eventId == EVENT_ENUM::ROBOTTO_ATTACK_SPEED_UPGRADE)
	{
		if (popUpArray[(int)POP_UPS::RED_RESOURCE_100].displayed == true)
		{
			app->uiManager->SetPopUpClosingBool(true);
			app->uiManager->popupPosX.NewEasing(EASING_TYPE::EASE_IN_EXPO, 10, -1000, 1.0);
		}
		else
			popUpArray[(int)POP_UPS::RED_RESOURCE_100].finished = true;
	}


	if (eventId == EVENT_ENUM::GOT_PURPLE_RESOURCE)
	{
		popUpArray[(int)POP_UPS::PURPLE_ORB].Activate();
	}
	else if (eventId == EVENT_ENUM::GATHERER_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::MELEE_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::RANGED_PASSIVE1_UPGRADE ||
		eventId == EVENT_ENUM::ROBOTTO_PASSIVE1_UPGRADE || eventId == EVENT_ENUM::GATHERER_ACTIVE1_UPGRADE || eventId == EVENT_ENUM::MELEE_ACTIVE1_UPGRADE ||
		eventId == EVENT_ENUM::RANGED_ACTIVE1_UPGRADE || eventId == EVENT_ENUM::ROBOTTO_ACTIVE1_UPGRADE)
	{
		if (popUpArray[(int)POP_UPS::PURPLE_ORB].displayed == true)
		{
			app->uiManager->SetPopUpClosingBool(true);
			app->uiManager->popupPosX.NewEasing(EASING_TYPE::EASE_IN_EXPO, 10, -1000, 1.0);
		}
		else
			popUpArray[(int)POP_UPS::PURPLE_ORB].finished = true;
	}


	if (eventId == EVENT_ENUM::UPGRADE_CENTER_CONSTRUCT)
	{
		popUpArray[(int)POP_UPS::BUY_TURRETS_AND_BARRICADES].Activate();
	}
	else if (eventId == EVENT_ENUM::TURRET_CONSTRUCT || eventId == EVENT_ENUM::BARRICADE_CONSTRUCT)
	{
		if (popUpArray[(int)POP_UPS::BUY_TURRETS_AND_BARRICADES].displayed == true)

		{
			app->uiManager->SetPopUpClosingBool(true);
			app->uiManager->popupPosX.NewEasing(EASING_TYPE::EASE_IN_EXPO, 10, -1000, 1.0);
		}
		else
			popUpArray[(int)POP_UPS::BUY_TURRETS_AND_BARRICADES].finished = true;
	}


	if (eventId == EVENT_ENUM::TURRET_CONSTRUCT || eventId == EVENT_ENUM::BARRICADE_CONSTRUCT)
	{
		popUpArray[(int)POP_UPS::UPGRADE_TURRETS_AND_BARRICADES].Activate();
	}
	else if (eventId == EVENT_ENUM::TURRET_UPGRADED || eventId == EVENT_ENUM::BARRICADE_UPGRADED)
	{
		if (popUpArray[(int)POP_UPS::UPGRADE_TURRETS_AND_BARRICADES].displayed == true)
		{
			app->uiManager->SetPopUpClosingBool(true);
			app->uiManager->popupPosX.NewEasing(EASING_TYPE::EASE_IN_EXPO, 10, -1000, 1.0);
		}
		else
			popUpArray[(int)POP_UPS::UPGRADE_TURRETS_AND_BARRICADES].finished = true;
	}


	if (eventId == EVENT_ENUM::HERO_RESQUED)
	{
		popUpArray[(int)POP_UPS::TRY_FOCUS_KEYS].Activate();
	}
	else if (app->input->GetKey(SDL_SCANCODE_1) == KEY_STATE::KEY_DOWN || app->input->GetKey(SDL_SCANCODE_2) == KEY_STATE::KEY_DOWN ||
		app->input->GetKey(SDL_SCANCODE_3) == KEY_STATE::KEY_DOWN || app->input->GetKey(SDL_SCANCODE_4) == KEY_STATE::KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_STATE::KEY_DOWN)
	{
		if (popUpArray[(int)POP_UPS::TRY_FOCUS_KEYS].displayed == true)
		{
			app->uiManager->SetPopUpClosingBool(true);
			app->uiManager->popupPosX.NewEasing(EASING_TYPE::EASE_IN_EXPO, 10, -1000, 1.0);
		}
		else if (popUpArray[(int)POP_UPS::TRY_FOCUS_KEYS].activated == true)
			popUpArray[(int)POP_UPS::TRY_FOCUS_KEYS].finished = true;
	}

	if (eventId == EVENT_ENUM::GAME_STARTED)
	{
		popUpArray[(int)POP_UPS::CAPTURE_ALL_BASES].Activate();
	}
	else if (eventId == EVENT_ENUM::PLAYER_CONQUERED_A_BASE)
	{
		if (popUpArray[(int)POP_UPS::CAPTURE_ALL_BASES].displayed == true)
		{
			app->uiManager->SetPopUpClosingBool(true);
			app->uiManager->popupPosX.NewEasing(EASING_TYPE::EASE_IN_EXPO, 10, -1000, 1.0);
		}
		else if (popUpArray[(int)POP_UPS::CAPTURE_ALL_BASES].activated == true)
			popUpArray[(int)POP_UPS::CAPTURE_ALL_BASES].finished = true;
	}
}


void ModulePopUpManager::CheckPopUpsToDisplay()
{
	if (displayingPopUp == false)
	{
		for (int i = 0; i < (int)POP_UPS::MAX; i++)
		{
			if (popUpArray[i].activated == true && popUpArray[i].finished == false)
			{
				app->uiManager->CreatePopUp(popUpArray[i].string);
				app->audio->PlayFx(popUp, 0, -1);
				popUpArray[i].displayed = true;
				displayingPopUp = true;
			}
		}
	}
}


void ModulePopUpManager::ClosePopUp()
{
	for (int i = 0; i < (int)POP_UPS::MAX; i++)
	{
		if (popUpArray[i].displayed)
		{
			popUpArray[i].Deactivate();
			displayingPopUp = false;
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

void PopUp::Activate()
{
	if (finished == true)
	{
		activated = false;
	}
	else
		activated = true;
}


void PopUp::Deactivate()
{
	finished = true;
	displayed = false;
	activated = false;
	app->uiManager->DeleteUIGroup(GROUP_TAG::POP_UP);
}