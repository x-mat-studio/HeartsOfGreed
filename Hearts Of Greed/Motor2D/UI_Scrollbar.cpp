#include "App.h"
#include "UI_Scrollbar.h"
#include "Audio.h"

UI_Scrollbar::UI_Scrollbar(float x, float y, UI* parent, SDL_Rect rect, SDL_Texture* texture, float maxValue) :

	UI({ x, y }, parent, UI_TYPE::SCROLLBAR, rect, true, true, texture), maxValue(maxValue), previousX(x)
{
	if (parent == nullptr)
		assert("Must have a father");
}

UI_Scrollbar::~UI_Scrollbar()
{}


void  UI_Scrollbar::HandleInput()
{
	if (previousX != localPosition.x)
	{
		currentValue = ConvertPosition();
		if (maxValue == 128.0f)
		{
			app->audio->SetVolume(currentValue);
		}
		else
		{
			app->audio->volumeAdjustment = currentValue - 255;
		}
	}

	previousX = localPosition.x;
}


void UI_Scrollbar::Move() {

	if (localPosition.x > father->rect.w - rect.w / 2)
		localPosition.x = father->rect.w - rect.w / 2;

	else if (localPosition.x < 0)
		localPosition.x = 0;

	position.x = localPosition.x + father->GetPosition().x;
	position.y = father->GetPosition().y - 15;
}

float UI_Scrollbar::ConvertPosition()
{
	return (localPosition.x + (rect.w * 0.5)) * maxValue / father->rect.w;
}

