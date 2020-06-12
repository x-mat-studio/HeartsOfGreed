#include "App.h"
#include "UI_Text.h"
#include "Fonts.h"
#include "Textures.h"

UI_Text::UI_Text(float x, float y, UI* parent, char* text, bool interactable, Uint32 lenght) : 
	
	UI({ x, y }, parent, UI_TYPE::TEXT, { 0, 0, 0, 0 }, interactable, false, nullptr)
{
	texture = app->fonts->Print(text, {255, 255, 255}, app->fonts->fonts[0], lenght);
}

UI_Text::~UI_Text()
{
	app->tex->UnLoad(texture);
}