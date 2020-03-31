#include "UI_Image.h"

UI_Image::UI_Image(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect* rect, P2SString uiName, bool draggable) : UI(positionValue, father, uiType, rect, uiName, draggable)
{}

UI_Image::~UI_Image()
{}

bool UI_Image::Start()
{
	return true;
}

bool UI_Image::PreUpdate(float dt)
{
	return true;
}

bool UI_Image::Update(float dt)
{
	return true;
}

bool UI_Image::PostUpdate(float dt, SDL_Texture* texture)
{
	return true;
}

void UI_Image::HandleInput()
{}