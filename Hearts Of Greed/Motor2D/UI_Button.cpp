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
	includeFather(includeFather),
	defaultPosition(positionValue.x)
{

	if (this->name == "saveButton" || this->name == "loadButton")
		interactable = false;

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
		if (interactable)
		{
			if (hover)
			{
				HoverFeedback();

				if (app->input->GetMouseButtonDown(1) == KEY_STATE::KEY_DOWN)
					OnClick();

				
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

	this->worldPosition.x = defaultPosition;

	return true;
}

bool UI_Button::OnAbove()
{
	bool ret = false;

	SDL_Point mouse;
	app->input->GetMousePositionRaw(mouse.x, mouse.y);

	mouse.x = (mouse.x) / app->win->GetUIScale();
	mouse.y = (mouse.y) / app->win->GetUIScale();

	SDL_Rect intersect = { worldPosition.x , worldPosition.y, box.w, box.h };

	if (SDL_PointInRect(&mouse, &intersect) && this->enabled && this->interactable)
		ret = true;

	return ret;
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