#include "UI_Button.h"
#include "EventManager.h"
#include "Window.h"
#include "Render.h"
#include "Audio.h"

UI_Button::UI_Button(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool menuClosure, bool includeFather, DRAGGABLE draggable, EVENT_ENUM eventR, EVENT_ENUM eventTrigger) : UI(positionValue, father, uiType, rect, uiName, draggable),
	accuratedDrag({0, 0}),
	eventRecieved(eventR),
	eventTriggerer(eventTrigger),
	closeMenu(menuClosure),
	includeFather(includeFather)
{

	if (this->name == "saveButton" || this->name == "loadButton")
		interactable = false;

	defaultPosition = positionValue.x;

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

	return true;
}

bool UI_Button::Update(float dt)
{

	if (enabled) 
	{

		if (hiding_unhiding)
			Hide(dt);
			

		if (interactable)
		{
			if (hover)
			{
				HoverFeedback();

				if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_UP) 
				{
					OnClick();

					// Only for Debug and Testing
					if (this->name == "testButton")
					{
						hiding_unhiding = true;
					}
						
				}
					
	
					if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_REPEAT)
					{


						if (draggable > DRAGGABLE::DRAG_OFF) 
						{
							if (draggable > DRAGGABLE::DRAG_OFF)
								dragging = true;

							iMPoint mouseClick = { 0,0 };
							app->input->GetMousePosition(mouseClick.x, mouseClick.y);
							accuratedDrag = { mouseClick.x - (this->worldPosition.x), mouseClick.y - (this->worldPosition.y) };
						}
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

	return true;
}

bool UI_Button::PostUpdate(float dt)
{
	if(enabled)
		Draw(texture);

	if (!hiding_unhiding && !hidden)
		this->worldPosition.x = defaultPosition;

	return true;
}


void UI_Button::OnClick()
{
	app->eventManager->GenerateEvent(eventRecieved, eventTriggerer);
	if (closeMenu == true)
	{
		CloseMenu();
	}

	app->audio->PlayFx(app->uiManager->clickSound);
}

void UI_Button::HoverFeedback()
{
	if (hoverSound)
		app->audio->PlayFx(app->uiManager->hoverSound);

	hoverSound = false;

	if(!hiding_unhiding && !hidden)
		this->worldPosition.x -= 8;
}

void UI_Button::CloseMenu()
{
	app->uiManager->DeleteUI(parent, includeFather);
}

void UI_Button::MovingIt(float dt)
{

	iMPoint MousePos = { 0,0 };
	app->input->GetMousePosition(MousePos.x, MousePos.y);

	fMPoint currentPos = this->worldPosition;

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