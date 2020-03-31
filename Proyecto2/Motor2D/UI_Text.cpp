#include "UI_Text.h"

UI_Text::UI_Text(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect* rect, P2SString uiName, bool draggable) : UI(positionValue, father, uiType, rect, uiName, draggable)
{}

UI_Text::~UI_Text()
{}

bool UI_Text::Start()
{
	return true;
}

bool UI_Text::PreUpdate(float dt)
{
	return true;
}

bool UI_Text::Update(float dt)
{
	return true;
}

bool UI_Text::PostUpdate(float dt, SDL_Texture* texture)
{
	return true;
}

void UI_Text::HandleInput()
{}