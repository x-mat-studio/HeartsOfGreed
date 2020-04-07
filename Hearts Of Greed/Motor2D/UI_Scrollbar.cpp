#include "UI_Scrollbar.h"


UI_Scrollbar::UI_Scrollbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable)
{
	SDL_Rect aux = app->uiManager->RectConstructor(449, 24, 24, 24);
	scrollButton = app->uiManager->AddUIElement(fMPoint(0 , this->worldPosition.y), this, UI_TYPE::UI_BUTTON, aux, (P2SString)"scrollButton", DRAGGABLE::DRAG_X);
	scrollButton->worldPosition.y = this->worldPosition.y - scrollButton->box.h / 2 + this->box.h / 2;
	interactable = true;
}

UI_Scrollbar::~UI_Scrollbar()
{}

bool UI_Scrollbar::Start()
{

	return true;
}

bool UI_Scrollbar::PreUpdate(float dt)
{

	return true;
}

bool UI_Scrollbar::Update(float dt)
{

	return true;
}

bool UI_Scrollbar::PostUpdate(float dt)
{

	Draw(texture);

	return true;
}


void UI_Scrollbar::ScrollLimit() 
{

	// We don't need to put Y axis limits, because we're not using it.

	if (scrollButton->localPosition.x > 0)
	{
		scrollButton->localPosition.x = 0;

		scrollButton->worldPosition.x = this->worldPosition.x - scrollButton->localPosition.x;
	}
	else if (scrollButton->localPosition.x < (-this->box.w + scrollButton->box.w))
	{
		scrollButton->localPosition.x = -this->box.w + scrollButton->box.w;

		scrollButton->worldPosition.x = this->worldPosition.x - scrollButton->localPosition.x;
	}

}

void UI_Scrollbar::HandleInput()
{

}