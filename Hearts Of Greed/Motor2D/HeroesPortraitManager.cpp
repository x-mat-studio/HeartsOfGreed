#include "HeroesPortraitManager.h"
#include "HeroPortrait.h"
#include "UIManager.h"

HeroesPortraitManager::HeroesPortraitManager(float x, float y, UI* parent, bool dragable) :

	UI(fMPoint(x, y), parent, UI_TYPE::HERO_PORTRAIT_MANAGER, {0, 0, 0, 0}, false, false, nullptr)

{
	app->uiManager->SetPortraitManager(this);
}


HeroesPortraitManager::~HeroesPortraitManager()
{
	int heroPortraitsNumber = heroPortraitsVector.size();

	for (int i = heroPortraitsNumber; i >= 0; i--)
	{
		delete heroPortraitsVector[i];
		heroPortraitsVector[i] = nullptr;

		heroPortraitsVector.erase(heroPortraitsVector.begin() + i);
	}
}


bool HeroesPortraitManager::PreUpdate(float dt)
{
	int heroPortraitsNumber = heroPortraitsVector.size();

	for (int i = 0; i < heroPortraitsNumber; i++)
	{
		heroPortraitsVector[i]->PreUpdate(dt);
	}

	return true;
}


bool HeroesPortraitManager::Update(float dt)
{
	int heroPortraitsNumber = heroPortraitsVector.size();

	for (int i = 0; i < heroPortraitsNumber; i++)
	{
		heroPortraitsVector[i]->Update(dt);
	}

	return true;
}


bool HeroesPortraitManager::PostUpdate(float dt)
{
	int heroPortraitsNumber = heroPortraitsVector.size();

	for (int i = 0; i < heroPortraitsNumber; i++)
	{
		heroPortraitsVector[i]->SetLocalPosition(fMPoint(3, 60*i));
		heroPortraitsVector[i]->PostUpdate(dt);
	}
	return true;
}


void HeroesPortraitManager::AddPortrait(HeroPortrait* portrait)
{
	heroPortraitsVector.push_back(portrait);
}


void HeroesPortraitManager::DeletePortrait(HeroPortrait* portrait)
{
	int heroPortraitsNumber = heroPortraitsVector.size();

	for (int i = 0; i < heroPortraitsNumber; i++)
	{
		if (heroPortraitsVector[i] == portrait)
		{
			delete heroPortraitsVector[i];
			heroPortraitsVector[i] = nullptr;

			heroPortraitsVector.erase(heroPortraitsVector.begin() + i);
		}
	}
}