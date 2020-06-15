#include "HeroesPortraitManager.h"
#include "HeroPortrait.h"
#include "UIManager.h"

HeroesPortraitManager::HeroesPortraitManager(float x, float y, UI* parent, bool dragable) :

	UI(fMPoint(x, y), parent, UI_TYPE::HERO_PORTRAIT_MANAGER, { 0, 0, 0, 0 }, false, false, nullptr)

{
	app->uiManager->SetPortraitManager(this);
}


HeroesPortraitManager::~HeroesPortraitManager()
{
	int heroPortraitsNumber = heroPortraitsVector.size();

	for (int i = heroPortraitsNumber - 1; i >= 0; i--)
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
	Move();

	int heroPortraitsNumber = heroPortraitsVector.size();

	for (int i = 0; i < heroPortraitsNumber; i++)
	{
		heroPortraitsVector[i]->SetLocalPosition(fMPoint(-35, 48 * i + 30));
		heroPortraitsVector[i]->Update(dt);
	}

	return true;
}


bool HeroesPortraitManager::PostUpdate(float dt)
{
	int heroPortraitsNumber = heroPortraitsVector.size();

	for (int i = 0; i < heroPortraitsNumber; i++)
	{
		
		heroPortraitsVector[i]->PostUpdate(dt);
	}

	return true;
}


void HeroesPortraitManager::AddPortrait(HeroPortrait* portrait)
{
	portrait->SetFather(this);

	if (portrait->position != -1)
	{
		if (portrait->position >= heroPortraitsVector.size())
		{
			heroPortraitsVector.push_back(portrait);
		}

		else
			heroPortraitsVector.insert(heroPortraitsVector.begin() + portrait->position, portrait);
	}
}


void HeroesPortraitManager::DeletePortrait(Hero* portrait)
{
	int heroPortraitsNumber = heroPortraitsVector.size();

	for (int i = 0; i < heroPortraitsNumber; i++)
	{
		if (heroPortraitsVector[i]->GetHero() == portrait)
		{
			delete heroPortraitsVector[i];
			heroPortraitsVector[i] = nullptr;

			heroPortraitsVector.erase(heroPortraitsVector.begin() + i);

			break;
		}
	}
}


bool HeroesPortraitManager::OnAbove()
{
	bool ret = false;

	int elementsNumber = heroPortraitsVector.size();

	for (int i = elementsNumber - 1; i >= 0 && ret == false; i--)
	{
		if (heroPortraitsVector[i]->OnAbove() == true)
		{
			i--;

			for (i; i >= 0; i--)
			{
				heroPortraitsVector[i]->UnFocus();
			}

			ret = true;
		}
	}

	return ret;
}


UI* HeroesPortraitManager::SearchFocus()
{
	int numElem = heroPortraitsVector.size();
	UI* focusUI;

	for (int i = numElem - 1; i >= 0; i--)
	{
		focusUI = heroPortraitsVector[i]->SearchFocus();

		if (focusUI != nullptr)
		{
			return focusUI;
		}
	}

	return nullptr;
}


void HeroesPortraitManager::UnFocus()
{
	int elementsNumber = heroPortraitsVector.size();

	for (int i = 0; i < elementsNumber; i++)
	{
		heroPortraitsVector[i]->UnFocus();
	}
}