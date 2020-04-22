#include "UI_Text.h"
#include "Render.h"
#include "Textures.h"
#include "Fonts.h"

UI_Text::UI_Text(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable, char* text, SDL_Color color, TTF_Font* font) : UI(positionValue, father, uiType, rect, uiName, draggable)
{
	
	if (this->name == P2SString("saveText") || this->name == P2SString("loadText"))
		color = { 100, 100, 100 };

	if (text != nullptr)
		texture = app->fonts->Print(text, color, font);

}

UI_Text::~UI_Text()
{}


bool UI_Text::Update(float dt)
{
	if (hiding_unhiding)
	{
		Hide(dt);
	}

	if (parent != nullptr)
	{
		enabled = parent->enabled;
	}

	return true;
}

bool UI_Text::PostUpdate(float dt)
{
	// We only use directly Blit in the case of Text. We need an NULL SDL_Rect.
	if (enabled && texture != nullptr)
		app->render->Blit(texture, worldPosition.x, worldPosition.y, nullptr, false, false, 0.0f, 255, 255, 255);

	return true;
}

void UI_Text::HandleInput()
{}

void UI_Text::LoadNewTexture(char* newtext, TTF_Font* newFont)
{
	if (texture != nullptr)
		app->tex->UnLoad(texture);

	SDL_Color color{ 255,255,255 };

	if (this->name == P2SString("saveText") || this->name == P2SString("loadText"))
		color = { 100, 100, 100 };

	texture = app->fonts->Print(newtext, color, newFont);
}
