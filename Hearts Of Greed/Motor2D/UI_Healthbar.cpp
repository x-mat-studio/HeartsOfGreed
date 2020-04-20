#include "UI_Healthbar.h"
#include "Player.h"
#include "Hero.h"

UI_Healthbar::UI_Healthbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, Entity* entity, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable),
	originalWidth(rect.w),
	entity(entity)
{
	if (entity != nullptr)
	{
		if (this->name == "Ebar")
		{
			Hero* hero = (Hero*)entity;
			maxValue = &hero->maxEnergyPoints;
			currentValue = &hero->energyPoints;
			previousValue = -1;
		}
		else
		{
			maxValue = &entity->hitPointsMax;
			currentValue = &entity->hitPointsCurrent;
			previousValue = -1;
		}
	}
	else
	{
		maxValue = nullptr;
		currentValue = nullptr;
		previousValue = 0;
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
	if (hiding_unhiding)
	{
		Hide(dt);
	}

	return true;
}

bool UI_Healthbar::PostUpdate(float dt)
{
	if (entity != nullptr)
	{
		AdjustValue();
		previousValue = *currentValue;
		Draw(texture);
	}

	return true;
}

void UI_Healthbar::HandleInput()
{}

void UI_Healthbar::AdjustValue()
{
	if (currentValue != nullptr) 
	{
		if (*currentValue > 0 && *currentValue != previousValue && *maxValue > 0)
		{
			box.w = originalWidth * (*currentValue) / (*maxValue);
		}
	}
}

void UI_Healthbar::EntityDeath()
{
	this->entity = nullptr;
	this->currentValue = nullptr;
	this->maxValue = nullptr;
}