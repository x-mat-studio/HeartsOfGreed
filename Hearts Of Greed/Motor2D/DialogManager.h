#ifndef __MODULE_DIALOG_MANAGER_H__
#define __MODULE_DIALOG_MANAGER_H__

#include "Module.h"
#include "vector"

enum class DIALOG_INPUT : int
{
	INPUT_ERROR = -1,

	NULL_INPUT,

	NEXT_DIALOG,
	END_DIALOG,

	TUTORIAL_START,
	TUTORIAL_END,

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

	//Tutorial
	TUTORIAL_START,
	TUTORIAL_END,

	//Mission 1
	MISSION_1_ST_B1,
	MISSION_1_ST_A1,
	MISSION_1_ST_B2,

	MISSION_1_END_B3,
	MISSION_1_END_A2,

	//Mission2
	MISSION_2_ST_C1,
	MISSION_2_ST_A1,

	MISSION_2_END_C2,
	MISSION_2_END_A2,
	MISSION_2_END_C3,
	MISSION_2_END_A3,

	//Mission3




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

	P2SString* currentDialog1stCharacter;
	P2SString* currentDialog2ndCharacter;
	
	DIALOG_INPUT input;
	DIALOG_STATE state;

	//TUTORIAL-------------
	P2SString dialogTutorialStart;
	P2SString dialogTutorialEnd;

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
	P2SString dialogMission4_ST_B1;

	P2SString dialogMission4_END_C1;
	P2SString dialogMission4_END_A2;
};


#endif //__MODULE_DIALOG_MANAGER_H__