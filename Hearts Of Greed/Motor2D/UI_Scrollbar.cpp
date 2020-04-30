#include "UI_Scrollbar.h"

UI_Scrollbar::UI_Scrollbar(float x, float y, UI* parent, SDL_Rect rect, SDL_Texture* texture) :

	UI({ x, y }, parent, UI_TYPE::SCROLLBAR, rect, true, false, texture)
{}

UI_Scrollbar::~UI_Scrollbar()
{}

