#include "UI_Image.h"

UI_Image::UI_Image(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable)
{}

UI_Image::~UI_Image()
{
	texture = nullptr;
}

bool UI_Image::Update(float dt)
{
	if (hiding_unhiding)
	{
		Hide(dt);
	}

	if (parent != nullptr)
	{
		enabled = parent->enabled;
	}

	return true;
}

bool UI_Image::PostUpdate(float dt)
{
	if (enabled && texture != nullptr)
		Draw(texture);

	return true;
}

void UI_Image::HandleInput()
{}