#include <iostream> 
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Input.h"

#include "j1Audio.h"

#include "j1App.h"



// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	want_to_save = want_to_load = false;

	//modules
	input = new j1Input();
	win = new j1Window();
	audio = new j1Audio();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	
	AddModule(audio);

	// render last to swap buffer

	
	PERF_PEEK(ptimer);
}

// Destructor
j1App::~j1App()
{
	// release modules
	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool j1App::Awake()
{
	PERF_START(ptimer);
	
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
		
	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());

		frameRate = app_config.attribute("framerate_cap").as_int();
		capFrames = true;
	}

	if(ret == true)
	{
		int numModules = modules.size();

		for (int i = 0; i < numModules; i++)
		{
			ret = modules[i]->Awake(config.child(modules[i]->name.GetString()));
		}
	}


	load_game = config.first_child().child("load").attribute("fileName").as_string();
	save_game = config.first_child().child("load").attribute("fileName").as_string();


	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	int numModules = modules.size();

	for (int i = 0; i < numModules; i++)
	{
		ret = modules[i]->Start();
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();


	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if(result != NULL)
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// TODO 4: Calculate the dt: differential time since last frame
	dt = frameTime.ReadSec() * TIME_CONST;

	//just to when we debug, the player doesnt trespass the floor
	if (dt > MAX_DT)
	{
		dt = MAX_DT;
	}

	frameTime.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	PERF_START(ptimer);

	if (want_to_save == true)
		SavegameNow();

	if (want_to_load == true)
		LoadGameNow();

	// Framerate calculations --

	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	float avg_fps = float(frameCount) / startupTime.ReadSec();
	float seconds_since_startup = startupTime.ReadSec();
	uint32 last_frame_ms = frameTime.Read();
	uint32 frames_on_last_update = prevLastSecFrameCount;

	static char title[256];
	if (capFrames == false)
	{
		/*if (App->render->vsinc == true)
		{
			sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu  Cap On  Vsinc On",
				avg_fps, last_frame_ms, frames_on_last_update, seconds_since_startup, frameCount);
		}

		else
		{
			sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu  Cap On  Vsinc Off",
				avg_fps, last_frame_ms, frames_on_last_update, seconds_since_startup, frameCount);
		}*/
		
	}

	if (capFrames == true)
	{

		/*if (App->render->vsinc == true)
		{
			sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu Cap Off Vsinc On",
				avg_fps, last_frame_ms, frames_on_last_update, seconds_since_startup, frameCount);
		}

		else
		{
			sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu Cap Off  Vsinc Off",
				avg_fps, last_frame_ms, frames_on_last_update, seconds_since_startup, frameCount);
		}*/
		
	}
	

	App->win->SetTitle(title);


	//actuvate / deactivate framrate cap
	/*if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		
		capFrames = !capFrames;
	}*/

	// TODO 2: Use SDL_Delay to make sure you get your capped framerate
	int delay;
	int averageFrame = ((1.0f / frameRate) * 1000);
	delay = averageFrame - last_frame_ms;

	if (delay > 0 && capFrames == true)
	{
		//SDL_Delay(delay);
		
	}

}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	int numModules = modules.size();

	for(int i = 0; i < numModules && ret == true; i++)
	{

		if(modules[i]->active == false) {
			continue;
		}

		ret = modules[i]->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	int numModules = modules.size();

	for (int i = 0; i < numModules && ret == true; i++)
	{
		if(modules[i]->active == false) {
			continue;
		}

		ret = modules[i]->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	int numModules = modules.size();

	for (int i = 0; i < numModules && ret == true; i++)
	{
		if(modules[i]->active == false) {
			continue;
		}

		ret = modules[i]->PostUpdate();
	}

	frames++;		//NOT SURE
	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	int numModules = modules.size();

	for(int i = numModules; i > 0 && ret == true; i--)
	{
		ret = modules[i]->CleanUp();
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void j1App::LoadGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
}

// ---------------------------------------
void j1App::SaveGame() const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
}

// ---------------------------------------


bool j1App::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;


	pugi::xml_parse_result result = data.load_file(load_game.GetString());

	if(result != NULL)
	{

		root = data.child("game_state");

		int numModules = modules.size();
		ret = true;

		for(int i = 0; i < numModules && ret == true; i++)
		{
			ret = modules[i]->Load(root.child(modules[i]->name.GetString()));
		}

		data.reset();
	}
	

	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	int numModules = modules.size();

	for (int i = 0; i < numModules && ret == true; i++)
	{
		ret = modules[i]->Save(root.append_child(modules[i]->name.GetString()));
	}

	if(ret == true)
	{
		data.save_file(save_game.GetString());
	}

	data.reset();
	want_to_save = false;
	return ret;
}