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

	P2SString* GetCurrentString() const;
private:

	void InternalInput(float dt);
	void ProcessFsm();


private:

	P2SString* currentDialog;
	//Pay attention to the dialog nomenclature

	P2SString dialogTutorialStart;
	P2SString dialogTutorialEnd;


	P2SString dialogMission1_ST_B1;
	P2SString dialogMission1_ST_A1;
	P2SString dialogMission1_ST_B2;

	P2SString dialogMission1_END_B3;
	P2SString dialogMission1_END_A2;

	DIALOG_INPUT input;
	DIALOG_STATE state;



};


#endif //__MODULE_DIALOG_MANAGER_H__