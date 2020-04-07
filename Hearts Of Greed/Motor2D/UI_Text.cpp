#include "UI_Text.h"
#include "Render.h"

UI_Text::UI_Text(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable, char* text, SDL_Color color) : UI(positionValue, father, uiType, rect, uiName, draggable)
{
	texture = app->fonts->Print(text, color);
}

UI_Text::~UI_Text()
{}


bool UI_Text::PostUpdate(float dt)
{
	// We only use directly Blit in the case of Text. We need an NULL SDL_Rect.
	app->render->Blit(texture, worldPosition.x, worldPosition.y, nullptr, 0.0f, false, false);

	return true;
}

void UI_Text::HandleInput()
{}