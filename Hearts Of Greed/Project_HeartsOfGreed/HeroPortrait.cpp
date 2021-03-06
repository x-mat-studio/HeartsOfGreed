#include "HeroPortrait.h"
#include "UIManager.h"
#include "Hero.h"

HeroPortrait::HeroPortrait(Hero* hero) :

	UI(fMPoint(3, 0), (UI*)app->uiManager->GetPortraitManager(), UI_TYPE::HERO_PORTRAIT, { 0, 0, 0, 0 }, false, false, nullptr),

	hero(hero),

	life(hero->GetCurrentHP()),
	mana(hero->GetEnergyPoints()),

	healthRect(nullptr),
	manaRect(nullptr),

	focused(false),
	heroType(ENTITY_TYPE::UNKNOWN),
	originalBarsWidth(0)
{}


HeroPortrait::~HeroPortrait()
{
	hero = nullptr;
	healthRect = nullptr;
	manaRect = nullptr;

	int numberElements = uiElementsVector.size();

	for (int i = numberElements - 1; i >= 0; i--)
	{
		delete uiElementsVector[i];
		uiElementsVector[i] = nullptr;

		uiElementsVector.erase(uiElementsVector.begin() + i);
	}

	uiElementsVector.clear();
}


bool HeroPortrait::PreUpdate(float dt)
{
	focused = hero->selectedByPlayer;

	if (hero->GetCurrentHP() != life && hero->GetCurrentHP() > 0)
	{
		life = hero->GetCurrentHP();

		healthRect->w = life * originalBarsWidth / hero->hitPointsMax;

		if (healthRect->w == 0)
		{
			healthRect->w = 1;
		}
	}

	int energyPoints = hero->GetEnergyPoints();

	if (energyPoints != mana && energyPoints > 0)
	{
		mana = energyPoints;

		manaRect->w = mana * originalBarsWidth / hero->GetMaxEnergyPoints();

		if (manaRect->w == 0)
		{
			manaRect->w = 1;
		}
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

	if (focused == true)
	{
		SDL_SetTextureColorMod(app->uiManager->GetAtlasTexture(), 65, 169, 76);
	}

	for (size_t i = 0; i < numElements; i++)
	{
		uiElementsVector[i]->PostUpdate(dt);
	}

	SDL_SetTextureColorMod(app->uiManager->GetAtlasTexture(), 255, 255, 255);

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


Hero* HeroPortrait::GetHero()
{
	return hero;
}


bool HeroPortrait::OnAbove()
{
	bool ret = false;

	int elementsNumber = uiElementsVector.size();

	for (int i = elementsNumber - 1; i >= 0 && ret == false; i--)
	{
		if (uiElementsVector[i]->OnAbove() == true)
		{
			i--;

			for (i; i >= 0; i--)
			{
				uiElementsVector[i]->UnFocus();
			}

			ret = true;
		}
	}

	return ret;
}


UI* HeroPortrait::SearchFocus()
{
	int numElem = uiElementsVector.size();
	UI* focusUI;

	for (int i = numElem - 1; i >= 0; i--)
	{
		focusUI = uiElementsVector[i]->SearchFocus();

		if (focusUI != nullptr)
		{
			return focusUI;
		}
	}

	return nullptr;
}


void HeroPortrait::UnFocus()
{
	int elementsNumber = uiElementsVector.size();

	for (int i = 0; i < elementsNumber; i++)
	{
		uiElementsVector[i]->UnFocus();
	}
}