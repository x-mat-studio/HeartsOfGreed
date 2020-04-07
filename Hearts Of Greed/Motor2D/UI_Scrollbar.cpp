#include "UI_Scrollbar.h"


UI_Scrollbar::UI_Scrollbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable)
{
	interactable = true;
}

UI_Scrollbar::~UI_Scrollbar()
{}

bool UI_Scrollbar::Start()
{

	return true;
}

bool UI_Scrollbar::PreUpdate(float dt)
{
	
	return true;
}

bool UI_Scrollbar::Update(float dt)
{

	return true;
}

bool UI_Scrollbar::PostUpdate(float dt)
{


	return true;
}


void UI_Scrollbar::HandleInput()
{

}