#include "DataPages.h"
#include "Player.h"
#include "UIManager.h"
#include "UIFactory.h"
#include "Base.h"
#include "Barricade.h"
#include "Turret.h"
#include "UpgradeCenter.h"
#include "Enemy.h"
#include "NightEnemy.h"
#include "RangedEnemy.h"
#include "GigaEnemy.h"
#include "Hero.h"

DataPages::DataPages(float x, float y, UI* parent, Entity* entity) :

	UI({ x, y }, parent, UI_TYPE::DATA_PAGES, { 0, 0, 0, 0 }, true, false, nullptr),

	state(DATA_PAGE_ENUM::FOCUSED_NONE),

	factory(app->uiManager->GetFactory()),
	focusEntity(entity),
	healthRect(nullptr),
	energyRect(nullptr),
	alignment(ENTITY_ALIGNEMENT::UNKNOWN),
	attackDamage(0),
	attackSpeed(0),
	hpRecovery(0),
	level(0),
	life(0),
	lifeMax(0),
	energy(0),
	energyMax(0),
	range(0),
	resources(0),
	vision(0),
	xpToNextLevel(0),
	heroSkillPoints(0),
	skillResource(0)
{
	originalBarsWidth = factory->GetHealthBarBackground().w;
}

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

			case ENTITY_TYPE::HERO_ROBO:

				factory->CreateRobottoPage(&dataPageVector, this);
				GetHeroValue();
				state = DATA_PAGE_ENUM::FOCUSED_ROBOTTO;
				break;

			case ENTITY_TYPE::ENEMY:

				factory->CreateWanamingoPage(&dataPageVector, this);
				GetWanamingoValue();
				state = DATA_PAGE_ENUM::FOCUSED_WANAMINGO;
				break;

			case ENTITY_TYPE::ENEMY_NIGHT:

				factory->CreateSpeedomingoPage(&dataPageVector, this);
				GetSpeedomingoValue();
				state = DATA_PAGE_ENUM::FOCUSED_SPEEDOMINGO;
				break;

			case ENTITY_TYPE::ENEMY_RANGED:

				factory->CreateSnipermingoPage(&dataPageVector, this);
				GetSnipermingoValue();
				state = DATA_PAGE_ENUM::FOCUSED_SNIPERMINGO;
				break;

			case ENTITY_TYPE::ENEMY_GIGA:

				factory->CreateGigamingoPage(&dataPageVector, this);
				GetGigamingoValue();
				state = DATA_PAGE_ENUM::FOCUSED_GIGAMINGO;
				break;

			case ENTITY_TYPE::BLDG_TURRET:

				factory->CreateTurretPage(&dataPageVector, this);
				GetTurretValue();
				state = DATA_PAGE_ENUM::FOCUSED_TURRET;
				break;

			case ENTITY_TYPE::BLDG_UPGRADE_CENTER:

				factory->CreateUpgradeCenterPage(&dataPageVector, this);
				GetUpgradeCenterValue();
				state = DATA_PAGE_ENUM::FOCUSED_UPGRADE_CENTER;
				break;

			case ENTITY_TYPE::BLDG_BASE:

				if (focus->GetAlignment() == ENTITY_ALIGNEMENT::PLAYER)
				{
					factory->CreateBasePage(&dataPageVector, this);
				}
				else
				{
					factory->CreateNonPlayerBasePage(&dataPageVector, this);
				}
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

		case DATA_PAGE_ENUM::FOCUSED_BARRICADE:
			CheckBarricadeValues();
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

		case DATA_PAGE_ENUM::FOCUSED_ROBOTTO:
			CheckHeroesValues();
			break;

		case DATA_PAGE_ENUM::FOCUSED_TURRET:
			CheckTurretValues();
			break;

		case DATA_PAGE_ENUM::FOCUSED_UPGRADE_CENTER:
			CheckUpgradeCenterValues();
			break;

		case DATA_PAGE_ENUM::FOCUSED_WANAMINGO:
			CheckWanamingoValues();
			break;

		case DATA_PAGE_ENUM::FOCUSED_SPEEDOMINGO:
			CheckSpeedomingoValues();
			break;

		case DATA_PAGE_ENUM::FOCUSED_SNIPERMINGO:
			CheckSnipermingoValues();
			break;

		case DATA_PAGE_ENUM::FOCUSED_GIGAMINGO:
			CheckGigamingoValues();
			break;

		case DATA_PAGE_ENUM::FOCUSED_UNKNOWN:
			break;
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


bool DataPages::CheckData(float previous, float current)
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

	if (CheckData(attackDamage, focus->GetAttackDamage()))
	{
		if (CheckData(attackSpeed, focus->GetAttackSpeed()))
		{
			if (CheckData(range, focus->GetAttackRange()))
			{
				if (CheckData(hpRecovery, focus->GetRecoveryHitPointsRate()))
				{
					if (CheckData(xpToNextLevel, focus->GetExpToLevelUp()))
					{
						if (CheckData(lifeMax, focus->GetMaxHP()))
						{
							if (CheckData(energyMax, focus->GetMaxEnergyPoints()))
							{
								if (CheckData(heroSkillPoints, focus->GetHeroSkillPoints()))
								{
									if (CheckData(skillResource, app->player->GetResourcesSkill()))
									{
										AdjustHealthBars(focus->GetCurrentHP(), focus->GetMaxHP());
										AdjustManaBars(focus->GetEnergyPoints(), focus->GetMaxEnergyPoints());
										check = true;
									}
								}
							}
						}
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
					if (CheckData(lifeMax, focus->GetMaxHP()))
					{
						AdjustHealthBars(focus->GetCurrentHP(), focus->GetMaxHP());
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


void DataPages::CheckSpeedomingoValues()
{
	bool check = false;

	NightEnemy* focus = (NightEnemy*)focusEntity;

	if (CheckData(attackDamage, focus->GetAD()))
	{
		if (CheckData(attackSpeed, focus->GetAS()))
		{
			if (CheckData(hpRecovery, focus->GetRecov()))
			{
				if (CheckData(vision, focus->GetVision()))
				{
					if (CheckData(lifeMax, focus->GetMaxHP()))
					{
						AdjustHealthBars(focus->GetCurrentHP(), focus->GetMaxHP());
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


void DataPages::CheckSnipermingoValues()
{
	bool check = false;

	RangedEnemy* focus = (RangedEnemy*)focusEntity;

	if (CheckData(attackDamage, focus->GetAD()))
	{
		if (CheckData(attackSpeed, focus->GetAS()))
		{
			if (CheckData(hpRecovery, focus->GetRecov()))
			{
				if (CheckData(vision, focus->GetVision()))
				{
					if (CheckData(lifeMax, focus->GetMaxHP()))
					{
						AdjustHealthBars(focus->GetCurrentHP(), focus->GetMaxHP());
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


void DataPages::CheckGigamingoValues()
{
	bool check = false;

	GigaEnemy* focus = (GigaEnemy*)focusEntity;

	if (CheckData(attackDamage, focus->GetAD()))
	{
		if (CheckData(attackSpeed, focus->GetAS()))
		{
			if (CheckData(hpRecovery, focus->GetRecov()))
			{
				if (CheckData(vision, focus->GetVision()))
				{
					if (CheckData(lifeMax, focus->GetMaxHP()))
					{
						AdjustHealthBars(focus->GetCurrentHP(), focus->GetMaxHP());
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


void DataPages::CheckBarricadeValues()
{
	Barricade* focus = (Barricade*)focusEntity;

	AdjustHealthBars(focus->GetCurrentHP(), focus->GetMaxHP());

	focus = nullptr;
}


void DataPages::CheckBaseValues()
{
	bool check = false;

	Base* focus = (Base*)focusEntity;

	if (CheckData(resources, focus->GetRsrc()))
	{
		if (CheckData((int)alignment, (int)focus->GetAlignment()))
		{
			if (CheckData(lifeMax, focus->GetMaxHP()))
			{
				AdjustHealthBars(focus->GetCurrentHP(), focus->GetMaxHP());
				check = true;
			}
		}
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
					if (CheckData(lifeMax, focus->GetMaxHP()))
					{
						AdjustHealthBars(focus->GetCurrentHP(), focus->GetMaxHP());
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


void DataPages::CheckUpgradeCenterValues()
{
	bool check = false;

	UpgradeCenter* focus = (UpgradeCenter*)focusEntity;

	if (CheckData((int)alignment, (int)focus->GetAlignment()))
	{
		if (CheckData(lifeMax, focus->GetMaxHP()))
		{
			AdjustHealthBars(focus->GetCurrentHP(), focus->GetMaxHP());
			check = true;
		}
	}

	if (check == false)
	{
		DeleteCurrentData();
	}

	focus = nullptr;
}

void DataPages::GetHeroValue()
{
	Hero* focus = (Hero*)app->player->GetFocusedEntity();

	attackDamage = focus->GetAttackDamage();
	attackSpeed = focus->GetAttackSpeed();
	range = focus->GetAttackRange();
	hpRecovery = focus->GetRecoveryHitPointsRate();
	xpToNextLevel = focus->GetExpToLevelUp();
	lifeMax = focus->GetMaxHP();
	energyMax = focus->GetMaxEnergyPoints();
	heroSkillPoints = focus->GetHeroSkillPoints();
	skillResource = app->player->GetResourcesSkill();

	GetHealthBarValues();
}


void DataPages::GetWanamingoValue()
{
	Enemy* focus = (Enemy*)app->player->GetFocusedEntity();

	attackDamage = focus->GetAD();
	attackSpeed = focus->GetAS();
	vision = focus->GetVision();
	hpRecovery = focus->GetRecov();
	lifeMax = focus->GetMaxHP();

	GetHealthBarValues();
}


void DataPages::GetSpeedomingoValue()
{
	NightEnemy* focus = (NightEnemy*)app->player->GetFocusedEntity();

	attackDamage = focus->GetAD();
	attackSpeed = focus->GetAS();
	vision = focus->GetVision();
	hpRecovery = focus->GetRecov();
	lifeMax = focus->GetMaxHP();

	GetHealthBarValues();
}


void DataPages::GetSnipermingoValue()
{
	RangedEnemy* focus = (RangedEnemy*)app->player->GetFocusedEntity();

	attackDamage = focus->GetAD();
	attackSpeed = focus->GetAS();
	vision = focus->GetVision();
	hpRecovery = focus->GetRecov();
	lifeMax = focus->GetMaxHP();

	GetHealthBarValues();
}


void DataPages::GetGigamingoValue()
{
	GigaEnemy* focus = (GigaEnemy*)app->player->GetFocusedEntity();

	attackDamage = focus->GetAD();
	attackSpeed = focus->GetAS();
	vision = focus->GetVision();
	hpRecovery = focus->GetRecov();
	lifeMax = focus->GetMaxHP();

	GetHealthBarValues();
}


void DataPages::GetBaseValue()
{
	Base* focus = (Base*)app->player->GetFocusedEntity();

	resources = focus->GetRsrc();
	alignment = focus->GetAlignment();
	lifeMax = focus->GetMaxHP();

	GetHealthBarValues();
}


void DataPages::GetTurretValue()
{
	Turret* focus = (Turret*)app->player->GetFocusedEntity();

	level = focus->GetLvl();
	attackDamage = focus->GetAD();
	attackSpeed = focus->GetAS();
	range = focus->GetRng();
	lifeMax = focus->GetMaxHP();

	GetHealthBarValues();
}


void DataPages::GetUpgradeCenterValue()
{
	UpgradeCenter* focus = (UpgradeCenter*)app->player->GetFocusedEntity();

	alignment = focus->GetAlignment();
	lifeMax = focus->GetMaxHP();

	GetHealthBarValues();
}


void DataPages::GetBarricadeValue()
{
	Barricade* focus = (Barricade*)app->player->GetFocusedEntity();

	lifeMax = focus->GetMaxHP();
	
	GetHealthBarValues();
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

	life = -1;
	lifeMax = -1;
	energy = -1;
	energyMax = -1;
	resources = -1;
	level = -1;
	attackDamage = -1;
	attackSpeed = -1;
	range = -1;
	vision = -1;
	hpRecovery = -1;
	xpToNextLevel = -1;
	heroSkillPoints = -1;
	skillResource = -1;
	alignment = ENTITY_ALIGNEMENT::UNKNOWN;

	healthRect = nullptr;
	energyRect = nullptr;
	focusEntity = nullptr;
	state = DATA_PAGE_ENUM::FOCUSED_NONE;
}


bool DataPages::OnAbove()
{
	bool ret = false;

	int elementsNumber = dataPageVector.size();

	for (int i = elementsNumber - 1; i >= 0 && ret == false; i--)
	{
		if (dataPageVector[i]->OnAbove() == true)
		{
			i--;

			for (i; i >= 0; i--)
			{
				dataPageVector[i]->UnFocus();
			}

			ret = true;
		}
	}

	return ret;
}


UI* DataPages::SearchFocus()
{
	int numElem = dataPageVector.size();
	UI* focusUI;

	for (int i = numElem - 1; i >= 0; i--)
	{
		focusUI = dataPageVector[i]->SearchFocus();

		if (focusUI != nullptr)
		{
			return focusUI;
		}
	}

	return nullptr;
}


void DataPages::UnFocus()
{
	int elementsNumber = dataPageVector.size();

	for (int i = 0; i < elementsNumber; i++)
	{
		dataPageVector[i]->UnFocus();
	}
}


void DataPages::AdjustHealthBars(int newValue, int maxValue)
{
	if (newValue != life && newValue > 0)
	{
		life = newValue;

		healthRect->w = life * originalBarsWidth / maxValue;

		if (healthRect->w == 0)
		{
			healthRect->w = 1;
		}
	}
}


void DataPages::AdjustManaBars(int newValue, int maxValue)
{
	if (newValue != energy && newValue > 0)
	{
		energy = newValue;

		energyRect->w = energy * originalBarsWidth / maxValue;

		if (energyRect->w == 0)
		{
			energyRect->w = 1;
		}
	}
}


void DataPages::GetHealthBarValues()
{
	SDL_Rect rect = factory->GetGreenHealthBar();
	SDL_Rect rect2 = factory->GetBlueHealthBar();

	int numElem = dataPageVector.size();

	for (int i = 0; i < numElem; i++)
	{
		if (dataPageVector[i]->rect.x == rect.x && dataPageVector[i]->rect.y == rect.y
			&& dataPageVector[i]->rect.w == rect.w && dataPageVector[i]->rect.h == rect.h)
		{
			healthRect = &dataPageVector[i]->rect;
		}
		else if (dataPageVector[i]->rect.x == rect2.x && dataPageVector[i]->rect.y == rect2.y
			&& dataPageVector[i]->rect.w == rect2.w && dataPageVector[i]->rect.h == rect2.h)
		{
			energyRect = &dataPageVector[i]->rect;
		}
	}
}

