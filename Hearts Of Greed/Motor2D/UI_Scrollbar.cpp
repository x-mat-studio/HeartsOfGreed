#include "UI_Scrollbar.h"


UI_Scrollbar::UI_Scrollbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable, float maxValue) : UI(positionValue, father, uiType, rect, uiName, draggable),
generatedButton(false),
maxValue(maxValue)
{}

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
	
	GenerateScrollButton();

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

bool UI_Scrollbar::GenerateScrollButton() 
{

	if (!generatedButton) 
	{
		SDL_Rect aux = app->uiManager->RectConstructor(158, 16, 11, 32);
		scrollButton = app->uiManager->AddButton(fMPoint(this->worldPosition.x, this->worldPosition.y), this, UI_TYPE::UI_BUTTON, aux, (P2SString)"scrollButton", EVENT_ENUM::NULL_EVENT, false, false, false, false, DRAGGABLE::DRAG_X);
		scrollButton->box.w /= 2;
		scrollButton->box.h /= 2;
		scrollButton->worldPosition.y = this->worldPosition.y - scrollButton->box.h / 2 + this->box.h / 2;
		scrollButton->hidden = true;

		generatedButton = true;
	}

	scrollButton->localPosition.x = this->worldPosition.x - scrollButton->worldPosition.x;
	scrollButton->localPosition.y = this->worldPosition.y - scrollButton->worldPosition.y;
	ScrollLimit();
	
	return generatedButton;
}

float UI_Scrollbar::GetScrollValue()
{ 
	return -((float(-scrollButton->localPosition.x) / (float(-this->box.w) + float(scrollButton->box.w))) * maxValue); 
}