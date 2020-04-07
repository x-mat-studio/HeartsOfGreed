#include "UI_Portrait.h"

UI_Portrait::UI_Portrait(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable),
	nextVectorPosition(positionValue.y)
{}

UI_Portrait::~UI_Portrait()
{
	//DELETE ALL ELEMENTS IN A COOL WAY		TODO
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
	// CALL THE DRAW OF ALL ELEMENTS		TODO
	int numElem = portraitVector.size();

	for (int i = 0; i < numElem; i++)
	{
		portraitVector[i].background->PostUpdate(dt);
		portraitVector[i].backgroundLevel->PostUpdate(dt);
		portraitVector[i].portrait->PostUpdate(dt);
		portraitVector[i].healthbar->PostUpdate(dt);
		portraitVector[i].level->PostUpdate(dt);
	}

	return true;
}

void UI_Portrait::HandleInput()
{

}

void UI_Portrait::CreatePortrait(Hero* entity)
{
	SDL_Rect rect = RectConstructor(643, 145, 72, 56);

	Portrait newPortrait;
	newPortrait.position.x = worldPosition.x;
	newPortrait.position.y = nextVectorPosition;

	float secondaryHeight = newPortrait.position.y + 42;
	
	newPortrait.background = new UI_Image(newPortrait.position, this, UI_TYPE::UI_IMG, rect, (P2SString)"backgroundPortrait", DRAGGABLE::DRAG_OFF);
	
	rect = RectConstructor(55, 210, 18, 18);
	newPortrait.backgroundLevel = new UI_Image(fMPoint (newPortrait.position.x, secondaryHeight), this, UI_TYPE::UI_IMG, rect, (P2SString)"backgroundLevelPortrait", DRAGGABLE::DRAG_OFF);
	
	rect = RectConstructor(29, 76, 68, 14);
	newPortrait.healthbar = new UI_Healthbar(fMPoint(newPortrait.position.x + 5, secondaryHeight), this, UI_TYPE::UI_HEALTHBAR, rect, (P2SString)"healthbarPortrait", DRAGGABLE::DRAG_OFF);
	
	SDL_Color color;
	color.r = 255;
	color.b = 255;
	color.g = 255;
	rect = RectConstructor(0, 0, 0, 0);
	newPortrait.level = new UI_Text(fMPoint(newPortrait.position.x + 1, secondaryHeight), this, UI_TYPE::UI_TEXT, rect, (P2SString)"textPortrait", DRAGGABLE::DRAG_OFF, (char*)entity->level, color);

	switch (entity->GetType())
	{
	case ENTITY_TYPE::HERO_GATHERER:
		rect = RectConstructor(351, 149, 68, 52);
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x + 2, newPortrait.position.y + 5), this, UI_TYPE::UI_IMG, rect, (P2SString)"gathererProtrait", DRAGGABLE::DRAG_OFF);
		break;
	case ENTITY_TYPE::HERO_RANGED:
		rect = RectConstructor(147, 149, 68, 52);
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x + 2, newPortrait.position.y + 5), this, UI_TYPE::UI_IMG, rect, (P2SString)"rangedPortrait", DRAGGABLE::DRAG_OFF);
		break;
	case ENTITY_TYPE::HERO_MELEE:
		rect = RectConstructor(561, 149, 68, 52);
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x + 2, newPortrait.position.y + 5), this, UI_TYPE::UI_IMG, rect, (P2SString)"meleePortrait", DRAGGABLE::DRAG_OFF);
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
