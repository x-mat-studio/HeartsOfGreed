#include "UI_Image.h"

UI_Image::UI_Image(float x, float y, UI* parent, SDL_Rect rect, SDL_Texture* texture, bool dragable) : UI({ x, y }, parent, UI_TYPE::IMG, rect, true, dragable, texture)
{}

UI_Image::~UI_Image()
{}

bool UI_Image::PreUpdate(float dt)
{
	return true;
}

bool UI_Image::Update(float dt)
{
	return true;
}

bool UI_Image::PostUpdate(float dt)
{
	Draw(dt);

	return true;
}

void UI_Image::CleanUp()
{}

