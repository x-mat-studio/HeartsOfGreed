#include "UI_Portrait.h"

UI_Portrait::UI_Portrait(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable),
	nextVectorPosition(positionValue.y),
	backgroundRect(RectConstructor(643, 145, 72, 56)),
	backgroundHealthbarRect(RectConstructor(34, 22, 52, 10)),
	backgroundLevelRect(RectConstructor(55, 210, 18, 18)),
	healthbarRect(RectConstructor(29, 79, 50, 8)),
	meleePortraitRect(RectConstructor(561, 149, 68, 52)),
	gathererPortraitRect(RectConstructor(351, 149, 68, 52)),
	rangedPortraitRect(RectConstructor(147, 149, 68, 52)),
	no_move(true)
{}

UI_Portrait::~UI_Portrait()
{

	int numElements = portraitVector.size();

	for (int i = 0; i < numElements; i++)
	{
		RELEASE(portraitVector[i].background);
		RELEASE(portraitVector[i].backgroundLevel);
		RELEASE(portraitVector[i].backgroundHealthbar);
		RELEASE(portraitVector[i].healthbar);
		RELEASE(portraitVector[i].level);
		RELEASE(portraitVector[i].portrait);
		portraitVector[i].background = nullptr;
		portraitVector[i].backgroundLevel = nullptr;
		portraitVector[i].backgroundHealthbar = nullptr;
		portraitVector[i].healthbar = nullptr;
		portraitVector[i].level = nullptr;
		portraitVector[i].portrait = nullptr;
	}

	portraitVector.clear();

}

bool UI_Portrait::Start()
{
	return true;
}

bool UI_Portrait::PreUpdate(float dt)
{

	int numElem = portraitVector.size();

	for (int i = 0; i < numElem; i++)
	{
		portraitVector[i].healthbar->PreUpdate(dt);
		portraitVector[i].level->PreUpdate(dt);
	}

	return true;
}

bool UI_Portrait::Update(float dt)
{
	
	int numElem = portraitVector.size();

	for (int i = 0; i < numElem; i++)
	{
		if(hiding_unhiding == false)
		{ 
			no_move = true;
		}
		else if (no_move == true && hiding_unhiding == true)
		{
			portraitVector[i].healthbar->hiding_unhiding = true;
			portraitVector[i].level->hiding_unhiding = true;
			portraitVector[i].background->hiding_unhiding = true;
			portraitVector[i].backgroundLevel->hiding_unhiding = true;
			portraitVector[i].backgroundHealthbar->hiding_unhiding = true;
			portraitVector[i].portrait->hiding_unhiding = true;
			no_move = false;
		}

		portraitVector[i].healthbar->Update(dt);
		portraitVector[i].level->Update(dt);
		portraitVector[i].background->Update(dt);
		portraitVector[i].backgroundLevel->Update(dt);
		portraitVector[i].backgroundHealthbar->Update(dt);
		portraitVector[i].portrait->Update(dt);
	}

	if (hiding_unhiding == true)
	{
		Hide(dt);
	}

	return true;
}

bool UI_Portrait::PostUpdate(float dt)
{
	
	int numElem = portraitVector.size();

	for (int i = 0; i < numElem; i++)
	{
		portraitVector[i].background->PostUpdate(dt);
		portraitVector[i].portrait->PostUpdate(dt);
		portraitVector[i].backgroundLevel->PostUpdate(dt);
		portraitVector[i].backgroundHealthbar->PostUpdate(dt);
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

	Portrait newPortrait;
	newPortrait.position.x = worldPosition.x;
	newPortrait.position.y = nextVectorPosition;
	
	newPortrait.background = new UI_Image(newPortrait.position, this, UI_TYPE::UI_IMG, backgroundRect, (P2SString)"backgroundPortrait", DRAGGABLE::DRAG_OFF);
	
	newPortrait.backgroundLevel = new UI_Image(fMPoint (newPortrait.position.x, newPortrait.position.y + 38), this, UI_TYPE::UI_IMG, backgroundLevelRect, (P2SString)"backgroundLevelPortrait", DRAGGABLE::DRAG_OFF);

	newPortrait.backgroundHealthbar = new UI_Image(fMPoint (newPortrait.position.x + 18, newPortrait.position.y + 45), this, UI_TYPE::UI_IMG, backgroundHealthbarRect, (P2SString)"backgroundHealthbarPortrait", DRAGGABLE::DRAG_OFF);
	
	newPortrait.healthbar = new UI_Healthbar(fMPoint(newPortrait.position.x + 19, newPortrait.position.y + 46), this, UI_TYPE::UI_HEALTHBAR, healthbarRect, (P2SString)"healthbarPortrait", nullptr, DRAGGABLE::DRAG_OFF);
	
	SDL_Color color;
	char level;
	color.r = 255;
	color.b = 255;
	color.g = 255;
	sprintf_s(&level, 10, "%d", entity->level);
	newPortrait.level = new UI_Text(fMPoint(newPortrait.position.x + 5, newPortrait.position.y + 34), this, UI_TYPE::UI_TEXT, healthbarRect, (P2SString)"textPortrait", DRAGGABLE::DRAG_OFF, &level, color);

	switch (entity->GetType())
	{
	case ENTITY_TYPE::HERO_GATHERER:
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x + 2, newPortrait.position.y + 3), this, UI_TYPE::UI_IMG, gathererPortraitRect, (P2SString)"gathererProtrait", DRAGGABLE::DRAG_OFF);
		break;
	case ENTITY_TYPE::HERO_RANGED:
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x + 2, newPortrait.position.y + 3), this, UI_TYPE::UI_IMG, rangedPortraitRect, (P2SString)"rangedPortrait", DRAGGABLE::DRAG_OFF);
		break;
	case ENTITY_TYPE::HERO_MELEE:
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x + 2, newPortrait.position.y + 3), this, UI_TYPE::UI_IMG, meleePortraitRect, (P2SString)"meleePortrait", DRAGGABLE::DRAG_OFF);
		break;
	default:
		assert(0 == 1);
		break;
	}

	newPortrait.hero = entity;

	portraitVector.push_back(newPortrait);
}

void UI_Portrait::DeletePortrait(Portrait portrait)
{
	// TODO
}

void UI_Portrait::Move()
{
	// TODO
}

void UI_Portrait::CheckLevel()
{
	// TODO
}

void UI_Portrait::ReWriteLevelTexture()
{
	// TODO
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
