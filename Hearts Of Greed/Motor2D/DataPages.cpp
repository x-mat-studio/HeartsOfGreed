#include "DataPages.h"
#include "Player.h"
#include "UIManager.h"
#include "UIFactory.h"
#include "Base.h"
#include "Turret.h"
#include "Enemy.h"
#include "Hero.h"

DataPages::DataPages(float x, float y, UI* parent, Entity* entity) : UI({ x, y }, parent, UI_TYPE::DATA_PAGES, { 0, 0, 0, 0 }, true, false, nullptr),
state(DATA_PAGE_ENUM::FOCUSED_NONE),
focusEntity(entity),
factory(app->uiManager->GetFactory())
{}

DataPages::~DataPages()
{
	DeleteCurrentData();
}

bool DataPages::PreUpdate(float dt)
{
	Entity* focus = app->player->GetFocusedEntity();

	if (focusEntity != focus)
	{
		DeleteCurrentData();
		focusEntity = focus;
	}

	if (focus != nullptr)
	{
		switch (state)
		{
		case DATA_PAGE_ENUM::FOCUSED_NONE:

			switch (focus->GetType())
			{

			case ENTITY_TYPE::HERO_GATHERER:
				factory->CreateGathererPage(&dataPageVector, this);
				GetHeroValue();
				state = DATA_PAGE_ENUM::FOCUSED_GATHERER;
				break;
			case ENTITY_TYPE::HERO_MELEE:
				factory->CreateMeleePage(&dataPageVector, this);
				GetHeroValue();
				state = DATA_PAGE_ENUM::FOCUSED_MELEE;
				break;
			case ENTITY_TYPE::HERO_RANGED:
				factory->CreateRangedPage(&dataPageVector, this);
				GetHeroValue();
				state = DATA_PAGE_ENUM::FOCUSED_RANGED;
				break;
			case ENTITY_TYPE::ENEMY:
				factory->CreateWanamingoPage(&dataPageVector, this);
				GetWanamingoValue();
				state = DATA_PAGE_ENUM::FOCUSED_WANAMINGO;
				break;
			case ENTITY_TYPE::BLDG_TURRET:
				factory->CreateTurretPage(&dataPageVector, this);
				GetTurretValue();
				break;
			case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
				factory->CreateUpgradeCenterPage(&dataPageVector, this);
				GetUpgradeCenterValue();
				state = DATA_PAGE_ENUM::FOCUSED_UPGRADE_CENTER;
				break;
			case ENTITY_TYPE::BLDG_BASE:
				factory->CreateBasePage(&dataPageVector, this);
				GetBaseValue();
				state = DATA_PAGE_ENUM::FOCUSED_BASE;
				break;
			case ENTITY_TYPE::BLDG_BARRICADE:
				factory->CreateBarricadePage(&dataPageVector, this);
				GetBarricadeValue();
				state = DATA_PAGE_ENUM::FOCUSED_BARRICADE;
				break;
			default:
				state = DATA_PAGE_ENUM::FOCUSED_UNKNOWN;
				break;
			}
			break;
		case DATA_PAGE_ENUM::FOCUSED_BASE:
			CheckBaseValues();
			break;
		case DATA_PAGE_ENUM::FOCUSED_GATHERER:
			CheckHeroesValues();
			break;
		case DATA_PAGE_ENUM::FOCUSED_MELEE:
			CheckHeroesValues();
			break;
		case DATA_PAGE_ENUM::FOCUSED_RANGED:
			CheckHeroesValues();
			break;
		case DATA_PAGE_ENUM::FOCUSED_TURRET:
			CheckTurretValues();
			state = DATA_PAGE_ENUM::FOCUSED_TURRET;
			break;
		case DATA_PAGE_ENUM::FOCUSED_UPGRADE_CENTER:
			CheckUpgradeCenterValues();
			break;
		case DATA_PAGE_ENUM::FOCUSED_WANAMINGO:
			CheckWanamingoValues();
			break;
		case DATA_PAGE_ENUM::FOCUSED_UNKNOWN:
		default:
			break;
		}
	}

	int numElem = dataPageVector.size();

	for (int i = 0; i < numElem; i++)
	{
		dataPageVector[i]->PreUpdate(dt);
	}

	focus = nullptr;

	return true;
}


bool DataPages::Update(float dt)
{
	int numElem = dataPageVector.size();

	for (int i = 0; i < numElem; i++)
	{
		dataPageVector[i]->Update(dt);
	}

	return true;
}


bool DataPages::PostUpdate(float dt)
{
	int numElem = dataPageVector.size();

	for (int i = 0; i < numElem; i++)
	{
		dataPageVector[i]->PostUpdate(dt);
	}

	return true;
}


bool DataPages::CheckData(int previous, int current)
{
	if (previous == current)
	{
		return true;
	}
	return false;
}



void DataPages::CheckHeroesValues()
{
	bool check = false;

	Hero* focus = (Hero*)focusEntity;

	if (CheckData(attackDamage, focus->attackDamage))
	{
		if (CheckData(attackSpeed, focus->attackSpeed))
		{
			if (CheckData(range, focus->attackRange))
			{
				if (CheckData(hpRecovery, focus->recoveryHitPointsRate))
				{
					if (CheckData(xpToNextLevel, focus->expToLevelUp))
					{
						check = true;
					}
				}
			}
		}
	}

	if (check == false)
	{
		DeleteCurrentData();
	}

	focus = nullptr;
}


void DataPages::CheckWanamingoValues()
{
	bool check = false;

	Enemy* focus = (Enemy*)focusEntity;

	if (CheckData(attackDamage, focus->GetAD()))
	{
		if (CheckData(attackSpeed, focus->GetAS()))
		{
			if (CheckData(hpRecovery, focus->GetRecov()))
			{
				if (CheckData(vision, focus->GetVision()))
				{
					check = true;
				}
			}
		}
	}

	if (check == false)
	{
		DeleteCurrentData();
	}

	focus = nullptr;
}


void DataPages::CheckBarricadeValues()
{}


void DataPages::CheckBaseValues()
{
	bool check = false;

	Base* focus = (Base*)focusEntity;

	if (CheckData(resources, focus->GetRsrc()))
	{
		check = true;
	}

	if (check == false)
	{
		DeleteCurrentData();
	}

	focus = nullptr;
}


void DataPages::CheckTurretValues()
{
	bool check = false;

	Turret* focus = (Turret*)focusEntity;

	if (CheckData(level, focus->GetLvl()))
	{
		if (CheckData(attackDamage, focus->GetAD()))
		{
			if (CheckData(attackSpeed, focus->GetAS()))
			{
				if (CheckData(range, focus->GetRng()))
				{
					check = true;
				}
			}
		}
	}

	if (check == false)
	{
		DeleteCurrentData();
	}

	focus = nullptr;
}


void DataPages::CheckUpgradeCenterValues()
{}

void DataPages::GetHeroValue()
{
	Hero* focus = (Hero*)app->player->GetFocusedEntity();

	attackDamage = focus->attackDamage;
	attackSpeed = focus->attackSpeed;
	range = focus->attackRange;
	hpRecovery = focus->recoveryHitPointsRate;
	xpToNextLevel = focus->expToLevelUp;
}


void DataPages::GetWanamingoValue()
{
	Enemy* focus = (Enemy*)app->player->GetFocusedEntity();

	attackDamage = focus->GetAD();
	attackSpeed = focus->GetAS();
	vision = focus->GetVision();
	hpRecovery = focus->GetRecov();
}


void DataPages::GetBaseValue()
{
	Base* focus = (Base*)app->player->GetFocusedEntity();

	resources = focus->GetRsrc();
}


void DataPages::GetTurretValue()
{
	Turret* focus = (Turret*)app->player->GetFocusedEntity();

	level = focus->GetLvl();
	attackDamage = focus->GetAD();
	attackSpeed = focus->GetAS();
	range = focus->GetRng();
}


void DataPages::GetUpgradeCenterValue()
{

}


void DataPages::GetBarricadeValue()
{

}


void DataPages::DeleteCurrentData()
{
	int numElem = dataPageVector.size();

	for (int i = numElem - 1; i >= 0; i--)
	{
		RELEASE(dataPageVector[i]);
		dataPageVector[i] = nullptr;

		dataPageVector.erase(dataPageVector.begin() + i);
	}

	dataPageVector.clear();

	resources = -1;
	level = -1;
	attackDamage = -1;
	attackSpeed = -1;
	range = -1;
	vision = -1;
	hpRecovery = -1;
	xpToNextLevel = -1;

	focusEntity = nullptr;
	state = DATA_PAGE_ENUM::FOCUSED_NONE;
}
