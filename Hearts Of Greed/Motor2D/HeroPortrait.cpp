#include "HeroPortrait.h"
#include "UIManager.h"
#include "Hero.h"

HeroPortrait::HeroPortrait(Hero* hero) :

	UI(fMPoint(3, 0), (UI*)app->uiManager->GetPortraitManager(), UI_TYPE::HERO_PORTRAIT, { 0, 0, 0, 0 }, false, false, nullptr),

	hero(hero),

	life(hero->hitPointsCurrent),
	mana(hero->energyPoints),

	healthRect(nullptr),
	manaRect(nullptr)
{
}


HeroPortrait::~HeroPortrait()
{
	hero = nullptr;

	int numberElements = uiElementsVector.size();

	for (int i = numberElements; i >= 0; i--)
	{
		delete uiElementsVector[i];
		uiElementsVector[i] = nullptr;

		uiElementsVector.erase(uiElementsVector.begin() + i);
	}

	uiElementsVector.clear();
}


bool HeroPortrait::PreUpdate(float dt)
{

	if (hero->hitPointsCurrent != life && hero->hitPointsCurrent != 0)
	{
		life = hero->hitPointsCurrent;

		healthRect->w = life * originalBarsWidth / hero->hitPointsMax;
	}

	if (hero->hitPointsCurrent != life && hero->hitPointsCurrent != 0)
	{
		life = hero->hitPointsCurrent;

		healthRect->w = life * originalBarsWidth / hero->hitPointsMax;
	}

	int numElements = uiElementsVector.size();

	for (size_t i = 0; i < numElements; i++)
	{
		uiElementsVector[i]->PreUpdate(dt);
	}

	return true;
}


bool HeroPortrait::Update(float dt)
{
	Move();

	int numElements = uiElementsVector.size();

	for (size_t i = 0; i < numElements; i++)
	{
		uiElementsVector[i]->Update(dt);
	}

	return true;
}


bool HeroPortrait::PostUpdate(float dt)
{
	int numElements = uiElementsVector.size();

	for (size_t i = 0; i < numElements; i++)
	{
		uiElementsVector[i]->PostUpdate(dt);
	}

	return true;
}


void HeroPortrait::AddElement(UI* element)
{
	uiElementsVector.push_back(element);
}


void HeroPortrait::AddHealthBar(UI* element)
{
	healthRect = &element->rect;
	originalBarsWidth = healthRect->w;
	uiElementsVector.push_back(element);
}


void HeroPortrait::AddManaBar(UI* element)
{
	manaRect = &element->rect;
	uiElementsVector.push_back(element);
}