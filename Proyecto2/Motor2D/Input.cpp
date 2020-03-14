#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Input.h"
#include "Window.h"
#include "SDL/include/SDL.h"
#include "Brofiler/Brofiler/Brofiler.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput() : Module(), inputTexActivated(false), keyboard(new KEY_STATE[MAX_KEYS]), cursorPos(0)
{
	name.create("input");

	memset(keyboard, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouseButtons, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("Input Awake", Profiler::Color::DarkMagenta);

	bool ret = true;

	SDL_Init(0);


	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
		ret = false;


	return ret;
}

// Called before the first frame
bool ModuleInput::Start()
{
	BROFILER_CATEGORY("Input Start", Profiler::Color::DarkOliveGreen);

	SDL_StopTextInput();

	return true;
}

// Called each loop iteration
bool ModuleInput::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Input pre-update", Profiler::Color::Blue);

	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	mouseWheelMotionX = 0;
	mouseWheelMotionY = 0;

	for (int i = 0; i < MAX_KEYS; ++i)
	{


		if (keys[i] == 1)
		{


			if (keyboard[i] == KEY_STATE::KEY_IDLE)
				keyboard[i] = KEY_STATE::KEY_DOWN;
			else
				keyboard[i] = KEY_STATE::KEY_REPEAT;


		}
		else
		{


			if (keyboard[i] == KEY_STATE::KEY_REPEAT || keyboard[i] == KEY_STATE::KEY_DOWN)
				keyboard[i] = KEY_STATE::KEY_UP;
			else
				keyboard[i] = KEY_STATE::KEY_IDLE;


		}


	}


	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{


		if (mouseButtons[i] == KEY_STATE::KEY_DOWN)
			mouseButtons[i] = KEY_STATE::KEY_REPEAT;


		if (mouseButtons[i] == KEY_STATE::KEY_UP)
			mouseButtons[i] = KEY_STATE::KEY_IDLE;


	}


	while (SDL_PollEvent(&event) != 0)
	{


		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[(int)EVENT_WINDOW::WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:


			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[(int)EVENT_WINDOW::WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[(int)EVENT_WINDOW::WE_SHOW] = true;
				break;
			}


			break;

		case SDL_TEXTINPUT:
			/* Add new text onto the end of our text */


			if (cursorPos == 0)
				text += event.text.text;
			else if (cursorPos > 0)
				text.Insert(event.text.text, cursorPos);


			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseButtons[event.button.button - 1] = KEY_STATE::KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouseButtons[event.button.button - 1] = KEY_STATE::KEY_UP;
			break;

		case SDL_MOUSEWHEEL:

			mouseWheelMotionX = event.wheel.x;
			mouseWheelMotionY = event.wheel.y;
			break;

		case SDL_MOUSEMOTION:
			float scale = app->win->GetScale();
			mouseMotionX = event.motion.xrel / scale;
			mouseMotionY = event.motion.yrel / scale;
			mouseX = event.motion.x / scale;
			mouseY = event.motion.y / scale;
			mouseXRaw = event.motion.x;
			mouseYRaw = event.motion.y;

			break;


		}


	}

	HandleTextInput();

	return true;
}


// Called before quitting
bool ModuleInput::CleanUp()
{
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}


// ---------
bool ModuleInput::GetWindowEvent(EVENT_WINDOW ev)
{
	return windowEvents[(int)ev];
}


void ModuleInput::GetMousePosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}


void ModuleInput::GetMousePositionRaw(int& x, int& y)
{
	x = mouseXRaw;
	y = mouseYRaw;
}


void ModuleInput::GetMouseMotion(int& x, int& y)
{
	x = mouseMotionX;
	y = mouseMotionY;
}


void ModuleInput::GetScrollWheelMotion(int& x, int& y)
{
	x = mouseWheelMotionX;
	y = mouseWheelMotionY;
}


void ModuleInput::ActivateTextInput(SDL_Rect& rect)
{
	SDL_StartTextInput();
	SDL_SetTextInputRect(&rect);
	inputTexActivated = true;
}


void ModuleInput::DesactivateTextInput()
{
	SDL_StopTextInput();
	text.Clear();
	inputTexActivated = false;
}


void ModuleInput::HandleTextInput()
{

	if (app->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_STATE::KEY_DOWN)
		text.DeleteLast();

}


const char* ModuleInput::GetInputText()
{
	return text.GetString();
}