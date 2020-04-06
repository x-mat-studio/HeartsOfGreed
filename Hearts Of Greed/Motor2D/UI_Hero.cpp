#include "UI_Hero.h"

UI_Hero::UI_Hero(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool draggable) : UI(positionValue, father, uiType, rect, uiName, draggable)
{}

UI_Hero::~UI_Hero()
{}

bool UI_Hero::Start()
{
	return true;
}

bool UI_Hero::PreUpdate(float dt)
{
	return true;
}

bool UI_Hero::Update(float dt)
{
	return true;
}

bool UI_Hero::PostUpdate(float dt)
{
	Draw(texture);

	return true;
}

void UI_Hero::HandleInput()
{

}


