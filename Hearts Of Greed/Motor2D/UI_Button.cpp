#include "UI_Button.h"
#include "EventManager.h"
#include "Window.h"
#include "Render.h"
#include "Audio.h"

UI_Button::UI_Button(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, EVENT_ENUM eventR, bool menuClosure, bool includeFather,
	bool hiding, bool hoverMove, DRAGGABLE draggable, EVENT_ENUM eventTrigger,  bool interactable) : UI(positionValue, father, uiType, rect, uiName, draggable),
	
	accuratedDrag({0, 0}),
	eventRecieved(eventR),
	eventTriggerer(eventTrigger),
	hoverSound(-1)

{

	if (this->name == P2SString("saveButton") || this->name == P2SString("loadButton"))
		interactable = false;

	defaultPosition = positionValue.x;

	properties.hiding = hiding;
	properties.hoverMove = hoverMove;
	properties.closeMenu = menuClosure;
	properties.includeFather = includeFather;
	properties.draggable = draggable;
	properties.scrollbarPositioning = false;

	this->interactable = interactable;

}

UI_Button::~UI_Button()
{}

bool UI_Button::Start()
{
	return true;
}

bool UI_Button::PreUpdate(float dt)
{
	hover = OnAbove();

	if (parent != nullptr)
	{
		enabled = parent->enabled;
	}

	return true;
}

bool UI_Button::Update(float dt)
{
	if (enabled) 
	{
		if (hiding_unhiding)
		{
			Hide(dt);
		}

		if (interactable)
		{
			if (hover)
			{
				HoverFeedback();

				if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_UP) 
				{
					OnClick(dt);						
				}
					
	
				if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_REPEAT)
				{
					
					if (draggable > DRAGGABLE::DRAG_OFF) 
					{
						if (draggable > DRAGGABLE::DRAG_OFF)
							dragging = true;
				
						iMPoint mouseClick = app->input->GetMousePosScreen();
						mouseClick.x *= 0.5f;
						mouseClick.y *= 0.5f;
						accuratedDrag = { mouseClick.x - (this->worldPosition.x), mouseClick.y - (this->worldPosition.y) };
					}
				}

				if (name == P2SString("scrollButton") && app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_DOWN)
				{
					properties.scrollbarPositioning = true;
				}

			}
			else
				hoverSound = true;


			if (dragging) 
			{
				if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_IDLE || app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_UP)
					dragging = false;
				else
					MovingIt(dt);
			}
		}
	}

	if (properties.hiding == true)
	{
		if (hidden == false && hiding_unhiding == false)
		{
			if (worldPosition.x * app->win->GetUIScale() > app->win->width * 0.5f)
			{
				box.x = 556;
			}
			else
			{
				box.x = 540;
			}
		}
		else if (hidden == true && hiding_unhiding == false)
		{
			if (worldPosition.x * app->win->GetUIScale() > app->win->width * 0.5f)
			{
				box.x = 540;
			}
			else
			{
				box.x = 556;
			}
		}
	}
	
	if (properties.scrollbarPositioning == true && app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_UP)
	{
		app->eventManager->GenerateEvent(eventRecieved, eventTriggerer);
		properties.scrollbarPositioning = false;
	}

	return true;
}

bool UI_Button::PostUpdate(float dt)
{
	if(enabled)
		Draw(texture);

	if (properties.hoverMove && !hiding_unhiding && !hidden)
		this->worldPosition.x = defaultPosition;

	return true;
}


void UI_Button::OnClick(float dt)
{
	app->eventManager->GenerateEvent(eventRecieved, eventTriggerer);
	if (properties.closeMenu == true)
	{
		CloseMenu();
	}

	if (properties.hiding == true)
	{
		hiding_unhiding = true;
		app->uiManager->HideElements(this, dt);
	}

	app->audio->PlayFx(app->uiManager->clickSound,0,-1);
}

void UI_Button::HoverFeedback()
{
	if (hoverSound)
		app->audio->PlayFx(app->uiManager->hoverSound,0,-1);

	hoverSound = false;

	if(properties.hoverMove && !hiding_unhiding && !hidden)
		this->worldPosition.x -= 8;
}

void UI_Button::CloseMenu()
{
	app->uiManager->DeleteUIChilds(parent, properties.includeFather);
}

void UI_Button::MovingIt(float dt)
{
	iMPoint MousePos = app->input->GetMousePosScreen();
	MousePos.x *= 0.5f;
	MousePos.y *= 0.5f;

	if (draggable == DRAGGABLE::DRAG_X)
		this->worldPosition.x += ((MousePos.x - this->worldPosition.x) - accuratedDrag.x);	
	else if(draggable == DRAGGABLE::DRAG_Y)
		this->worldPosition.y += ((MousePos.y - this->worldPosition.y) - accuratedDrag.y);
	else if (draggable == DRAGGABLE::DRAG_XY) 
	{
		this->worldPosition.x += ((MousePos.x - this->worldPosition.x) - accuratedDrag.x);
		this->worldPosition.y += ((MousePos.y - this->worldPosition.y) - accuratedDrag.y);
	}

}

void UI_Button::HandleInput()
{
	
}