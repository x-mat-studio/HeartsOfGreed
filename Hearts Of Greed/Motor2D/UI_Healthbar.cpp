#include "UI_Healthbar.h"

UI_Healthbar::UI_Healthbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, Entity* entity, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable),
	originalWidth(rect.w),
	currentHealth(nullptr)
{
	if (entity != nullptr)
	{
		maxHealth = &entity->hitPointsMax;
		currentHealth = &entity->hitPointsCurrent;
		previousHealth = *currentHealth;
	}
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
	previousHealth = *currentHealth;
	
	Draw(texture);

	return true;
}

void UI_Healthbar::HandleInput()
{

}

void UI_Healthbar::AdjustHealth()
{
	if (currentHealth != nullptr) {

		if (*currentHealth > 0 && *currentHealth != previousHealth && *maxHealth > 0)
		{
			box.w = originalWidth * (*currentHealth) / (*maxHealth);
		}
	}
}
