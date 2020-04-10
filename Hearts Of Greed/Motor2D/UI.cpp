#include "UI.h"
#include "Render.h"

UI::UI()
{}

UI::UI(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable) :

	worldPosition(positionValue),
	parent(father),
	type(uiType),
	name(uiName),
	localPosition(0, 0),
	box(rect),
	toDelete(false),
	debugBox(false),
	focused(false),
	draggable(draggable),
	dragging(false),
	interactable(true),
	hover(false),
	enabled(true),
	texture (app->uiManager->GetAtlasTexture())

{}

UI::~UI()
{}

bool UI::operator==(UI* element)
{
	return true;
}

bool UI::Start()
{
	return true;
}

bool UI::PreUpdate(float dt)
{



	return true;
}

bool UI::Update(float dt)
{
	return true;
}

bool UI::PostUpdate(float dt)
{
	return true;
}

void UI::CleanUp()
{}

void UI::Draw(SDL_Texture* texture)
{

	if (hover && interactable)
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}
	else if (!hover && interactable)
	{
		SDL_SetTextureColorMod(texture, 200, 200, 200);
		SDL_SetTextureAlphaMod(texture, 200);
	}

	if (!interactable && this->type != UI_TYPE::UI_BUTTON)
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}
	else if (!interactable && this->type == UI_TYPE::UI_BUTTON)
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}


	app->render->Blit(texture, worldPosition.x, worldPosition.y, &box, '\000',255,255,255, false, false);

}

bool UI::MouseUnderElement(int x, int y)
{
	return true;
}

void UI::Drag(int x, int y)
{}

void UI::Move()
{}

void UI::HandleInput()
{}
