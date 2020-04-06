#include "UI_Portrait.h"

UI_Portrait::UI_Portrait(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool draggable) : UI(positionValue, father, uiType, rect, uiName, draggable),
	nextVectorPosition(positionValue.y)
{}

UI_Portrait::~UI_Portrait()
{
	//DELETE ALL ELEMENTS IN A COOL WAY
	//THEN, DELETE ALL ELEMENTS IN VECTOR
}

bool UI_Portrait::Start()
{
	return true;
}

bool UI_Portrait::PreUpdate(float dt)
{
	return true;
}

bool UI_Portrait::Update(float dt)
{
	return true;
}

bool UI_Portrait::PostUpdate(float dt)
{
	Draw(texture);

	return true;
}

void UI_Portrait::HandleInput()
{

}

void UI_Portrait::CreatePortrait(Entity* entity)
{
	SDL_Rect rect = RectConstructor(643, 145, 72, 56);

	Portrait newPortrait;
	newPortrait.position.x = worldPosition.x;
	newPortrait.position.y = nextVectorPosition;
	
	newPortrait.background = new UI_Image(newPortrait.position, this, UI_TYPE::UI_IMG, rect, (P2SString)"backgroundPortrait", false);
	
//	newPortrait.backgroundLevel = ;		TODO
	
	rect = RectConstructor(29, 76, 68, 14);
	newPortrait.healthbar = new UI_Healthbar(fMPoint(newPortrait.position.x - 5, newPortrait.position.y + 42), this, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"healthbarPortrait", false);
	
//	newPortrait.level = ;				TODO

	switch (entity->GetType())
	{
	case ENTITY_TYPE::HERO_GATHERER:
		rect = RectConstructor(351, 149, 68, 52);
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x - 2, newPortrait.position.y + 5), this, UI_TYPE::UI_IMG, rect, (P2SString)"gathererProtrait", false);
		break;
	case ENTITY_TYPE::HERO_RANGED:
		rect = RectConstructor(147, 149, 68, 52);
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x - 2, newPortrait.position.y + 5), this, UI_TYPE::UI_IMG, rect, (P2SString)"rangedPortrait", false);
		break;
	case ENTITY_TYPE::HERO_MELEE:
		rect = RectConstructor(561, 149, 68, 52);
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x - 2, newPortrait.position.y + 5), this, UI_TYPE::UI_IMG, rect, (P2SString)"meleePortrait", false);
		break;
	default:
		assert(0 == 1);
		break;
	}

	portraitVector.push_back(newPortrait);
}

SDL_Rect UI_Portrait::RectConstructor(int x, int y, int w, int h)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}
