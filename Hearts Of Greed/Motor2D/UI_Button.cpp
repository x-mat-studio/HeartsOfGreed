#include "UI_Button.h"
#include "EventManager.h"

UI_Button::UI_Button(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool menuClosure, DRAGGABLE draggable, EVENT_ENUM eventTrigger) : UI(positionValue, father, uiType, rect, uiName, draggable),
	accuratedDrag({0, 0}),
	eventTriggered(eventTrigger),
	closeMenu(menuClosure)
{}

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
		if (interactable) 
		{
			if (hover) 
			{
				if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_DOWN)
					OnClick();

				if(draggable > DRAGGABLE::DRAG_OFF)
					if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_REPEAT) 
					{
						if (draggable > DRAGGABLE::DRAG_OFF)
							dragging = true;

						iMPoint mouseClick = { 0,0 };
						app->input->GetMousePosition(mouseClick.x, mouseClick.y);								
						accuratedDrag = { mouseClick.x - (this->worldPosition.x), mouseClick.y - (this->worldPosition.y) };
					}
			}


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

	return true;
}

bool UI_Button::OnAbove()
{
	bool ret = false;

	SDL_Point mouse;
	app->input->GetMousePosition(mouse.x, mouse.y);

	SDL_Rect intersect = { worldPosition.x , worldPosition.y, box.w, box.h };

	if (SDL_PointInRect(&mouse, &intersect) && this->enabled && this->interactable) 
		ret = true;

	return ret;
}

void UI_Button::OnClick()
{
	app->eventManager->GenerateEvent(eventTriggered, EVENT_ENUM::NULL_EVENT);
}

void CloseMenu()
{
	// TODO if variable is closeMenu, close all UI that has the same father. Maybe do an invisible menu before opening a menu, to avoid deleting everything on nullptr? Or is the menu supposed to be the background?
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
		

	/* if (parent != nullptr)
	{
		if (dragable)
			this->localPosition.x += currentPos.x - this->worldPosition.x;

		if (dragable)
			this->localPosition.y += currentPos.y - this->worldPosition.y;

		if (dragable)
			this->worldPosition.x = parent->worldPosition.x - localPosition.x;

		if (dragable)
			this->worldPosition.y = parent->worldPosition.y - localPosition.y;
	} */

}

void UI_Button::HandleInput()
{
	
}