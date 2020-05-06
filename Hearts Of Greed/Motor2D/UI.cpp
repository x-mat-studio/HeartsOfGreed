#include "UI.h"
#include "Render.h"
#include "Input.h"
#include "Window.h"
#include "Textures.h"
#include "UIManager.h"

UI::UI() :

	localPosition(NULL, NULL),
	position(NULL, NULL),

	father(nullptr),
	texture(nullptr),

	type(UI_TYPE::NONE),

	rect(),

	interactable(false),
	dragable(false),
	focused(false)
{}


UI::UI(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, bool interactable, bool dragable, SDL_Texture* texture) :

	localPosition(positionValue),
	position(NULL, NULL),

	father(father),
	texture(texture),

	type(uiType),

	rect(rect),

	interactable(interactable),
	dragable(dragable),
	focused(false)

{
	if (father != nullptr)
	{
		position = father->GetPosition() + localPosition;
	}

	else
	{
		position = localPosition;
	}

}


UI::~UI()
{
	app->uiManager->CheckDragElement(this);
	texture = nullptr;
	father = nullptr;
}


bool UI::PreUpdate(float dt)
{
	HandleInput();
	return true;
}

bool UI::Update(float dt)
{
	Move();
	return true;
}

bool UI::PostUpdate(float dt)
{
	Draw(dt);
	return true;
}


void UI::HandleInput()
{
}


void UI::Draw(float dt)
{
	if (texture != nullptr)
	{
		if (rect.h == 0 || rect.w == 0)
		{
			app->render->Blit(texture, position.x, position.y, nullptr, false, false, '\000', 255, 255, 255);
		}

		else
		{
			app->render->Blit(texture, position.x, position.y, &rect, false, false, '\000', 255, 255, 255);
		}
	}
}


void UI::Drag(int x, int y)
{
	localPosition.x += x;
	localPosition.y += y;
}

bool UI::OnAbove()
{
	focused = false;

	if (interactable == false)
	{
		return false;
	}

	SDL_Point mouse;
	iMPoint mouseAux = app->input->GetMousePosScreen();
	mouse.x = mouseAux.x;
	mouse.y = mouseAux.y;

	mouse.x = (mouse.x) / app->win->GetUIScale();
	mouse.y = (mouse.y) / app->win->GetUIScale();

	SDL_Rect intersect = { position.x , position.y, rect.w, rect.h };

	if (SDL_PointInRect(&mouse, &intersect) && this->interactable)
	{
		focused = true;
		return true;
	}

	return false;
}


void UI::Move()
{
	if (father != nullptr)
	{
		position = father->GetPosition() + localPosition;
	}

	else
	{
		position = localPosition;
	}
}


fMPoint UI::GetPosition()
{
	return position;
}


fMPoint UI::GetLocalPosition()
{
	return localPosition;
}


void UI::SetLocalPosition(fMPoint locPos)
{
	localPosition = locPos;
}


bool UI::GetDragable()
{
	return dragable;
}


UI* UI::SearchFocus()
{
	if (OnAbove() == true)
	{
		return this;
	}

	return nullptr;
}


void UI::UnFocus()
{
	focused = false;
}