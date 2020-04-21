#include "UI_Portrait.h"
#include "Render.h"
#include "Window.h"

UI_Portrait::UI_Portrait(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable) : UI(positionValue, father, uiType, rect, uiName, draggable),
	nextVectorPosition(positionValue.y),
	backgroundRect{ 643, 145, 72, 56 },
	backgroundHealthbarRect{ 34, 22, 52, 10 },
	backgroundLevelRect{55, 210, 18, 18},
	healthbarRect{ 29, 79, 50, 8 },
	meleePortraitRect{ 561, 149, 68, 52 },
	gathererPortraitRect{ 351, 149, 68, 52 },
	rangedPortraitRect{ 147, 149, 68, 52 },
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
		portraitVector[i].hero = nullptr;
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

		if (portraitVector[i].lvl != portraitVector[i].hero->level)
		{
			portraitVector[i].ChangeLvl(portraitVector[i].hero->level);
		}
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
			if (i == numElem - 1)
			{
				no_move = false;
			}
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
		if(portraitVector[i].background != nullptr)
		portraitVector[i].background->PostUpdate(dt);

		if (portraitVector[i].portrait != nullptr)
		portraitVector[i].portrait->PostUpdate(dt);

		if (portraitVector[i].backgroundLevel != nullptr)
		portraitVector[i].backgroundLevel->PostUpdate(dt);

		if (portraitVector[i].backgroundHealthbar != nullptr)
		portraitVector[i].backgroundHealthbar->PostUpdate(dt);

		if (portraitVector[i].healthbar != nullptr)
		portraitVector[i].healthbar->PostUpdate(dt);

		if (portraitVector[i].level != nullptr)
		portraitVector[i].level->PostUpdate(dt);
		
		if (portraitVector[i].hero != nullptr) 
		{

			if (portraitVector[i].hero->selectedByPlayer == true) 
			{
				SDL_Rect portPos{ this->portraitVector[i].portrait->worldPosition.x * app->win->GetUIScale() , this->portraitVector[i].portrait->worldPosition.y * app->win->GetUIScale(), this->portraitVector[i].portrait->box.w * app->win->GetUIScale(),this->portraitVector[i].portrait->box.h * app->win->GetUIScale() };
				app->render->DrawQuad(portPos, 120, 100, 10, 80, true, false);
			}

		}
		
	}

	return true;
}

void UI_Portrait::HandleInput()
{}

void UI_Portrait::CreatePortrait(Hero* entity)
{
	Portrait newPortrait;
	newPortrait.position.x = worldPosition.x;
	newPortrait.position.y = nextVectorPosition;
	
	newPortrait.background = new UI_Image(newPortrait.position, this, UI_TYPE::UI_IMG, backgroundRect, P2SString("backgroundPortrait"), DRAGGABLE::DRAG_OFF);
	
	newPortrait.backgroundLevel = new UI_Image(fMPoint (newPortrait.position.x, newPortrait.position.y + 38), this, UI_TYPE::UI_IMG, backgroundLevelRect, P2SString("backgroundLevelPortrait"), DRAGGABLE::DRAG_OFF);

	newPortrait.backgroundHealthbar = new UI_Image(fMPoint (newPortrait.position.x + 18, newPortrait.position.y + 45), this, UI_TYPE::UI_IMG, backgroundHealthbarRect, P2SString("backgroundHealthbarPortrait"), DRAGGABLE::DRAG_OFF);
	
	newPortrait.healthbar = new UI_Healthbar(fMPoint(newPortrait.position.x + 19, newPortrait.position.y + 46), this, UI_TYPE::UI_HEALTHBAR, healthbarRect, P2SString("healthbarPortrait"), entity, DRAGGABLE::DRAG_OFF);
	
	SDL_Color color;
	char level [10];
	color.r = 255;
	color.b = 255;
	color.g = 255;

	sprintf_s(level, 10, "%d", entity->level);
	newPortrait.level = new UI_Text(fMPoint(newPortrait.position.x + 5, newPortrait.position.y + 34), this, UI_TYPE::UI_TEXT, healthbarRect, P2SString("textPortrait"), DRAGGABLE::DRAG_OFF, level, color);

	switch (entity->GetType())
	{
	case ENTITY_TYPE::HERO_GATHERER:
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x + 2, newPortrait.position.y + 3), this, UI_TYPE::UI_IMG, gathererPortraitRect, P2SString("gathererProtrait"), DRAGGABLE::DRAG_OFF);
		break;
	case ENTITY_TYPE::HERO_RANGED:
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x + 2, newPortrait.position.y + 3), this, UI_TYPE::UI_IMG, rangedPortraitRect, P2SString("rangedPortrait"), DRAGGABLE::DRAG_OFF);
		break;
	case ENTITY_TYPE::HERO_MELEE:
		newPortrait.portrait = new UI_Image(fMPoint(newPortrait.position.x + 2, newPortrait.position.y + 3), this, UI_TYPE::UI_IMG, meleePortraitRect, P2SString("meleePortrait"), DRAGGABLE::DRAG_OFF);
		break;
	default:
		break;
	}

	newPortrait.hero = entity;

	portraitVector.push_back(newPortrait);
}

void UI_Portrait::DeletePortrait()
{
	int numElem = portraitVector.size();

	bool deleted = false;

	for (int i = 0; i < numElem; i++)
	{
		if (portraitVector[i].hero == nullptr || portraitVector[i].hero->toDelete == true)
		{
			delete portraitVector[i].background;
			portraitVector[i].background = nullptr;

			delete portraitVector[i].backgroundLevel;
			portraitVector[i].backgroundLevel = nullptr;

			delete portraitVector[i].backgroundHealthbar;
			portraitVector[i].backgroundHealthbar = nullptr;

			delete portraitVector[i].healthbar;
			portraitVector[i].healthbar = nullptr;

			delete portraitVector[i].portrait;
			portraitVector[i].portrait = nullptr;

			delete portraitVector[i].level;
			portraitVector[i].level = nullptr;

			portraitVector[i].hero = nullptr;

			portraitVector.erase(portraitVector.begin() + i);

			deleted = true;
			nextVectorPosition -= 60;
			numElem = portraitVector.size();
			i--;
		}
		else if (deleted == true)
		{
			portraitVector[i].background->worldPosition.y -= 60;
			portraitVector[i].backgroundLevel->worldPosition.y -= 60;
			portraitVector[i].backgroundHealthbar->worldPosition.y -= 60;
			portraitVector[i].healthbar->worldPosition.y -= 60;
			portraitVector[i].portrait->worldPosition.y -= 60;
			portraitVector[i].level->worldPosition.y -= 60;
		}
	}
}

void UI_Portrait::Move()
{}

void Portrait::ChangeLvl(int newlvl)
{
	this->lvl = newlvl;
	char bufferText [10];

	sprintf_s(bufferText, 10, "%d", newlvl);

	this->level->LoadNewTexture(bufferText, nullptr);

}



