#include "UI.h"

UI::UI()
{}

UI::UI(fMPoint positionValue, UI* father, UI_TYPE uiType, P2SString uiName) :

	worldPosition(positionValue),
	parent(father),
	type(uiType),
	name(uiName)
{}

UI::~UI()
{}

bool UI::operator==(UI* element)
{
	return true;
}

bool UI::Start(SDL_Texture* texture)
{
	return true;
}

bool UI::PreUpdate(float dt)
{
	return true;
}

bool UI::Update(float dt)
{
	return true;
}

bool UI::PostUpdate(float dt)
{
	return true;
}

bool UI::Draw()
{
	return true;
}

bool UI::MouseUnderElement(int x, int y)
{
	return true;
}

void UI::Drag(int x, int y)
{}

void UI::Move()
{}

void UI::HandleInput()
{}
