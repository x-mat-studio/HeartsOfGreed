#include "App.h"
#include "UI_Scrollbar.h"
#include "Audio.h"

UI_Scrollbar::UI_Scrollbar(float x, float y, UI* parent, SDL_Rect rect, SDL_Texture* texture, float maxValue) :

	UI({ x, y }, parent, UI_TYPE::SCROLLBAR, rect, true, true, texture), maxValue(maxValue), previousX(x)
{
	if (parent == nullptr)
		assert("Must have a father");

	if (maxValue == 128.0f)
	{
		localPosition.x = ValueToPosition(app->audio->musicVolume);
	}
	else
	{
		localPosition.x = ValueToPosition(app->audio->volumeAdjustment + 255);
	}
}

UI_Scrollbar::~UI_Scrollbar()
{}


void  UI_Scrollbar::HandleInput()
{
	if (previousX != localPosition.x)
	{
		currentValue = PositionToValue();
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

float UI_Scrollbar::PositionToValue()
{
	return (localPosition.x + (rect.w * 0.5)) * maxValue / father->rect.w;
}


float UI_Scrollbar::ValueToPosition(float value)
{
	return (value * father->rect.w / maxValue);
}