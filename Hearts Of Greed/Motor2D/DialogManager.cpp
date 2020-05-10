#include "DialogManager.h"

ModuleDialogManager::ModuleDialogManager() :

	Module(),

	currentDialog(nullptr),

	dialogTutorialStart(),
	dialogTutorialEnd(),
	dialogMission1_ST_B1(),
	dialogMission1_ST_A1(),
	dialogMission1_ST_B2(),

	dialogMission1_END_B3(),
	dialogMission1_END_A2(),

	input(DIALOG_INPUT::NULL_INPUT),
	state(DIALOG_STATE::IDLE_DIALOG)
{
	name.create("DialogManager");
}


ModuleDialogManager::~ModuleDialogManager()
{
	dialogTutorialStart.Clear();
	dialogTutorialEnd.Clear();
	dialogMission1_ST_B1.Clear();
	dialogMission1_ST_A1.Clear();
	dialogMission1_ST_B2.Clear();

	dialogMission1_END_B3.Clear();
	dialogMission1_END_A2.Clear();

	input = DIALOG_INPUT::INPUT_ERROR;
	state = DIALOG_STATE::ERROR_DIALOG;

	currentDialog = nullptr;
}


bool ModuleDialogManager::Awake(pugi::xml_node& data)
{
	//Get all the dialogs froim xml

	return true;
}


bool ModuleDialogManager::PreUpdate(float dt)
{
	InternalInput(dt);
	ProcessFsm();

	return true;
}


void ModuleDialogManager::InternalInput(float dt)
{
	// HERE GOES ALL TIMERS

}


void ModuleDialogManager::ProcessFsm()
{
	
}


bool ModuleDialogManager::PushInput(DIALOG_INPUT inp)
{
	if (input != DIALOG_INPUT::NULL_INPUT)
	{
		return false;
	}

	else
		input = inp;
		
	return true;
}


P2SString* ModuleDialogManager::GetCurrentString() const
{
	return currentDialog;
}
