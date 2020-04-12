#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "SDL/include/SDL.h"
#include "Brofiler/Brofiler/Brofiler.h"


ModuleWindow::ModuleWindow() : Module(), scale(.0f),minScaleValue(.0f),maxScaleValue(.0f),height(0u),width(0u),window(NULL),screenSurface(NULL)
{
	name.create("window");
}


// Destructor
ModuleWindow::~ModuleWindow()
{
}


// Called before render is available
bool ModuleWindow::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("Window Awake", Profiler::Color::Crimson);
	bool ret = true;


	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		ret = false;
	else
	{
		//Create window
		
		bool fullscreen = config.child("fullscreen").attribute("value").as_bool(false);
		bool borderless = config.child("borderless").attribute("value").as_bool(false);
		bool resizable = config.child("resizable").attribute("value").as_bool(false);
		bool fullscreenWindow = config.child("fullscreen_window").attribute("value").as_bool(false);

		width = config.child("resolution").attribute("width").as_int(640);
		height = config.child("resolution").attribute("height").as_int(480);
		scale = config.child("resolution").attribute("scale").as_float(1.0);
		minScaleValue = config.child("resolution").attribute("minScaleValue").as_float(1.0);
		maxScaleValue = config.child("resolution").attribute("maxScaleValue").as_float(1.0);
		
		stateResolution = RESOLUTION_MODE::STATIC;
		
		if (fullscreen == true)
			stateResolution = RESOLUTION_MODE::FULLSCREEN;

		if(borderless == true)
			stateResolution = RESOLUTION_MODE::BORDERLESS;

		if(resizable == true)
			stateResolution = RESOLUTION_MODE::RESIZABLE;

		if(fullscreenWindow == true)
			stateResolution = RESOLUTION_MODE::FULLSCREEN_WINDOW;

		ret = ChangeWindow(stateResolution);
	}

	return ret;
}


// Called before quitting
bool ModuleWindow::CleanUp()
{

	//Destroy window
	if(window != NULL)
		SDL_DestroyWindow(window);


	window = nullptr;
	screenSurface = nullptr;

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

bool ModuleWindow::Update(float dt)
{
	bool ret = true; 
	
	//ONCE we have UI this should be menu events
	
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_STATE::KEY_DOWN) {

		ChangeResolution(RESOLUTION_MODE::FULLSCREEN);

	}
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_STATE::KEY_DOWN) {

		ChangeResolution(RESOLUTION_MODE::STATIC);

	}

	return ret;
}


SDL_Window* ModuleWindow::ResizeWindow(RESOLUTION_MODE stateResolution)
{
	window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SetResolutionFlag(stateResolution));

	return window;
}


bool ModuleWindow::AssignSurface(SDL_Window * window)
{
	if (window == NULL)
		return false;
	else
	{
		//Get window surface
		screenSurface = SDL_GetWindowSurface(window);
		return true;
	}
}


bool ModuleWindow::ChangeWindow(RESOLUTION_MODE stateResolution)
{
	SDL_Window* newWindow = ResizeWindow(stateResolution);

	bool changeRet = AssignSurface(newWindow);

	return changeRet;
}


int ModuleWindow::SetResolutionFlag(RESOLUTION_MODE stateResolution)
{
	Uint32 flags = 0;

	switch (stateResolution)
	{
	case RESOLUTION_MODE::FULLSCREEN:
		flags |= SDL_WINDOW_FULLSCREEN;
		break;

	case RESOLUTION_MODE::BORDERLESS:
		flags |= SDL_WINDOW_BORDERLESS;
		break;

	case RESOLUTION_MODE::RESIZABLE:
		flags |= SDL_WINDOW_RESIZABLE;
		break;

	case RESOLUTION_MODE::FULLSCREEN_WINDOW:
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		break;

	case RESOLUTION_MODE::STATIC:
		flags |= SDL_WINDOW_SHOWN;
		break;

	default:
		flags |= SDL_WINDOW_SHOWN;
		break;
	}

	return flags;
}


bool ModuleWindow::ChangeResolution(RESOLUTION_MODE newResolution)
{
	bool ret = false;

	if (stateResolution != newResolution) {

		stateResolution = newResolution;

		SDL_SetWindowFullscreen(window, SetResolutionFlag(stateResolution));

		app->render->AssignCameraMeasures();

		ret = true;
	}

	return ret;
}


// Set new window title
void ModuleWindow::SetTitle(const char* new_title)
{
	SDL_SetWindowTitle(window, new_title);
}


void ModuleWindow::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}


float ModuleWindow::GetScale() const
{
	return scale;
}

float ModuleWindow::GetUIScale() const
{
	// UI Scale is always the same.
	return 2.0f;
}


float ModuleWindow::SetScale(float newScale)
{
	scale = newScale;
	
	
	if (scale < minScaleValue)
		scale = minScaleValue;
	else if (scale > maxScaleValue)
		scale = maxScaleValue;

	
	return scale;
}


float  ModuleWindow::AddScale(float addedScale)
{
	scale += addedScale;


	if (scale < minScaleValue)
		scale = minScaleValue;
	else if (scale > maxScaleValue)
		scale = maxScaleValue;


	return scale;
}
