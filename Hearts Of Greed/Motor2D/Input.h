#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"


#define NUM_MOUSE_BUTTONS 5
#define TEXT_INPUT_MAX_LENGHT 100

struct SDL_Rect;


enum class EVENT_WINDOW //used to control mouse events
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum class KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};


struct EventsOnKeyPress
{
	EVENT_ENUM keyIdle		=	EVENT_ENUM::NULL_EVENT;
	EVENT_ENUM keyDown		=	EVENT_ENUM::NULL_EVENT;
	EVENT_ENUM keyRepeat	=	EVENT_ENUM::NULL_EVENT;
	EVENT_ENUM keyUp		=	EVENT_ENUM::NULL_EVENT;
};

class ModuleInput : public Module
{

public:

	ModuleInput();

	// Destructor
	virtual ~ModuleInput();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(EVENT_WINDOW ev);

	// Check key states (includes mouse and joy buttons)
	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int& x, int& y);
	void GetMouseMotion(int& x, int& y);
	//Same as GetMousePosition but without the scaling factor
	void GetMousePositionRaw(int& x, int& y);
	//Get motion of the scrollwheel in both x (left[negative]/right[positive]) and y (up[positive]/down[negative])axis
	void GetScrollWheelMotion(int& x, int& y);


	void ActivateTextInput(SDL_Rect& rect);
	void DesactivateTextInput();

	const char* GetInputText();

	// keybinding managing:

	void AddKeyBinding(int key, KEY_STATE keyAction, EVENT_ENUM event);
	void AddMouseBinding(int buttonId, KEY_STATE buttonAction, EVENT_ENUM event);

	
	void RemoveSingleKeyBinding(int key, KEY_STATE keyAction);
	void RemoveKeyBinding(int key);
	void RemoveSingleMouseBinding(int buttonId, KEY_STATE keyAction);
	void RemoveMouseBinding(int buttonId);

	void keyBindingSendEvent(int key, KEY_STATE keyAction);
	void mouseBindingSendEvent(int button, KEY_STATE keyAction);



private:
	//	bool TextHasSpace();
	void HandleTextInput();
	void HandleDebugKeys();

public:
	bool inputTexActivated;

private:
	bool		windowEvents[(int)EVENT_WINDOW::WE_COUNT];
	KEY_STATE* keyboard;
	KEY_STATE	mouseButtons[NUM_MOUSE_BUTTONS];
	int			mouseMotionX;
	int			mouseMotionY;
	int			mouseX;
	int			mouseY;
	int			mouseXRaw;
	int			mouseYRaw;

	int			mouseWheelMotionX;
	int			mouseWheelMotionY;


	P2SString	text;
	int			cursorPos;

	EventsOnKeyPress* keybindings;
	EventsOnKeyPress mouseBindings[NUM_MOUSE_BUTTONS];


};

#endif // __INPUT_H__