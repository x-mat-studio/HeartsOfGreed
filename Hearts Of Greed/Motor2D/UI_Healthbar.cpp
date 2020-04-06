#include "UI_Healthbar.h"

UI_Healthbar::UI_Healthbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable)
{}

UI_Healthbar::~UI_Healthbar()
{}

bool UI_Healthbar::Start()
{
	return true;
}

bool UI_Healthbar::PreUpdate(float dt)
{
	return true;
}

bool UI_Healthbar::Update(float dt)
{
	return true;
}

void UI_Healthbar::HandleInput()
{

}