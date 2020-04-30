#include "UI_Scrollbar.h"

UI_Scrollbar::UI_Scrollbar(float x, float y, UI* parent, SDL_Rect rect, SDL_Texture* texture, int maxValue) :

	UI({ x, y }, parent, UI_TYPE::SCROLLBAR, rect, true, true, texture), maxValue(maxValue)
{
	if (parent == nullptr)
		assert("Must have a father");
}

UI_Scrollbar::~UI_Scrollbar()
{}

void UI_Scrollbar::Move() {

	if (localPosition.x > father->rect.w - rect.w / 2)
		localPosition.x = father->rect.w - rect.w / 2;

	else if (localPosition.x < 0)
		localPosition.x = 0;

	position.x = localPosition.x + father->GetPosition().x;
	position.y = father->GetPosition().y - 15;
}

