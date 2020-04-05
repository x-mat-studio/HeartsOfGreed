#include "UI_Text.h"

UI_Text::UI_Text(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool draggable, char* text) : UI(positionValue, father, uiType, rect, uiName, draggable)
{
	texture = app->fonts->Print(text);
}

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

bool UI_Text::PostUpdate(float dt)
{

	Draw(texture);

	return true;
}

void UI_Text::HandleInput()
{}