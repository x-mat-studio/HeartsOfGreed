#include "UI_Healthbar.h"


UI_Healthbar::UI_Healthbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable),
	maxHealth(nullptr),		// TODO initialize and make them pretty (I'm already recieving the entity with the portraits)
	currentHealth(nullptr),
	originalWidth(rect.w),
	previousHealth(NULL)

{}

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
