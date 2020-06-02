#ifndef __MODULE_DIALOG_MANAGER_H__
#define __MODULE_DIALOG_MANAGER_H__

#include "Module.h"
#include "vector"

enum class DIALOG_INPUT : int
{
	INPUT_ERROR = -1,

	NULL_INPUT,

	SKIP_DIALOG,
	NEXT_DIALOG,

	TUTORIAL_START,
	TUTORIAL_END,

	TUTORIAL2_START,
	TUTORIAL2_END,

	TUTORIAL3_START,
	TUTORIAL3_END,

	MISSION_1_START,
	MISSION_1_END,

	MISSION_2_START,
	MISSION_2_END,

	MISSION_3_START,
	MISSION_3_END,

	MISSION_4_START,
	MISSION_4_END,

};

enum class DIALOG_STATE : int
{
	ERROR_DIALOG = -1,

	IDLE_DIALOG,

	//TUTORIAL-------------
	TUTORIAL_ST,
	TUTORIAL_ST_A2,
	TUTORIAL_ST_B2,
	TUTORIAL_ST_LAST,
	TUTORIAL_END,
	TUTORIAL_END_B1,
	TUTORIAL_END_LAST,
	TUTORIAL2_ST,
	TUTORIAL2_ST_LAST,
	TUTORIAL2_END,
	TUTORIAL2_END_B1,
	TUTORIAL2_END_LAST,
	TUTORIAL3_ST_B1,
	TUTORIAL3_ST_A2,
	TUTORIAL3_ST_LAST,
	TUTORIAL3_END_B1,
	TUTORIAL3_END_LAST,

	//Quest 1 ------------
	MISSION1_ST_B1,
	MISSION1_ST_A1,
	MISSION1_ST_B2,

	MISSION1_END_A2,
	MISSION1_END_B3,

	//Quest 2 ------------
	MISSION2_ST_B1,
	MISSION2_ST_A1,

	MISSION2_END_A2,
	MISSION2_END_B2,

	//Quest 3 ------------
	MISSION3_ST_A1,

	MISSION3_END_B1,
	MISSION3_END_A2,
	MISSION3_END_B2,

	//Quest 4 ------------
	MISSION4_ST_A1,
	MISSION4_ST_A2,

	MISSION4_END_B1,
	MISSION4_END_A3,

};

class ModuleDialogManager : public Module
{
public:

	ModuleDialogManager();
	~ModuleDialogManager();

	bool Awake(pugi::xml_node&);

	bool PreUpdate(float dt);

	bool PushInput(DIALOG_INPUT input);

	P2SString* GetCurrentString1() const;
	P2SString* GetCurrentString2() const;
private:

	void InternalInput(float dt);
	void ProcessFsm();


private:

	P2SString* currentDialog1;
	P2SString* currentDialog2;

	DIALOG_INPUT input;
	DIALOG_STATE state;

	//TUTORIAL-------------
	//Tutorial 1 ---------
	P2SString dialogTutorial1_ST_A1;
	P2SString dialogTutorial1_ST_B1;
	P2SString dialogTutorial1_ST_A2;
	P2SString dialogTutorial1_ST_B2;

	P2SString dialogTutorial1_END_A1;
	P2SString dialogTutorial1_END_B1;

	//Tutorial 2 ---------
	P2SString dialogTutorial2_ST_A1;
	P2SString dialogTutorial2_ST_B1;
	P2SString dialogTutorial2_END_A1;
	P2SString dialogTutorial2_END_B1;

	//Tutorial 3 ---------
	P2SString dialogTutorial3_ST_A1;
	P2SString dialogTutorial3_ST_B1;
	P2SString dialogTutorial3_ST_A2;

	P2SString dialogTutorial3_END_A1;
	P2SString dialogTutorial3_END_B1;

	//Quest 1 ------------
	P2SString dialogMission1_ST_B1;
	P2SString dialogMission1_ST_A1;
	P2SString dialogMission1_ST_B2;

	P2SString dialogMission1_END_A2;
	P2SString dialogMission1_END_B3;

	//Quest 2 ------------
	P2SString dialogMission2_ST_B1;
	P2SString dialogMission2_ST_A1;

	P2SString dialogMission2_END_A2;
	P2SString dialogMission2_END_B2;

	//Quest 3 ------------
	P2SString dialogMission3_ST_A1;

	P2SString dialogMission3_END_B1;
	P2SString dialogMission3_END_A2;
	P2SString dialogMission3_END_B2;

	//Quest 4 ------------
	P2SString dialogMission4_ST_A1;
	P2SString dialogMission4_ST_A2;

	P2SString dialogMission4_END_B1;
	P2SString dialogMission4_END_A3;
};


#endif //__MODULE_DIALOG_MANAGER_H__