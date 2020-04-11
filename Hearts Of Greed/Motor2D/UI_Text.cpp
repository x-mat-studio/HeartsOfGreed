#include "UI_Text.h"
#include "Render.h"

UI_Text::UI_Text(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable, char* text, SDL_Color color) : UI(positionValue, father, uiType, rect, uiName, draggable)
{

	if (this->name == "saveText" || this->name == "loadText")
		color = {100, 100, 100};

	texture = app->fonts->Print(text, color);
	
}

UI_Text::~UI_Text()
{}


bool UI_Text::PostUpdate(float dt)
{
	// We only use directly Blit in the case of Text. We need an NULL SDL_Rect.
	app->render->Blit(texture, worldPosition.x, worldPosition.y, nullptr, false, false, 0.0f, 255, 255, 255);

	return true;
}

void UI_Text::HandleInput()
{}