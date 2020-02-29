#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "SDL/include/SDL.h"
#include "Brofiler/Brofiler/Brofiler.h"

#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name.create("input");

	inputTexActivated = false;

	keyboard = new j1KeyState[MAX_KEYS];

	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);

	cursorPos = 0;
}

// Destructor
j1Input::~j1Input()
{
	delete[] keyboard;
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("Input Awake", Profiler::Color::DarkMagenta)
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	BROFILER_CATEGORY("Input Start", Profiler::Color::DarkOliveGreen)
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	BROFILER_CATEGORY("Input pre-update", Profiler::Color::Blue)
	static SDL_Event event;
	
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if(mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_TEXTINPUT:
				/* Add new text onto the end of our text */
				if (cursorPos == 0)
				{
					text += event.text.text;
				}

				else if (cursorPos > 0)
				{
					text.Insert(event.text.text, cursorPos);
				}

				break;

			case SDL_MOUSEBUTTONDOWN:
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

			case SDL_MOUSEBUTTONUP:
				mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

			case SDL_MOUSEMOTION:
				int scale = App->win->GetScale();
				mouse_motion_x = event.motion.xrel / scale;
				mouse_motion_y = event.motion.yrel / scale;
				mouse_x = event.motion.x / scale;
				mouse_y = event.motion.y / scale;
			break;
		}
	}

	HandleTextInput();

	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}


void j1Input::ActivateTextInput(SDL_Rect &rect) {

	SDL_StartTextInput();
	SDL_SetTextInputRect(&rect);
	inputTexActivated = true;
}


void j1Input::DesactivateTextInput() {

	SDL_StopTextInput();
	text.Clear();
	inputTexActivated = false;
}



void j1Input::HandleTextInput() {

	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		text.DeleteLast();
	}

}

const char* j1Input::getInputText() {

	return text.GetString();
}