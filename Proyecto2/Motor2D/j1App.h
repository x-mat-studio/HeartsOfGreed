#ifndef __j1APP_H__
#define __j1APP_H__

#define TIME_CONST 50
#define MAX_DT 3

#include "j1Module.h"
#include "PerfectTimer.h"
#include "SimpleTimer.h"
#include "vector"
#include "PugiXml\src\pugixml.hpp"

typedef unsigned int uint;

// Modules
class j1Window;
class j1Input;
class j1Audio;
class j1Render;


class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win;
	
	j1Input*			input;
	
	j1Audio*			audio;
	
	j1Render*			render;
	
	uint32_t			frames;
	
	uint				frameRate = 0;

	bool				vSyncActivated;

private:
	std::vector<j1Module*>	modules;

	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;
	p2SString			load_game;
	mutable p2SString	save_game;

	PerfectTimer		ptimer;
	
	SimpleTimer			startupTime;
	SimpleTimer			frameTime;
	SimpleTimer			lastSecFrameTime;
	
	uint64				frameCount = 0;
	
	uint32				lastSecFrameCount = 0;
	uint32				prevLastSecFrameCount = 0;

	bool				capFrames;
	bool				want_to_load;
	mutable bool		want_to_save;

	float				dt;
};

extern j1App* App; 

#endif