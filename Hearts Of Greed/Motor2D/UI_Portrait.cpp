#include "UI_Portrait.h"

UI_Portrait::UI_Portrait(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool draggable) : UI(positionValue, father, uiType, rect, uiName, draggable)
{}

UI_Portrait::~UI_Portrait()
{}

bool UI_Portrait::Start()
{
	return true;
}

bool UI_Portrait::PreUpdate(float dt)
{
	return true;
}

bool UI_Portrait::Update(float dt)
{
	return true;
}

bool UI_Portrait::PostUpdate(float dt)
{
	Draw(texture);

	return true;
}

void UI_Portrait::HandleInput()
{

}


