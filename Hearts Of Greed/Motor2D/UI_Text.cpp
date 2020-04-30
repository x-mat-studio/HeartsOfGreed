#include "App.h"
#include "UI_Text.h"
#include "Fonts.h"

UI_Text::UI_Text(float x, float y, UI* parent, char* text, bool interactable) : 
	
	UI({ x, y }, parent, UI_TYPE::TEXT, { 0, 0, 0, 0 }, interactable, false, nullptr)
{
	texture = app->fonts->Print(text, {255, 255, 255}, app->fonts->fonts[0]);
}

UI_Text::~UI_Text()
{}