#include "App.h"
#include "UI_Text.h"
#include "Fonts.h"

UI_Text::UI_Text(float x, float y, UI* parent, char* text, bool interactable) : UI({ x, y }, parent, UI_TYPE::TEXT, { 0, 0, 0, 0 }, interactable, false, nullptr)
{
	if (text != nullptr)
	{
		texture = app->fonts->Print(text, {100, 100, 100}, app->fonts->fonts[0]);
	}
}

UI_Text::~UI_Text()
{}

bool UI_Text::PreUpdate(float dt)
{
	return true;
}

bool UI_Text::Update(float dt)
{
	return true;
}

bool UI_Text::PostUpdate(float dt)
{
	Draw(dt);

	return true;
}

void UI_Text::CleanUp()
{}
