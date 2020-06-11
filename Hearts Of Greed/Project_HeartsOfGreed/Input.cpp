#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Input.h"
#include "Window.h"
#include "SDL/include/SDL.h"
#include "Brofiler/Brofiler/Brofiler.h"
#include "EventManager.h"
#include "Window.h"
#include "Render.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput() : Module(), inputTexActivated(false), keyboard(new KEY_STATE[MAX_KEYS]), cursorPos(0), keybindings(new EventsOnKeyPress[MAX_KEYS])
{
	name.create("input");

	memset(keyboard, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouseButtons, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
	delete[] keybindings;
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

	//configure bindings (will be loaded from xml in the future) TODO
	AddMouseBinding(SDL_BUTTON_LEFT - 1, KEY_STATE::KEY_DOWN, EVENT_ENUM::ENTITY_INTERACTION);
	AddMouseBinding(SDL_BUTTON_LEFT - 1, KEY_STATE::KEY_REPEAT, EVENT_ENUM::SELECT_UNITS);
	AddMouseBinding(SDL_BUTTON_LEFT - 1, KEY_STATE::KEY_UP, EVENT_ENUM::STOP_SELECTING_UNITS);
	AddMouseBinding(SDL_BUTTON_RIGHT - 1, KEY_STATE::KEY_DOWN, EVENT_ENUM::ENTITY_COMMAND);

	//Camera bindings
	AddKeyBinding(SDL_SCANCODE_W, KEY_STATE::KEY_DOWN, EVENT_ENUM::CAMERA_UP);
	AddKeyBinding(SDL_SCANCODE_W, KEY_STATE::KEY_UP, EVENT_ENUM::STOP_CAMERA_UP);
	AddKeyBinding(SDL_SCANCODE_S, KEY_STATE::KEY_DOWN, EVENT_ENUM::CAMERA_DOWN);
	AddKeyBinding(SDL_SCANCODE_S, KEY_STATE::KEY_UP, EVENT_ENUM::STOP_CAMERA_DOWN);
	AddKeyBinding(SDL_SCANCODE_A, KEY_STATE::KEY_DOWN, EVENT_ENUM::CAMERA_LEFT);
	AddKeyBinding(SDL_SCANCODE_A, KEY_STATE::KEY_UP, EVENT_ENUM::STOP_CAMERA_LEFT);
	AddKeyBinding(SDL_SCANCODE_D, KEY_STATE::KEY_DOWN, EVENT_ENUM::CAMERA_RIGHT);
	AddKeyBinding(SDL_SCANCODE_D, KEY_STATE::KEY_UP, EVENT_ENUM::STOP_CAMERA_RIGHT);
	AddKeyBinding(SDL_SCANCODE_LSHIFT, KEY_STATE::KEY_DOWN, EVENT_ENUM::CAMERA_SPRINT);
	AddKeyBinding(SDL_SCANCODE_LSHIFT, KEY_STATE::KEY_UP, EVENT_ENUM::STOP_CAMERA_SPRINT);
	AddKeyBinding(SDL_SCANCODE_TAB, KEY_STATE::KEY_DOWN, EVENT_ENUM::HERO_CHANGE_FOCUS);

	AddKeyBinding(SDL_SCANCODE_ESCAPE, KEY_STATE::KEY_DOWN, EVENT_ENUM::EXIT_MENUS);
	AddKeyBinding(SDL_SCANCODE_ESCAPE, KEY_STATE::KEY_UP, EVENT_ENUM::EXIT_CONSTRUCTION_MODE);

	//Skill bindings
	AddKeyBinding(SDL_SCANCODE_E, KEY_STATE::KEY_DOWN, EVENT_ENUM::SKILL1);
	AddKeyBinding(SDL_SCANCODE_R, KEY_STATE::KEY_DOWN, EVENT_ENUM::SKILL2);
	AddKeyBinding(SDL_SCANCODE_T, KEY_STATE::KEY_DOWN, EVENT_ENUM::SKILL3);

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
			{
				keyboard[i] = KEY_STATE::KEY_DOWN;
				keyBindingSendEvent(i, KEY_STATE::KEY_DOWN);
			}
			else
			{
				keyboard[i] = KEY_STATE::KEY_REPEAT;
				keyBindingSendEvent(i, KEY_STATE::KEY_REPEAT);
			}


		}
		else
		{


			if (keyboard[i] == KEY_STATE::KEY_REPEAT || keyboard[i] == KEY_STATE::KEY_DOWN)
			{
				keyboard[i] = KEY_STATE::KEY_UP;
				keyBindingSendEvent(i, KEY_STATE::KEY_UP);

			}
			else
			{
				keyboard[i] = KEY_STATE::KEY_IDLE;
				keyBindingSendEvent(i, KEY_STATE::KEY_IDLE);
			}


		}


	}


	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{


		if (mouseButtons[i] == KEY_STATE::KEY_DOWN)
		{
			mouseButtons[i] = KEY_STATE::KEY_REPEAT;
			mouseBindingSendEvent(i, KEY_STATE::KEY_REPEAT);
		}


		if (mouseButtons[i] == KEY_STATE::KEY_UP)
		{
			mouseButtons[i] = KEY_STATE::KEY_IDLE;
			mouseBindingSendEvent(i, KEY_STATE::KEY_IDLE);
		}


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
				app->eventManager->GenerateEvent(EVENT_ENUM::FULLSCREEN_REGAIN_FOCUS, EVENT_ENUM::NULL_EVENT);
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
			mouseBindingSendEvent(event.button.button - 1, KEY_STATE::KEY_DOWN);
			break;

		case SDL_MOUSEBUTTONUP:
			mouseButtons[event.button.button - 1] = KEY_STATE::KEY_UP;
			mouseBindingSendEvent(event.button.button - 1, KEY_STATE::KEY_UP);
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

	HandleDebugKeys();

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


void ModuleInput::GetMouseRelPosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}


void ModuleInput::GetMouseRelPositionRaw(int& x, int& y)
{
	x = mouseXRaw;
	y = mouseYRaw;
}

fMPoint ModuleInput::GetMousePosScaled() const
{
	float scale =app->win->GetScale();
	iMPoint aux;
	fMPoint ret;
	SDL_GetMouseState(&aux.x, &aux.y);
	ret.x = round((float)aux.x / scale);
	ret.y = round((float)aux.y / scale);
	return ret;
}

iMPoint ModuleInput::GetMousePosScreen() const
{
	iMPoint ret;
	SDL_GetMouseState(&ret.x, &ret.y);
	return ret;
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

fMPoint ModuleInput::GetMousePosWorld() const
{
	float scale = app->win->GetScale();
	iMPoint mouseAux = GetMousePosScreen();
	fMPoint ret;
	ret.x = (-app->render->currentCamX + mouseAux.x) / scale;
	ret.y = (-app->render->currentCamY + mouseAux.y) / scale;
	return ret;
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


void ModuleInput::AddKeyBinding(int key, KEY_STATE keyAction, EVENT_ENUM event)
{


	switch (keyAction)
	{
	case KEY_STATE::KEY_IDLE:
		keybindings[key].keyIdle = event;
		break;

	case KEY_STATE::KEY_DOWN:
		keybindings[key].keyDown = event;
		break;

	case KEY_STATE::KEY_REPEAT:
		keybindings[key].keyRepeat = event;
		break;

	case KEY_STATE::KEY_UP:
		keybindings[key].keyUp = event;
		break;

	}


}


void ModuleInput::AddMouseBinding(int buttonId, KEY_STATE buttonAction, EVENT_ENUM event)
{
	switch (buttonAction)
	{
	case KEY_STATE::KEY_IDLE:
		mouseBindings[buttonId].keyIdle = event;
		break;

	case KEY_STATE::KEY_DOWN:
		mouseBindings[buttonId].keyDown = event;
		break;

	case KEY_STATE::KEY_REPEAT:
		mouseBindings[buttonId].keyRepeat = event;
		break;

	case KEY_STATE::KEY_UP:
		mouseBindings[buttonId].keyUp = event;
		break;

	}
}


void ModuleInput::RemoveSingleKeyBinding(int key, KEY_STATE keyAction)
{


	switch (keyAction)
	{
	case KEY_STATE::KEY_IDLE:
		keybindings[key].keyIdle = EVENT_ENUM::NULL_EVENT;
		break;

	case KEY_STATE::KEY_DOWN:
		keybindings[key].keyDown = EVENT_ENUM::NULL_EVENT;
		break;

	case KEY_STATE::KEY_REPEAT:
		keybindings[key].keyRepeat = EVENT_ENUM::NULL_EVENT;
		break;

	case KEY_STATE::KEY_UP:
		keybindings[key].keyUp = EVENT_ENUM::NULL_EVENT;
		break;

	}


}


void ModuleInput::RemoveKeyBinding(int key)
{

	keybindings[key].keyDown = EVENT_ENUM::NULL_EVENT;
	keybindings[key].keyIdle = EVENT_ENUM::NULL_EVENT;
	keybindings[key].keyRepeat = EVENT_ENUM::NULL_EVENT;
	keybindings[key].keyUp = EVENT_ENUM::NULL_EVENT;

}



void ModuleInput::RemoveSingleMouseBinding(int buttonId, KEY_STATE keyAction)
{

	switch (keyAction)
	{
	case KEY_STATE::KEY_IDLE:
		mouseBindings[buttonId].keyIdle = EVENT_ENUM::NULL_EVENT;
		break;

	case KEY_STATE::KEY_DOWN:
		mouseBindings[buttonId].keyDown = EVENT_ENUM::NULL_EVENT;
		break;

	case KEY_STATE::KEY_REPEAT:
		mouseBindings[buttonId].keyRepeat = EVENT_ENUM::NULL_EVENT;
		break;

	case KEY_STATE::KEY_UP:
		mouseBindings[buttonId].keyUp = EVENT_ENUM::NULL_EVENT;
		break;

	}
}


void ModuleInput::RemoveMouseBinding(int buttonId)
{
	mouseBindings[buttonId].keyDown = EVENT_ENUM::NULL_EVENT;
	mouseBindings[buttonId].keyIdle = EVENT_ENUM::NULL_EVENT;
	mouseBindings[buttonId].keyRepeat = EVENT_ENUM::NULL_EVENT;
	mouseBindings[buttonId].keyUp = EVENT_ENUM::NULL_EVENT;
}


void ModuleInput::keyBindingSendEvent(int key, KEY_STATE keyAction)
{
	EVENT_ENUM nullEvent = EVENT_ENUM::NULL_EVENT;


	switch (keyAction)
	{
	case KEY_STATE::KEY_IDLE:
		if (keybindings[key].keyIdle != nullEvent)
			app->eventManager->GenerateEvent(keybindings[key].keyIdle, nullEvent);

		break;

	case KEY_STATE::KEY_DOWN:
		if (keybindings[key].keyDown != nullEvent)
			app->eventManager->GenerateEvent(keybindings[key].keyDown, nullEvent);

		break;

	case KEY_STATE::KEY_REPEAT:
		if (keybindings[key].keyRepeat != nullEvent)
		app->eventManager->GenerateEvent(keybindings[key].keyRepeat, nullEvent);

		break;

	case KEY_STATE::KEY_UP:
		if (keybindings[key].keyUp != nullEvent)
		app->eventManager->GenerateEvent(keybindings[key].keyUp, nullEvent);

		break;

	}


}


void ModuleInput::mouseBindingSendEvent(int button, KEY_STATE keyAction)
{


	EVENT_ENUM nullEvent = EVENT_ENUM::NULL_EVENT;

	switch (keyAction)
	{
	case KEY_STATE::KEY_IDLE:
		if (mouseBindings[button].keyIdle != nullEvent)
			app->eventManager->GenerateEvent(mouseBindings[button].keyIdle, nullEvent);
		break;

	case KEY_STATE::KEY_DOWN:
		if (mouseBindings[button].keyDown != nullEvent)
			app->eventManager->GenerateEvent(mouseBindings[button].keyDown, nullEvent);
		break;

	case KEY_STATE::KEY_REPEAT:
		if (mouseBindings[button].keyRepeat != nullEvent)
			app->eventManager->GenerateEvent(mouseBindings[button].keyRepeat, nullEvent);
		break;

	case KEY_STATE::KEY_UP:
		if (mouseBindings[button].keyUp != nullEvent)
			app->eventManager->GenerateEvent(mouseBindings[button].keyUp, nullEvent);
		break;
	}


}


void ModuleInput::HandleDebugKeys()
{

	if (GetKey(SDL_SCANCODE_F6) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::KILL_ALL_ENEMIES, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_F11) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::DESACTIVATE_GODMODE_HEROES, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_F10) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::ACTIVATE_GODMODE_HEROES, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_F8) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::DEBUG_DAY, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_F9) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::DEBUG_NIGHT, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_0) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::GIVE_RESOURCES, EVENT_ENUM::NULL_EVENT);
		app->eventManager->GenerateEvent(EVENT_ENUM::GIVE_RESOURCES_SKILL, EVENT_ENUM::NULL_EVENT);
		app->eventManager->GenerateEvent(EVENT_ENUM::GIVE_RESOURCES_BOOST, EVENT_ENUM::NULL_EVENT);
	}


	if (GetKey(SDL_SCANCODE_U) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::SPAWN_ENEMY_GIGA, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_I) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::SPAWN_ENEMY_NIGHT, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_O) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::SPAWN_ENEMY_RANGED, EVENT_ENUM::NULL_EVENT);
	}


	if (GetKey(SDL_SCANCODE_KP_1) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::SPAWN_BASE, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_KP_2) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::SPAWN_BUILDING, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_KP_3) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::SPAWN_ENEMY, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_KP_4) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::SPAWN_GATHERER_HERO, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_KP_5) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::SPAWN_MELEE_HERO, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_KP_6) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::SPAWN_RANGED_HERO, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_KP_7) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::SPAWN_ROBO_HERO, EVENT_ENUM::NULL_EVENT);
	}

	if (GetKey(SDL_SCANCODE_KP_8) == KEY_STATE::KEY_DOWN)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::SPAWN_TURRET, EVENT_ENUM::NULL_EVENT);
	}
}