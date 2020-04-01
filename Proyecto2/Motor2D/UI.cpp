#include "UI.h"
#include "Render.h"

UI::UI()
{}

UI::UI(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect* rect, P2SString uiName, bool draggable) :

	worldPosition(positionValue),
	parent(father),
	type(uiType),
	name(uiName),
	localPosition(0, 0),
	box(rect),
	toDelete(false),
	debugBox(false),
	focused(false),
	dragable(draggable)
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

bool UI::PostUpdate(float dt, SDL_Texture* texture)
{

	Draw(texture);

	return true;
}

void UI::Draw(SDL_Texture* texture)
{
	app->render->Blit(texture, worldPosition.x, worldPosition.y, box);

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
