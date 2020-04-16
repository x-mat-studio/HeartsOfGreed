#include "UI.h"
#include "Render.h"
#include "Input.h"
#include "Window.h"

UI::UI()
{}

UI::UI(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable) :

	worldPosition(positionValue),
	parent(father),
	type(uiType),
	name(uiName),
	localPosition(0, 0),
	box(rect),
	debugBox(false),
	focused(false),
	draggable(draggable),
	dragging(false),
	interactable(true),
	hover(false),
	enabled(true),
	hiding_unhiding(false),
	hidden(false),
	defaultPosition(positionValue.x),
	hideSpeed(150.0f),
	texture(app->uiManager->GetAtlasTexture())

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

	if (hover && interactable && this->type == UI_TYPE::UI_BUTTON)
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}
	else if (!hover && interactable && this->type == UI_TYPE::UI_BUTTON)
	{
		SDL_SetTextureColorMod(texture, 200, 200, 200);
		SDL_SetTextureAlphaMod(texture, 225);
	}
	else if (!interactable && this->type == UI_TYPE::UI_BUTTON)
	{
		SDL_SetTextureColorMod(texture, 100, 100, 100);
		SDL_SetTextureAlphaMod(texture, 255);
	}

	if (this->type != UI_TYPE::UI_BUTTON)
	{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(texture, 255);
	}


	app->render->Blit(texture, worldPosition.x, worldPosition.y, &box, false, false, '\000', 255, 255, 255);

}

bool UI::MouseUnderElement(int x, int y)
{
	return true;
}

void UI::Drag(int x, int y)
{}

bool UI::OnAbove()
{
	bool ret = false;

	SDL_Point mouse;
	iMPoint mouseAux = app->input->GetMousePosScreen();
	mouse.x = mouseAux.x;
	mouse.y = mouseAux.y;

	mouse.x = (mouse.x) / app->win->GetUIScale();
	mouse.y = (mouse.y) / app->win->GetUIScale();

	SDL_Rect intersect = { worldPosition.x , worldPosition.y, box.w, box.h };

	if (SDL_PointInRect(&mouse, &intersect) && this->enabled && this->interactable)
		ret = true;

	return ret;
}

void UI::Hide(float dt)
{

	float position = worldPosition.x * app->win->GetUIScale();
	bool right;

	if (position > app->win->width / 2)
		right = true;
	else
		right = false;

	if (right)
	{
		if (hiding_unhiding && !hidden)
		{
			this->worldPosition.x += hideSpeed * dt;

			if (((position)+(box.w / 10)) > app->win->width)
			{
				hidden = true;
				hiding_unhiding = false;
				app->uiManager->StopAll(this, false, true, false);
			}
		}

		else if (hiding_unhiding && hidden)
		{
			this->worldPosition.x -= hideSpeed * dt;

			if (worldPosition.x <= defaultPosition)
			{
				worldPosition.x = defaultPosition;
				hidden = false;
				hiding_unhiding = false;
				app->uiManager->StopAll(this, true, false, false);
			}
		}
	}

	else
	{
		if (hiding_unhiding && !hidden)
		{
			this->worldPosition.x -= hideSpeed * dt;

			if (((position / 2) + (9 * box.w / 10)) < 0)
			{
				hidden = true;
				hiding_unhiding = false;
				app->uiManager->StopAll(this, false, true, false);
			}
		}

		else if (hiding_unhiding && hidden)
		{
			this->worldPosition.x += hideSpeed * dt;

			if (worldPosition.x >= defaultPosition)
			{
				worldPosition.x = defaultPosition;
				hidden = false;
				hiding_unhiding = false;
				app->uiManager->StopAll(this, true, false, false);
			}
		}
	}

}

void UI::Move()
{}

void UI::HandleInput()
{}
