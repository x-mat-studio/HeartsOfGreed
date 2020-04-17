#include "UI_Healthbar.h"
#include "Player.h"

UI_Healthbar::UI_Healthbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, Entity* entity, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable),
	originalWidth(rect.w),
	currentHealth(nullptr),
	entity(entity),
	entityBuffer(entity)
{
	if (entity != nullptr)
	{
		maxHealth = &entity->hitPointsMax;
		currentHealth = &entity->hitPointsCurrent;
		previousHealth = *currentHealth;
	}
	else
	{
		maxHealth = nullptr;
		currentHealth = nullptr;
		previousHealth = 0;
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
	if (parent->name == "portraitBG")
	{
		entityBuffer = app->player->focusedEntity;
	}

	if (entity == entityBuffer)
	{
		previousHealth = *currentHealth;

		Draw(texture);
	}
	else
	{
		if (parent->name == "portraitBG")
		{
			app->uiManager->DeleteUI(parent, false); // TODO It would probably be cleaner to send a signal of this element and a variable to delete it at the postupdate
		}
	}

	return true;
}

void UI_Healthbar::HandleInput()
{}

void UI_Healthbar::AdjustHealth()
{
	if (currentHealth != nullptr) 
	{
		if (*currentHealth > 0 && *currentHealth != previousHealth && *maxHealth > 0)
		{
			box.w = originalWidth * (*currentHealth) / (*maxHealth);
		}
	}
}

void UI_Healthbar::EntityDeath()
{
	this->entity = nullptr;
	this->currentHealth = nullptr;
	this->maxHealth = nullptr;
}