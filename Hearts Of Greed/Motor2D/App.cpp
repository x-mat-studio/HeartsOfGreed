#include <iostream> 
#include "p2Defs.h"
#include "p2Log.h"
#include "Window.h"
#include "Input.h"
#include "Textures.h"
#include "Collision.h"
#include "Audio.h"
#include "Render.h"
#include "Map.h"
#include "TestScene.h"
#include "Player.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "UIManager.h"
#include "Pathfinding.h"
#include "AI.h"
#include "FoWManager.h"
#include "Fonts.h"
#include "App.h"
#include "FadeToBlack.h"
#include "IntroScene.h"
#include "MainMenuScene.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "Minimap.h"


// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(pTimer);

	wantToSave = wantToLoad = false;

	//modules
	input = new ModuleInput();
	win = new ModuleWindow();
	audio = new ModuleAudio();
	render = new ModuleRender();
	map = new ModuleMap();
	tex = new ModuleTextures();
	coll = new ModuleCollision();
	entityManager = new ModuleEntityManager();
	eventManager = new ModuleEventManager();
	uiManager = new ModuleUIManager();
	player = new ModulePlayer();
	ai = new ModuleAI();
	pathfinding = new ModulePathfinding();
	fonts = new ModuleFonts();
	fowManager = new ModuleFoWManager();
	fadeToBlack = new ModuleFadeToBlack();

	introScene = new ModuleIntroScene();
	mainMenu = new ModuleMainMenuScene();
	testScene = new ModuleTestScene();
	winScene = new ModuleWinScene();
	loseScene = new ModuleLoseScene();
	minimap = new Minimap();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(eventManager);
	AddModule(input);
	AddModule(win);
	AddModule(audio);
	AddModule(tex);
	AddModule(fonts);
	AddModule(map);
	AddModule(mainMenu);
	AddModule(testScene);
	AddModule(winScene);
	AddModule(loseScene);
	AddModule(introScene);
	AddModule(coll);
	AddModule(fowManager);
	AddModule(entityManager);
	AddModule(uiManager);
	AddModule(pathfinding);
	AddModule(player);
	AddModule(ai);

	AddModule(minimap);

	//Fade to black before render
	AddModule(fadeToBlack);
	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(pTimer);
}

// Destructor
App::~App()
{
	// release modules
	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(pTimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;

	config = LoadConfig(config_file);


	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());

		frameRate = app_config.attribute("framerate_cap").as_int();
		capFrames = true;
	}


	if (ret == true)
	{
		int numModules = modules.size();


		for (int i = 0; i < numModules; i++)
		{
			ret = modules[i]->Awake(config.child(modules[i]->name.GetString()));
		}


	}


	loadGame = config.first_child().child("load").attribute("fileName").as_string();
	saveGame = config.first_child().child("load").attribute("fileName").as_string();
	//Set disabled modules here-------------
	
	//Scenes
	//introScene->Disable();
	mainMenu->Disable();
	winScene->Disable();
	loseScene->Disable();
	testScene->Disable();
	
	//Other
	player->Disable();
	minimap->Disable();
	//-----------------------------------
	PERF_PEEK(pTimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	int numModules = modules.size();

	uiManager->LoadAtlas();

	for (int i = 0; i < numModules; i++)
	{
		if (modules[i]->IsEnabled())
		{
			ret = modules[i]->Start();
		}
	}


	return ret;
}

// Called each loop iteration
bool App::Update()
{

	bool ret = true;
	PrepareUpdate();


	if (input->GetWindowEvent(EVENT_WINDOW::WE_QUIT) == true)
		ret = false;


	if (ret == true)
		ret = PreUpdate();


	if (ret == true)
		ret = DoUpdate();


	if (ret == true)
		ret = PostUpdate();


	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");


	if (result != NULL)
		ret = config_file.child("config");


	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// Calculate the dt
	dt = frameTime.ReadSec();


	//just to when we debug, the player doesnt trespass the floor
	if (dt > MAX_DT)
		dt = MAX_DT;


	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	PERF_START(pTimer);


	if (wantToSave == true)
		SavegameNow();


	if (wantToLoad == true)
		LoadGameNow();


	// Framerate calculations --


	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}


	float avgFps = float(frameCount) / startupTime.ReadSec();
	float secondsSinceStartup = startupTime.ReadSec();
	uint32 lastFrameMs = frameTime.ReadSec();
	uint32 framesOnLastUpdate = prevLastSecFrameCount;

	int mouseX, mouseY = 0;
	input->GetMousePosition(mouseX, mouseY);
	static char title[256];
	sprintf_s(title, 256, " Hearts of Greed || Camera X: %i || Camera Y: %i  || Mouse X:%f  Y:%f",
		app->render->GetCameraX(), app->render->GetCameraY(),
		(-app->render->currentCamX + mouseX) / app->win->GetScale(),
		(-app->render->currentCamY + mouseY) / app->win->GetScale()
	);

	app->win->SetTitle(title);

	if (capFrames == false)
	{
		/*if (App->render->vsinc == true)
		{
			sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu  Cap On  Vsinc On",
				avgFps, lastFrameMs, framesOnLastUpdate, secondsSinceStartup, frameCount);
		}

		else
		{
			sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu  Cap On  Vsinc Off",
				avgFps, lastFrameMs, framesOnLastUpdate, secondsSinceStartup, frameCount);
		}*/

	}


	if (capFrames == true)
	{

		/*if (App->render->vsinc == true)
		{
			sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu Cap Off Vsinc On",
				avgFps, lastFrameMs, framesOnLastUpdate, secondsSinceStartup, frameCount);
		}

		else
		{
			sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu Cap Off  Vsinc Off",
				avgFps, lastFrameMs, framesOnLastUpdate, secondsSinceStartup, frameCount);
		}*/

	}

	//actuvate / deactivate framrate cap
	/*if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {

		capFrames = !capFrames;
	}*/

	int delay;
	int averageFrame = ((1.0f / frameRate) * 1000);
	delay = averageFrame - lastFrameMs;


	if (delay > 0 && capFrames == true)
	{
		//SDL_Delay(delay);

	}


}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	int numModules = modules.size();


	for (int i = 0; i < numModules && ret == true; i++)
	{


		if (modules[i]->IsEnabled() == false)
			continue;


		ret = modules[i]->PreUpdate(dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_STATE::KEY_DOWN) {

		debugMode = !debugMode;
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	int numModules = modules.size();


	for (int i = 0; i < numModules && ret == true; i++)
	{


		if (modules[i]->IsEnabled() == false)
		{
			continue;
		}


		ret = modules[i]->Update(dt);
	}


	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	int numModules = modules.size();

	for (int i = 0; i < numModules && ret == true; i++)
	{
		if (modules[i]->IsEnabled() == false) {
			continue;
		}

		ret = modules[i]->PostUpdate(dt);
	}

	frames++;		//NOT SURE
	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	int numModules = modules.size();


	for (int i = numModules - 1; i >= 0 && ret == true; i--)
	{
		ret = modules[i]->CleanUp();
	}


	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{


	if (index < argc)
		return args[index];
	else
		return NULL;


}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	wantToLoad = true;
}

// ---------------------------------------
void App::SaveGame() const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	wantToSave = true;
}

// ---------------------------------------


bool App::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;


	pugi::xml_parse_result result = data.load_file(loadGame.GetString());

	if (result != NULL)
	{

		root = data.child("game_state");

		int numModules = modules.size();
		ret = true;

		for (int i = 0; i < numModules && ret == true; i++)
		{
			ret = modules[i]->Load(root.child(modules[i]->name.GetString()));
		}

		data.reset();
	}


	wantToLoad = false;
	return ret;
}

bool App::SavegameNow() const
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

	if (ret == true)
	{
		data.save_file(saveGame.GetString());
	}

	data.reset();
	wantToSave = false;
	return ret;
}