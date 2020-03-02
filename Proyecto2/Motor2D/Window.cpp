#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Window.h"
#include "SDL/include/SDL.h"
#include "Brofiler/Brofiler/Brofiler.h"


ModuleWindow::ModuleWindow() : Module()
{
	window = NULL;
	screenSurface = NULL;
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
		Uint32 flags = SDL_WINDOW_SHOWN;
		bool fullscreen = config.child("fullscreen").attribute("value").as_bool(false);
		bool borderless = config.child("borderless").attribute("value").as_bool(false);
		bool resizable = config.child("resizable").attribute("value").as_bool(false);
		bool fullscreenWindow = config.child("fullscreen_window").attribute("value").as_bool(false);

		width = config.child("resolution").attribute("width").as_int(640);
		height = config.child("resolution").attribute("height").as_int(480);
		scale = config.child("resolution").attribute("scale").as_float(1.0);


		if(fullscreen == true)
			flags |= SDL_WINDOW_FULLSCREEN;


		if(borderless == true)
			flags |= SDL_WINDOW_BORDERLESS;


		if(resizable == true)
			flags |= SDL_WINDOW_RESIZABLE;


		if(fullscreenWindow == true)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;


		window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);


		if(window == NULL)
			ret = false;
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}


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