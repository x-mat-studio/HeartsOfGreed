#include "UI_Healthbar.h"

UI_Healthbar::UI_Healthbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, Entity* entity, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable),
	originalWidth(rect.w),
	previousHealth(NULL)
{
	maxHealth = nullptr;
	currentHealth = nullptr;
}

UI_Healthbar::~UI_Healthbar()
{}

bool UI_Healthbar::Start()
{
	return true;
}

bool UI_Healthbar::PreUpdate(float dt)
{
	return true;
}

bool UI_Healthbar::Update(float dt)
{
	AdjustHealth();
	
	if (hiding_unhiding)
	{
		Hide(dt);
	}

	return true;
}

bool UI_Healthbar::PostUpdate(float dt)
{
	Draw(texture);

	return true;
}

void UI_Healthbar::HandleInput()
{

}

void UI_Healthbar::AdjustHealth()
{
	if (currentHealth!= nullptr && *currentHealth != previousHealth)
	{
		box.w = originalWidth * (*currentHealth) / (*maxHealth);
	}
}
