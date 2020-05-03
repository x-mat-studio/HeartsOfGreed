#include "DataPages.h"
#include "Player.h"
#include "UIManager.h"

DataPages::DataPages(UI* parent, Entity* entity) : UI({ 0, 0 }, parent, UI_TYPE::DATA_PAGES, { 0, 0, 0, 0 }, true, false, nullptr),
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

	switch (state)
	{
	case DATA_PAGE_ENUM::FOCUSED_NONE:
		if (focus != nullptr)
		{
			switch (focus->GetType())
			{

			case ENTITY_TYPE::HERO_GATHERER:
				CreateGathererPage();
				break;
			case ENTITY_TYPE::HERO_MELEE:
				CreateMeleePage();
				break;
			case ENTITY_TYPE::HERO_RANGED:
				CreateRangedPage();
				break;
			case ENTITY_TYPE::ENEMY:
				CreateWanamingoPage();
				break;
			case ENTITY_TYPE::BLDG_TURRET:
				CreateTurretPage();
				break;
			case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
				CreateUpgradeCenterPage();
				break;
			case ENTITY_TYPE::BLDG_BASE:
				CreateBasePage();
				break;
			case ENTITY_TYPE::BLDG_BARRICADE:
				CreateBarricadePage();
				break;
			default:
				state = DATA_PAGE_ENUM::FOCUSED_UNKNOWN;
				break;
			}
		}
		break;
	case DATA_PAGE_ENUM::FOCUSED_BASE:
		CheckBaseValues();
		break;
	case DATA_PAGE_ENUM::FOCUSED_GATHERER:
		CheckGathererValues();
		break;
	case DATA_PAGE_ENUM::FOCUSED_MELEE:
		CheckMeleeValues();
		break;
	case DATA_PAGE_ENUM::FOCUSED_RANGED:
		CheckRangedValues();
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
	case DATA_PAGE_ENUM::FOCUSED_UNKNOWN:
	default:
		break;
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


void DataPages::ChangeTexture()
{}


void DataPages::CreateGathererPage()
{
	

	state = DATA_PAGE_ENUM::FOCUSED_GATHERER;
}


void DataPages::CreateMeleePage()
{


	state = DATA_PAGE_ENUM::FOCUSED_MELEE;
}


void DataPages::CreateRangedPage()
{


	state = DATA_PAGE_ENUM::FOCUSED_RANGED;
}


void DataPages::CreateWanamingoPage()
{


	state = DATA_PAGE_ENUM::FOCUSED_WANAMINGO;
}


void DataPages::CreateBasePage()
{


	state = DATA_PAGE_ENUM::FOCUSED_BASE;
}


void DataPages::CreateTurretPage()
{


	state = DATA_PAGE_ENUM::FOCUSED_TURRET;
}


void DataPages::CreateUpgradeCenterPage()
{


	state = DATA_PAGE_ENUM::FOCUSED_UPGRADE_CENTER;
}


void DataPages::CreateBarricadePage()
{


	state = DATA_PAGE_ENUM::FOCUSED_BARRICADE;
}


void DataPages::CheckGathererValues()
{}


void DataPages::CheckMeleeValues()
{}


void DataPages::CheckRangedValues()
{}


void DataPages::CheckWanamingoValues()
{}


void DataPages::CheckBarricadeValues()
{}


void DataPages::CheckBaseValues()
{}


void DataPages::CheckTurretValues()
{}


void DataPages::CheckUpgradeCenterValues()
{}


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

	focusEntity = nullptr;
	state = DATA_PAGE_ENUM::FOCUSED_NONE;
}
