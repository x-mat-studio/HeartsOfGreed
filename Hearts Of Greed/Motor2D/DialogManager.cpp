#include "App.h"
#include "DialogManager.h"
#include "EventManager.h"

ModuleDialogManager::ModuleDialogManager() :

	Module(),

	currentDialog1(nullptr),
	currentDialog2(nullptr),

	//TUTORIAL-------------
	dialogTutorial1_ST_A1(),
	dialogTutorial1_ST_B1(),
	dialogTutorial1_ST_A2(),
	dialogTutorial1_ST_B2(),

	dialogTutorial1_END_A1(),
	dialogTutorial1_END_B1(),

	//Tutorial 2 ---------
	dialogTutorial2_ST_A1(),
	dialogTutorial2_ST_B1(),
	dialogTutorial2_END_A1(),
	dialogTutorial2_END_B1(),

	//Tutorial 3 ---------
	dialogTutorial3_ST_A1(),
	dialogTutorial3_ST_B1(),
	dialogTutorial3_ST_A2(),

	dialogTutorial3_END_A1(),
	dialogTutorial3_END_B1(),

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
	dialogMission4_ST_A2(),

	dialogMission4_END_B1(),
	dialogMission4_END_A3(),

	input(DIALOG_INPUT::NULL_INPUT),
	state(DIALOG_STATE::IDLE_DIALOG)
{
	name.create("dialogManager"); //hahahahaaaha
}


ModuleDialogManager::~ModuleDialogManager()
{

	input = DIALOG_INPUT::INPUT_ERROR;
	state = DIALOG_STATE::ERROR_DIALOG;

	currentDialog1 = nullptr;
	currentDialog2 = nullptr;

	//TUTORIAL-------------
	dialogTutorial1_ST_A1.Clear();
	dialogTutorial1_ST_B1.Clear();
	dialogTutorial1_ST_A2.Clear();
	dialogTutorial1_ST_B2.Clear();

	dialogTutorial1_END_A1.Clear();
	dialogTutorial1_END_B1.Clear();

	//Tutorial 2 ---------
	dialogTutorial2_ST_A1.Clear();
	dialogTutorial2_ST_B1.Clear();
	dialogTutorial2_END_A1.Clear();
	dialogTutorial2_END_B1.Clear();

	//Tutorial 3 ---------
	dialogTutorial3_ST_A1.Clear();
	dialogTutorial3_ST_B1.Clear();
	dialogTutorial3_ST_A2.Clear();

	dialogTutorial3_END_A1.Clear();
	dialogTutorial3_END_B1.Clear();

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
	dialogMission4_ST_A2.Clear();

	dialogMission4_END_B1.Clear();
	dialogMission4_END_A3.Clear();
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
		//Tutorial 1 --------
	dialogTutorial1_ST_A1 = (P2SString)dialogue.child("tutorial1").child("start").child("entry1").attribute("text").as_string();
	dialogTutorial1_ST_B1 = (P2SString)dialogue.child("tutorial1").child("start").child("entry2").attribute("text").as_string();
	dialogTutorial1_ST_A2 = (P2SString)dialogue.child("tutorial1").child("start").child("entry3").attribute("text").as_string();
	dialogTutorial1_ST_B2 = (P2SString)dialogue.child("tutorial1").child("start").child("entry4").attribute("text").as_string();

	dialogTutorial1_END_A1 = (P2SString)dialogue.child("tutorial1").child("finish").child("entry1").attribute("text").as_string();
	dialogTutorial1_END_B1 = (P2SString)dialogue.child("tutorial1").child("finish").child("entry2").attribute("text").as_string();

	//Tutorial 2 --------
	dialogTutorial2_ST_A1 = (P2SString)dialogue.child("tutorial2").child("start").child("entry1").attribute("text").as_string();
	dialogTutorial2_ST_B1 = (P2SString)dialogue.child("tutorial2").child("start").child("entry2").attribute("text").as_string();

	dialogTutorial2_END_A1 = (P2SString)dialogue.child("tutorial2").child("finish").child("entry1").attribute("text").as_string();
	dialogTutorial2_END_B1 = (P2SString)dialogue.child("tutorial2").child("finish").child("entry2").attribute("text").as_string();


	//Tutorial 3 --------
	dialogTutorial3_ST_A1 = (P2SString)dialogue.child("tutorial3").child("start").child("entry1").attribute("text").as_string();
	dialogTutorial3_ST_B1 = (P2SString)dialogue.child("tutorial3").child("start").child("entry2").attribute("text").as_string();
	dialogTutorial3_ST_A2 = (P2SString)dialogue.child("tutorial3").child("start2").child("entry3").attribute("text").as_string();

	dialogTutorial3_END_A1 = (P2SString)dialogue.child("tutorial3").child("finish").child("entry1").attribute("text").as_string();
	dialogTutorial3_END_B1 = (P2SString)dialogue.child("tutorial3").child("finish").child("entry2").attribute("text").as_string();

	//Quest 1 ------------
	dialogMission1_ST_B1 = dialogue.child("dialog1").child("start").child("entry1").attribute("text").as_string();
	dialogMission1_ST_A1 = dialogue.child("dialog1").child("start").child("entry2").attribute("text").as_string();
	dialogMission1_ST_B2 = dialogue.child("dialog1").child("start").child("entry3").attribute("text").as_string();

	dialogMission1_END_A2 = dialogue.child("dialog1").child("finish").child("entry1").attribute("text").as_string();
	dialogMission1_END_B3 = dialogue.child("dialog1").child("finish").child("entry2").attribute("text").as_string();

	//Quest 2 ------------
	dialogMission2_ST_B1 = dialogue.child("dialog2").child("start").child("entry1").attribute("text").as_string();
	dialogMission2_ST_A1 = dialogue.child("dialog2").child("start").child("entry2").attribute("text").as_string();

	dialogMission2_END_A2 = dialogue.child("dialog2").child("finish").child("entry1").attribute("text").as_string();
	dialogMission2_END_B2 = dialogue.child("dialog2").child("finish").child("entry2").attribute("text").as_string();

	//Quest 3 ------------
	dialogMission3_ST_A1 = dialogue.child("dialog3").child("start").child("entry1").attribute("text").as_string();

	dialogMission3_END_B1 = dialogue.child("dialog3").child("finish").child("entry1").attribute("text").as_string();
	dialogMission3_END_A2 = dialogue.child("dialog3").child("finish").child("entry2").attribute("text").as_string();
	dialogMission3_END_B2 = dialogue.child("dialog3").child("finish").child("entry3").attribute("text").as_string();

	//Quest 4 ------------
	dialogMission4_ST_A1 = dialogue.child("dialog4").child("start").child("entry1").attribute("text").as_string();
	dialogMission4_ST_A2 = dialogue.child("dialog4").child("start").child("entry2").attribute("text").as_string();

	dialogMission4_END_B1 = dialogue.child("dialog4").child("finish").child("entry1").attribute("text").as_string();
	dialogMission4_END_A3 = dialogue.child("dialog4").child("finish").child("entry2").attribute("text").as_string();


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
	// HERE GOES ALL TIMERS1_st_

}


void ModuleDialogManager::ProcessFsm()
{
	if (input != DIALOG_INPUT::NULL_INPUT)
	{
		switch (state)
		{
		case DIALOG_STATE::ERROR_DIALOG:
			break;


		case DIALOG_STATE::IDLE_DIALOG:
			currentDialog1 = nullptr;
			currentDialog2 = nullptr;

			switch (input)
			{
			case DIALOG_INPUT::INPUT_ERROR:
				assert("Input Error");
				break;


			case DIALOG_INPUT::NEXT_DIALOG:
				assert("Impossible to go to next dialog");
				break;


			case DIALOG_INPUT::TUTORIAL_START:
				state = DIALOG_STATE::TUTORIAL_ST;

				currentDialog1 = &dialogTutorial1_ST_A1;
				currentDialog2 = nullptr;
				break;


			case DIALOG_INPUT::TUTORIAL_END:
				state = DIALOG_STATE::TUTORIAL_END_B1;

				currentDialog1 = &dialogTutorial1_END_A1;
				currentDialog2 = nullptr;
				break;


			case DIALOG_INPUT::TUTORIAL2_START:
				state = DIALOG_STATE::TUTORIAL2_ST;

				currentDialog1 = &dialogTutorial2_ST_A1;
				currentDialog2 = nullptr;
				break;


			case DIALOG_INPUT::TUTORIAL2_END:
				state = DIALOG_STATE::TUTORIAL2_END_B1;

				currentDialog1 = &dialogTutorial2_END_A1;
				currentDialog2 = nullptr;
				break;


			case DIALOG_INPUT::TUTORIAL3_START:
				state = DIALOG_STATE::TUTORIAL3_ST_B1;

				currentDialog1 = &dialogTutorial3_ST_A1;
				currentDialog2 = nullptr;
				break;


			case DIALOG_INPUT::TUTORIAL3_END:
				state = DIALOG_STATE::TUTORIAL3_END_B1;

				currentDialog1 = &dialogTutorial3_END_A1;
				currentDialog2 = nullptr;
				break;


			case DIALOG_INPUT::MISSION_1_START:
				state = DIALOG_STATE::MISSION1_ST_B1;

				currentDialog1 = nullptr;
				currentDialog2 = &dialogMission1_ST_B1;
				break;


			case DIALOG_INPUT::MISSION_1_END:
				state = DIALOG_STATE::MISSION1_END_A2;

				currentDialog1 = &dialogMission1_END_A2;
				currentDialog2 = nullptr;
				break;


			case DIALOG_INPUT::MISSION_2_START:
				state = DIALOG_STATE::MISSION2_ST_B1;

				currentDialog1 = nullptr;
				currentDialog2 = &dialogMission2_ST_B1;
				break;


			case DIALOG_INPUT::MISSION_2_END:
				state = DIALOG_STATE::MISSION2_END_A2;

				currentDialog1 = &dialogMission2_END_A2;
				currentDialog2 = nullptr;
				break;


			case DIALOG_INPUT::MISSION_3_START:
				state = DIALOG_STATE::MISSION3_ST_A1;

				currentDialog1 = &dialogMission3_ST_A1;
				currentDialog2 = nullptr;
				break;


			case DIALOG_INPUT::MISSION_3_END:
				state = DIALOG_STATE::MISSION3_END_B1;

				currentDialog1 = nullptr;
				currentDialog2 = &dialogMission3_END_B1;
				break;


			case DIALOG_INPUT::MISSION_4_START:
				state = DIALOG_STATE::MISSION4_ST_A1;

				currentDialog1 = &dialogMission4_ST_A1;
				currentDialog2 = nullptr;
				break;


			case DIALOG_INPUT::MISSION_4_END:
				state = DIALOG_STATE::MISSION4_END_B1;

				currentDialog1 = nullptr;
				currentDialog2 = &dialogMission4_END_B1;
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL_ST:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::TUTORIAL_ST_A2;

				currentDialog2 = &dialogTutorial1_ST_B1;

				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL_ST_A2:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::TUTORIAL_ST_B2;

				currentDialog1 = &dialogTutorial1_ST_A2;

				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL_ST_B2:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::TUTORIAL_ST_LAST;

				currentDialog2 = &dialogTutorial1_ST_B2;

				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL_ST_LAST:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::TUTORIAL_END_B1;

				currentDialog1 = &dialogTutorial1_END_A1;

				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL_END_B1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::TUTORIAL_END_LAST;

				currentDialog2 = &dialogTutorial1_END_B1;

				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL_END_LAST:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL2_ST:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::TUTORIAL2_ST_LAST;

				currentDialog2 = &dialogTutorial2_ST_B1;

				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL2_ST_LAST:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);


				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL2_END_B1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::TUTORIAL2_END_LAST;

				currentDialog2 = &dialogTutorial2_END_B1;

				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL2_END_LAST:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);

				app->eventManager->GenerateEvent(EVENT_ENUM::TUTORIAL3_START, EVENT_ENUM::NULL_EVENT);

				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL3_ST_B1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::TUTORIAL3_ST_A2;

				currentDialog2 = &dialogTutorial3_ST_B1;

				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL3_ST_A2:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::TUTORIAL3_ST_LAST;

				currentDialog1 = &dialogTutorial3_ST_A2;

				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL3_ST_LAST:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL3_END_B1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::TUTORIAL3_END_LAST;

				currentDialog2 = &dialogTutorial3_END_B1;

				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::TUTORIAL3_END_LAST:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION1_ST_B1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::MISSION1_ST_A1;

				currentDialog1 = &dialogMission1_ST_A1;
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION1_ST_A1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::MISSION1_ST_B2;

				currentDialog2 = &dialogMission1_ST_B2;
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION1_ST_B2:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION1_END_A2:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::MISSION1_END_B3;

				currentDialog2 = &dialogMission1_END_B3;
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION1_END_B3:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION2_ST_B1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::MISSION2_ST_A1;

				currentDialog1 = &dialogMission2_ST_A1;
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION2_ST_A1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION2_END_A2:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::MISSION2_END_B2;

				currentDialog2 = &dialogMission2_END_B2;
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION2_END_B2:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION3_ST_A1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION3_END_B1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::MISSION3_END_A2;

				currentDialog1 = &dialogMission3_END_A2;
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION3_END_A2:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::MISSION3_END_B2;

				currentDialog2 = &dialogMission3_END_B2;
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION3_END_B2:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION4_ST_A1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::MISSION4_ST_A2;

				currentDialog1 = &dialogMission4_ST_A2;
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION4_ST_A2:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;


		case DIALOG_STATE::MISSION4_END_B1:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::MISSION4_END_A3;

				currentDialog1 = &dialogMission4_END_A3;
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;
		case DIALOG_STATE::MISSION4_END_A3:

			switch (input)
			{
			case DIALOG_INPUT::NEXT_DIALOG:
				state = DIALOG_STATE::IDLE_DIALOG;

				currentDialog1 = nullptr;
				currentDialog2 = nullptr;

				app->eventManager->GenerateEvent(EVENT_ENUM::CLOSE_DIALOG_WINDOW, EVENT_ENUM::NULL_EVENT);
				break;
			}

			input = DIALOG_INPUT::NULL_INPUT;
			break;
		default:
			break;
		}
	}

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
	return currentDialog1;
}


P2SString* ModuleDialogManager::GetCurrentString2() const
{
	return currentDialog2;
}
