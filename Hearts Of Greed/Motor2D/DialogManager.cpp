#include "DialogManager.h"

ModuleDialogManager::ModuleDialogManager() :

	Module(),

	currentDialog1stCharacter(nullptr),
	currentDialog2ndCharacter(nullptr),

	//TUTORIAL-------------
	dialogTutorialStart(),
	dialogTutorialEnd(),

	//Quest 1 ------------
	dialogMission1_ST_B1(),
	dialogMission1_ST_A1(),
	dialogMission1_ST_B2(),

	dialogMission1_END_A2(),
	dialogMission1_END_B3(),

	//Quest 2 ------------
	dialogMission2_ST_B1(),
	dialogMission2_ST_A1(),

	dialogMission2_END_A2(),
	dialogMission2_END_B2(),

	//Quest 3 ------------
	dialogMission3_ST_A1(),

	dialogMission3_END_B1(),
	dialogMission3_END_A2(),
	dialogMission3_END_B2(),

	//Quest 4 ------------
	dialogMission4_ST_A1(),
	dialogMission4_ST_B1(),

	dialogMission4_END_C1(),
	dialogMission4_END_A2(),

	input(DIALOG_INPUT::NULL_INPUT),
	state(DIALOG_STATE::IDLE_DIALOG)
{
	name.create("dialogManager"); //hahahahaaaha
}


ModuleDialogManager::~ModuleDialogManager()
{
	
	input = DIALOG_INPUT::INPUT_ERROR;
	state = DIALOG_STATE::ERROR_DIALOG;

	currentDialog1stCharacter = nullptr;
	currentDialog2ndCharacter = nullptr;

	//TUTORIAL-------------
	 dialogTutorialStart.Clear();
	 dialogTutorialEnd.Clear();

	//Quest 1 ------------
	 dialogMission1_ST_B1.Clear();
	 dialogMission1_ST_A1.Clear();
	 dialogMission1_ST_B2.Clear();

	 dialogMission1_END_A2.Clear();
	 dialogMission1_END_B3.Clear();

	//Quest 2 ------------
	 dialogMission2_ST_B1.Clear();
	 dialogMission2_ST_A1.Clear();

	 dialogMission2_END_A2.Clear();
	 dialogMission2_END_B2.Clear();

	//Quest 3 ------------
	 dialogMission3_ST_A1.Clear();

	 dialogMission3_END_B1.Clear();
	 dialogMission3_END_A2.Clear();
	 dialogMission3_END_B2.Clear();

	//Quest 4 ------------
	 dialogMission4_ST_A1.Clear();
	 dialogMission4_ST_B1.Clear();

	 dialogMission4_END_C1.Clear();
	 dialogMission4_END_A2.Clear();
}


bool ModuleDialogManager::Awake(pugi::xml_node& data)
{
	//Load required doc

	P2SString filename = data.child("load").attribute("docnameDialogues").as_string();
	pugi::xml_document diagDoc;
	diagDoc.load_file(filename.GetString());
	pugi::xml_node dialogue = diagDoc.child("dialogues");

	//r.x = heroNode.child("sample").child("collider").child("rect").attribute("x").as_int(0);

	//Assign dialogues to strings

	//TUTORIAL-------------
	 dialogTutorialStart		= (P2SString)dialogue.child("tutorial").child("start").child("entry1").attribute("text").as_string();

	 dialogTutorialEnd			= (P2SString)dialogue.child("tutorial").child("finish").child("entry1").attribute("text").as_string();

	//Quest 1 ------------
	 dialogMission1_ST_B1		= dialogue.child("dialog1").child("start").child("entry1").attribute("text").as_string();
	 dialogMission1_ST_A1		= dialogue.child("dialog1").child("start").child("entry2").attribute("text").as_string();
	 dialogMission1_ST_B2		= dialogue.child("dialog1").child("start").child("entry3").attribute("text").as_string();

	 dialogMission1_END_A2		= dialogue.child("dialog1").child("finish").child("entry1").attribute("text").as_string();
	 dialogMission1_END_B3		= dialogue.child("dialog1").child("finish").child("entry2").attribute("text").as_string();

	//Quest 2 ------------
	 dialogMission2_ST_B1		= dialogue.child("dialog2").child("start").child("entry1").attribute("text").as_string();
	 dialogMission2_ST_A1		= dialogue.child("dialog2").child("start").child("entry2").attribute("text").as_string();

	 dialogMission2_END_A2		= dialogue.child("dialog2").child("finish").child("entry1").attribute("text").as_string();
	 dialogMission2_END_B2		= dialogue.child("dialog2").child("finish").child("entry2").attribute("text").as_string();

	//Quest 3 ------------
	 dialogMission3_ST_A1		= dialogue.child("dialog3").child("start").child("entry1").attribute("text").as_string();

	 dialogMission3_END_B1		= dialogue.child("dialog3").child("finish").child("entry1").attribute("text").as_string();
	 dialogMission3_END_A2		= dialogue.child("dialog3").child("finish").child("entry2").attribute("text").as_string();
	 dialogMission3_END_B2		= dialogue.child("dialog3").child("finish").child("entry3").attribute("text").as_string();

	//Quest 4 ------------
	 dialogMission4_ST_A1		= dialogue.child("dialog4").child("start").child("entry1").attribute("text").as_string();
	 dialogMission4_ST_B1		= dialogue.child("dialog4").child("start").child("entry2").attribute("text").as_string();

	 dialogMission4_END_C1		= dialogue.child("dialog4").child("finish").child("entry1").attribute("text").as_string();
	 dialogMission4_END_A2		= dialogue.child("dialog4").child("finish").child("entry2").attribute("text").as_string();


	diagDoc.reset();
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


P2SString* ModuleDialogManager::GetCurrentString1() const
{
	return currentDialog1stCharacter;
}


P2SString* ModuleDialogManager::GetCurrentString2() const
{
	return currentDialog2ndCharacter;
}
