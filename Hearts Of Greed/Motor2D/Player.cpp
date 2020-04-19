#include "App.h"

#include "Player.h"

#include "Hero.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "EntityManager.h"
#include "Brofiler/Brofiler/Brofiler.h"
#include "Map.h"
#include "EventManager.h"
#include "Minimap.h"
#include "Base.h"
#include "UIManager.h"

ModulePlayer::ModulePlayer() :

	Module(),

	focusedEntity(nullptr),

	resources(0),
	selectRect{ 0,0,0,0 },

	selectUnits(false),
	entityComand(false),
	entityInteraction(false),
	buildMode(false),
	skill1(false),
	skill2(false),
	skill3(false),
	prepareSkill(false),
	doingAction(false),
	hasClicked(false),
	constrAreaInfo(nullptr),

	doSkill(false),
	buildingPrevPosition{INT_MIN,INT_MIN},
	baseDrawCenter{ FLT_MIN, FLT_MIN },
	turretCost(0),

	buildingToBuild(ENTITY_TYPE::UNKNOWN)

{
	name.create("player");
}


ModulePlayer::~ModulePlayer()
{}


// Called before render is available
bool ModulePlayer::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("Player Awake", Profiler::Color::DarkCyan);

	turretCost = 95;
	return true;
}


// Called before the first frame
bool ModulePlayer::Start()
{
	//register the module as a listener to the following events
	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_COMMAND, this);
	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_INTERACTION, this);
	app->eventManager->EventRegister(EVENT_ENUM::SELECT_UNITS, this);
	app->eventManager->EventRegister(EVENT_ENUM::STOP_SELECTING_UNITS, this);

	app->eventManager->EventRegister(EVENT_ENUM::HERO_CHANGE_FOCUS, this);

	app->eventManager->EventRegister(EVENT_ENUM::SKILL1, this);
	app->eventManager->EventRegister(EVENT_ENUM::SKILL2, this);
	app->eventManager->EventRegister(EVENT_ENUM::SKILL3, this);

	app->eventManager->EventRegister(EVENT_ENUM::GIVE_RESOURCES, this);

	app->eventManager->EventRegister(EVENT_ENUM::TURRET_CONSTRUCT, this);


	app->eventManager->EventRegister(EVENT_ENUM::EXIT_CONSTRUCTION_MODE, this);


	return true;
}


bool ModulePlayer::CleanUp()
{
	app->eventManager->EventUnRegister(EVENT_ENUM::ENTITY_COMMAND, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ENTITY_INTERACTION, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SELECT_UNITS, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::STOP_SELECTING_UNITS, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::SKILL1, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SKILL2, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SKILL3, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::HERO_CHANGE_FOCUS, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::GIVE_RESOURCES, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::TURRET_CONSTRUCT, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::EXIT_CONSTRUCTION_MODE, this);


	constrAreaInfo = nullptr;
	constrArea.clear();

	return true;
}


// Called each loop iteration
bool ModulePlayer::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Player Pre-Update", Profiler::Color::Blue);

	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_STATE::KEY_DOWN && buildMode == false) // For debug purposes
	{
		ActivateBuildMode(ENTITY_TYPE::BLDG_TURRET, nullptr);
	}

	else if (app->input->GetKey(SDL_SCANCODE_4) == KEY_STATE::KEY_DOWN && buildMode == true) // For debug purposes
	{
		DesactivateBuildMode();
	}

	if (app->input->GetKey(SDL_SCANCODE_5) == KEY_STATE::KEY_DOWN) // For debug purposes
	{
		resources += 1000;
	}



	CheckListener(this);

	HandleInput();

	return true;
}


// Called each loop iteration
bool ModulePlayer::Update(float dt)
{
	BROFILER_CATEGORY("Player Update", Profiler::Color::Blue);

	CheckListener(this);

	return true;
}

// Called each loop iteration
bool ModulePlayer::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Player Post-Update", Profiler::Color::Blue);

	CheckListener(this);

	if (buildMode == true)
	{
		if (constrAreaInfo != nullptr)
		{
			iMPoint center = app->map->WorldToMap(round(baseDrawCenter.x), round(baseDrawCenter.y));
			fMPoint wBuildPos = app->input->GetMousePosWorld();
			iMPoint mBuildPos = app->map->WorldToMap(wBuildPos.x, wBuildPos.y);

			if (center.InsideCircle(mBuildPos, constrAreaInfo->radius))
			{
				buildingPrevPosition = app->map->MapToWorld(mBuildPos.x, mBuildPos.y);
			}

			app->entityManager->PlayerBuildPreview(buildingPrevPosition.x, buildingPrevPosition.y, buildingToBuild);

			if (constrArea.size() > 0)
			{
				for (uint i = 0; i < constrArea.size(); i++)
				{
					iMPoint pos = app->map->MapToWorld(constrArea[i].x - 1, constrArea[i].y);
					app->render->Blit(app->entityManager->debugPathTexture, pos.x, pos.y, NULL, false, true, 100);
				}
			}
		}
		else
		{
			fMPoint mousePos = app->input->GetMousePosWorld();
			app->entityManager->PlayerBuildPreview(mousePos.x, mousePos.y, buildingToBuild);
		}
	}

	DrawSelectQuad();
	return true;
}


//Handles Player Input
bool ModulePlayer::HandleInput()
{

	if (buildMode == false)
	{
		if (prepareSkill == true || doSkill == true)
		{
			CommandSkill();
		}
		
		else if (entityComand)
		{
			entityComand = false;
			doingAction = false;
			RightClick();
		}

		else if (entityInteraction)
		{
			entityInteraction = false;
			doingAction = false;
			focusedEntity = nullptr;
			LeftClick();
		}
		else if (selectUnits && hasClicked)
		{
			Select();
		}
	}

	else
	{
		clickPosition= app->input->GetMousePosScreen();


		if (entityInteraction)
		{
			entityInteraction = false;
			doingAction = false;
			BuildClick();
		}
	}

	return true;
}


bool ModulePlayer::Click()
{
	hasClicked = true;

	clickPosition= app->input->GetMousePosScreen();

	clickPosition.x = (-app->render->currentCamX + clickPosition.x) / app->win->GetScale();
	clickPosition.y = (-app->render->currentCamY + clickPosition.y) / app->win->GetScale();

	return false;
}


void ModulePlayer::LeftClick()
{
	ENTITY_TYPE type;
	Click();

	focusedEntity = app->entityManager->CheckEntityOnClick(clickPosition);

	if (focusedEntity != nullptr)
	{
		type = focusedEntity->GetType();
		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED)
		{
			heroesVector.clear();
			heroesVector.push_back((Hero*)focusedEntity);
		}
		app->eventManager->GenerateEvent(EVENT_ENUM::ENTITY_ON_CLICK, EVENT_ENUM::NULL_EVENT);
	}
}


void ModulePlayer::RightClick()
{
	if (heroesVector.empty())
		return;

	bool enemyFound;

	Click();

	Entity* obj = app->entityManager->CheckEntityOnClick(clickPosition, false);
	
	int numHeroes = heroesVector.size();

	for (int i = 0; i < numHeroes; i++)
	{
		enemyFound = heroesVector[i]->LockOn(obj);

		heroesVector[i]->MoveTo(clickPosition.x, clickPosition.y, enemyFound);
	}

}


void ModulePlayer::Select()
{
	iMPoint mousePosition = app->input->GetMousePosScreen();

	int rectX;
	int rectY;
	int rectW;
	int rectH;

	mousePosition.x = (-app->render->currentCamX + mousePosition.x) / app->win->GetScale();
	mousePosition.y = (-app->render->currentCamY + mousePosition.y) / app->win->GetScale();

	if (mousePosition.x > clickPosition.x)
	{
		rectX = clickPosition.x;
	}
	else
	{
		rectX = mousePosition.x;
	}

	rectW = abs(mousePosition.x - clickPosition.x);

	if (mousePosition.y > clickPosition.y)
	{
		rectY = clickPosition.y;
	}
	else
	{
		rectY = mousePosition.y;
	}

	rectH = abs(mousePosition.y - clickPosition.y);

	selectRect = { rectX,rectY, rectW,rectH };

	if (rectW > 10 || rectH > 10)
	{
		app->entityManager->CheckHeroOnSelection(selectRect, &heroesVector);
	}
	


	if (heroesVector.empty() == false)
	{
		focusedEntity = heroesVector[0];
	}

}



void ModulePlayer::CommandSkill()
{
	hasClicked = false;

	if (heroesVector.empty() == true)
	{
		prepareSkill = false;
		skill1 = false;
		skill2 = false;
		skill3 = false;
		doSkill = false;

		return;
	}

	else
		if (heroesVector[0] == nullptr)
		{
			prepareSkill = false;
			skill1 = false;
			skill2 = false;
			skill3 = false;
			doSkill = false;

			return;
		}


	if (prepareSkill == true)
	{
		PrepareHeroSkills();
	}


	else if (doSkill == true)
	{
		DoHeroSkills();
	}

}


void ModulePlayer::PrepareHeroSkills()
{

	if (skill1 == true)
	{
		if (heroesVector[0]->skill1Charged == true && (heroesVector[0]->energyPoints >= heroesVector[0]->skill1Cost))
		{
			doSkill = heroesVector[0]->PrepareSkill1();
			prepareSkill = !doSkill;
		}

		else
		{
			prepareSkill = false;
			skill1 = false;
		}
	}

	else if (skill2 == true)
	{
		if (heroesVector[0]->skill2Charged == true)
		{
			doSkill = heroesVector[0]->PrepareSkill2();
			prepareSkill = !doSkill;
		}

		else
		{
			prepareSkill = false;
			skill2 = false;
		}
	}


	else if (skill3 == true)
	{
		if (heroesVector[0]->skill3Charged == true)
		{
			doSkill = heroesVector[0]->PrepareSkill3();
			prepareSkill = !doSkill;
		}

		else
		{
			prepareSkill = false;
			skill3 = false;
		}
	}
}


void ModulePlayer::DoHeroSkills()
{
	if (entityComand == true)
	{
		entityComand = false;
		heroesVector[0]->SkillCanceled();


		prepareSkill = false;
		skill1 = false;
		skill2 = false;
		skill3 = false;
		doSkill = false;
	}

	if (entityInteraction == true)
	{
		entityInteraction = false;

		if (skill1 == true)
		{
			if (heroesVector[0]->ActivateSkill1(app->input->GetMousePosWorld()) == true)
			{
				skill1 = false;
				doSkill = false;
				selectUnits = false;
			}
		}

		else if (skill2 == true)
		{
			if (heroesVector[0]->ActivateSkill2() == true)
			{
				skill2 = false;
				doSkill = false;
				selectUnits = false;
			}
		}

		else if (skill3 == true)
		{
			if (heroesVector[0]->ActivateSkill3() == true)
			{
				skill3 = false;
				doSkill = false;
				selectUnits = false;
			}
		}
	}

}


bool ModulePlayer::BuildClick()
{
	int x, y;

	x = 0;
	y = 0;

	if (buildingPrevPosition.x != INT_MIN)
	{
		x = buildingPrevPosition.x;
		y = buildingPrevPosition.y;
	}
	else
	{
		fMPoint mousePos = app->input->GetMousePosWorld();
		x = mousePos.x;
		y = mousePos.y;
	}

	SDL_Rect rect = app->entityManager->GetSample(buildingToBuild)->GetCollider()->rect;

	x -= rect.w * 0.5f;
	y -= rect.h;

	app->entityManager->AddEntity(buildingToBuild, x, y, ENTITY_ALIGNEMENT::PLAYER);
	SubstractBuildResources();
	DesactivateBuildMode();

	return true;
}


void ModulePlayer::SubstractBuildResources()
{
	switch (buildingToBuild)
	{
	case ENTITY_TYPE::BLDG_TURRET:
	{
		resources -= turretCost;
	}
	break;
	}

}


void ModulePlayer::ExecuteEvent(EVENT_ENUM eventId)
{
	iMPoint mouse=	app->input->GetMousePosScreen();

	switch (eventId)
	{
	case EVENT_ENUM::SELECT_UNITS:
		if (app->minimap->ClickingOnMinimap(mouse.x, mouse.y) == false && app->uiManager->MouseOnUI(mouse) == false)
		{
			selectUnits = true;
			doingAction = true;
		}
		break;

	case EVENT_ENUM::STOP_SELECTING_UNITS:
		selectUnits = false;
		doingAction = false;
		break;

	case EVENT_ENUM::ENTITY_COMMAND:
		if (app->minimap->ClickingOnMinimap(mouse.x, mouse.y) == false && app->uiManager->MouseOnUI(mouse) == false)
		{
			entityComand = true;
			doingAction = true;
		}
		break;

	case EVENT_ENUM::ENTITY_INTERACTION:
		if (app->minimap->ClickingOnMinimap(mouse.x, mouse.y) == false && app->uiManager->MouseOnUI(mouse) == false)
		{
			entityInteraction = true;
			doingAction = true;
		}
		break;

	case EVENT_ENUM::SKILL1:
	
		if (doSkill == false) 
		{
			skill1 = true;
			prepareSkill = true;
		}
		
		break;

	case EVENT_ENUM::SKILL2:

		if (doSkill == false)
		{
			skill2 = true;
			prepareSkill = true;
		}
		
		break;

	case EVENT_ENUM::SKILL3:
		if (doSkill == false)
		{
			skill3 = true;
			prepareSkill = true;
		}
		
		break;

	case EVENT_ENUM::HERO_CHANGE_FOCUS:
	{
		int numHeroes = heroesVector.size();

		if (numHeroes > 0 )
		{
			for (int i = 0; i < numHeroes; i++)
			{
				if (focusedEntity == heroesVector[i])
				{
					if (i == numHeroes - 1)
					{
						focusedEntity = heroesVector[0];
					}
					else
						focusedEntity = heroesVector[i];
					break;
				}
			}
		}
	}
		break;

	case EVENT_ENUM::GIVE_RESOURCES:
		resources += 1000;
		break;

	case EVENT_ENUM::TURRET_CONSTRUCT:
		if (resources >= turretCost)
		{
			ActivateBuildMode(ENTITY_TYPE::BLDG_TURRET, app->uiManager->lastShop);
		}
		break;

	case EVENT_ENUM::TURRET_PURCHASE:
		resources -= turretCost;
		break;

	case EVENT_ENUM::EXIT_CONSTRUCTION_MODE:
		if (buildMode)
		{
			DesactivateBuildMode();
		}
		break;
	}


}


void ModulePlayer::DrawSelectQuad()
{
	if (selectUnits)
	{
		app->render->DrawQuad(selectRect, 0, 200, 0, 50, false);
		app->render->DrawQuad(selectRect, 0, 200, 0, 100);
	}
	else
	{
		selectRect = { 0,0,0,0 };
	}
}


void ModulePlayer::AddResources(int gain)
{
	resources += gain;
}


bool ModulePlayer::UseResources(int cost)
{
	if (cost > resources)
		return false;
	else
	{
		resources -= cost;
		return true;
	}
}


bool ModulePlayer::ActivateBuildMode(ENTITY_TYPE building, Base* contrBase)
{
	
	constrAreaInfo = nullptr;
	constrArea.clear();
	
	if (buildMode == false || (building != ENTITY_TYPE::BLDG_TURRET && building != ENTITY_TYPE::BLDG_UPGRADE_CENTER && building != ENTITY_TYPE::BLDG_BARRICADE && building != ENTITY_TYPE::BUILDING))
	{
		buildMode = true;
		buildingToBuild = building;

		app->eventManager->GenerateEvent(EVENT_ENUM::HIDE_MENU, EVENT_ENUM::NULL_EVENT);

		if (contrBase != nullptr)
		{
			baseDrawCenter = contrBase->GetPosition() + contrBase->GetCenter();
			baseDrawCenter.y += app->map->data.tileHeight;

			iMPoint origin = app->map->WorldToMap(round(baseDrawCenter.x), round(baseDrawCenter.y));
			origin = app->map->MapToWorld(origin.x, origin.y);

			constrAreaInfo = app->entityManager->RequestArea(SKILL_ID::BASE_AREA, &constrArea, origin);
		}
		return true;
	}

	else
		return false;

}


void ModulePlayer::DesactivateBuildMode()
{
	buildMode = false;
	buildingToBuild = ENTITY_TYPE::UNKNOWN;
	app->eventManager->GenerateEvent(EVENT_ENUM::UNHIDE_MENU, EVENT_ENUM::NULL_EVENT);


	constrAreaInfo = nullptr;
	constrArea.clear();
	baseDrawCenter = { FLT_MIN,FLT_MIN };
	buildingPrevPosition = { INT_MIN,INT_MIN };
}


void ModulePlayer::RemoveHeroFromVector(Hero* hero)
{
	int numHeroes = heroesVector.size();

	for (int i = 0; i < numHeroes; i++)
	{
		if (heroesVector[i] == hero)
		{
			heroesVector.erase(heroesVector.begin() + i);
			return;
		}
	}
}


void ModulePlayer::CheckFocusedEntity(Entity* entity)
{
	if (focusedEntity == entity)
	{
		focusedEntity = nullptr;
	}
}

Entity* ModulePlayer::GetFocusedEntity()
{
	return focusedEntity;
}


iMPoint ModulePlayer::GetClickPosition()
{
	return clickPosition;
}

int ModulePlayer::GetResources() const
{
	return resources;
}

bool ModulePlayer::IsBuilding() const
{
	return buildMode;
}


int ModulePlayer::GetTurretCost() const
{
	return turretCost;
}

