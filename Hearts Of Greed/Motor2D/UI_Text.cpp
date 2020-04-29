#include "App.h"
#include "UI_Text.h"
#include "Fonts.h"

UI_Text::UI_Text(float x, float y, UI* parent, char* text, bool interactable) : 
	
	UI({ x, y }, parent, UI_TYPE::TEXT, { 0, 0, 0, 0 }, interactable, false, nullptr)
{
	texture = app->fonts->Print(text, {100, 100, 100}, app->fonts->fonts[0]);
}

UI_Text::~UI_Text()
{}