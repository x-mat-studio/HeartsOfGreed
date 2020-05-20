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
#include "Base.h"
#include "ParticleSystem.h"
#include "Pathfinding.h"

ModulePlayer::ModulePlayer() :

	Module(),

	resources(0),
	resourcesSkill(0),
	resourcesBoost(0),
	selectRect{ 0,0,0,0 },

	selectUnits(false),
	entityComand(false),
	entityInteraction(false),

	focusedHero(0),

	buildMode(false),
	skill1(false),
	skill2(false),
	skill3(false),
	prepareSkill(false),
	doingAction(false),
	hasClicked(false),
	doSkill(false),
	UIMenuOn(false),

	constrAreaInfo(nullptr),
	baseInBuild(nullptr),
	focusedEntity(nullptr),

	buildingPrevPosition{ INT_MIN,INT_MIN },
	baseDrawCenter{ FLT_MIN, FLT_MIN },
	turretCost(0),
	buildAreaRadius(5),

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

	turretCost = 120;
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
	app->eventManager->EventRegister(EVENT_ENUM::LVL_UP_ALL, this);

	app->eventManager->EventRegister(EVENT_ENUM::SKILL1, this);
	app->eventManager->EventRegister(EVENT_ENUM::SKILL2, this);
	app->eventManager->EventRegister(EVENT_ENUM::SKILL3, this);

	app->eventManager->EventRegister(EVENT_ENUM::HERO_CHANGE_FOCUS, this);

	app->eventManager->EventRegister(EVENT_ENUM::GIVE_RESOURCES, this);
	app->eventManager->EventRegister(EVENT_ENUM::GIVE_RESOURCES_SKILL, this);
	app->eventManager->EventRegister(EVENT_ENUM::GIVE_RESOURCES_BOOST, this);

	app->eventManager->EventRegister(EVENT_ENUM::TURRET_CONSTRUCT, this);

	app->eventManager->EventRegister(EVENT_ENUM::EXIT_CONSTRUCTION_MODE, this);

	app->eventManager->EventRegister(EVENT_ENUM::FOCUS_HERO_GATHERER, this);
	app->eventManager->EventRegister(EVENT_ENUM::FOCUS_HERO_MELEE, this);
	app->eventManager->EventRegister(EVENT_ENUM::FOCUS_HERO_RANGED, this);
	app->eventManager->EventRegister(EVENT_ENUM::FOCUS_HERO_ROBO, this);

	resources = 0;
	resourcesSkill = 0;
	resourcesBoost = 0;

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
	app->eventManager->EventUnRegister(EVENT_ENUM::GIVE_RESOURCES_SKILL, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GIVE_RESOURCES_BOOST, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::TURRET_CONSTRUCT, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::EXIT_CONSTRUCTION_MODE, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::FOCUS_HERO_GATHERER, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::FOCUS_HERO_MELEE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::FOCUS_HERO_RANGED, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::FOCUS_HERO_ROBO, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::LVL_UP_ALL, this);

	constrAreaInfo = nullptr;
	constrArea.clear();

	heroesVector.clear();

	resources = 0;

	return true;
}


// Called each loop iteration
bool ModulePlayer::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Player Pre-Update", Profiler::Color::Blue);

	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_STATE::KEY_DOWN && buildMode == false) // For debug purposes
	{
		ActivateBuildMode(ENTITY_TYPE::BLDG_BARRICADE, nullptr);
	}

	else if (app->input->GetKey(SDL_SCANCODE_4) == KEY_STATE::KEY_DOWN && buildMode == true) // For debug purposes
	{
		DesactivateBuildMode();
	}
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_STATE::KEY_DOWN) // For debug purposes
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::LVL_UP_ALL,EVENT_ENUM::NULL_EVENT);
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

			if (center.InsideCircle(mBuildPos, buildAreaRadius) && app->pathfinding->IsWalkable(mBuildPos))
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
		if (UIMenuOn)
			return true;

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

		if (heroesVector.empty() == false && focusedHero < heroesVector.size())
		{
			focusedEntity = heroesVector[focusedHero];
		}
	}

	else
	{
		clickPosition = app->input->GetMousePosScreen();

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

	clickPosition = app->input->GetMousePosScreen();

	clickPosition.x = (-app->render->currentCamX + clickPosition.x) / app->win->GetScale();
	clickPosition.y = (-app->render->currentCamY + clickPosition.y) / app->win->GetScale();

	return false;
}


void ModulePlayer::LeftClick()
{
	ENTITY_TYPE type;
	Click();

	heroesVector.clear();

	focusedEntity = app->entityManager->CheckEntityOnClick(clickPosition);

	if (focusedEntity != nullptr)
	{
		type = focusedEntity->GetType();
		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED || type == ENTITY_TYPE::HERO_ROBO)
		{
			heroesVector.clear();
			heroesVector.push_back((Hero*)focusedEntity);
		}
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

		if (heroesVector[i]->MoveTo(clickPosition.x, clickPosition.y, enemyFound))
			app->audio->PlayFx(app->entityManager->moveHero, 0, 1);;

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

	if (rectW > 20 || rectH > 20)
	{
		app->entityManager->CheckHeroOnSelection(selectRect, &heroesVector);
	}



}


void ModulePlayer::CommandSkill()
{
	hasClicked = false;

	if (CheckFocusedHero() == false)
		return;



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
		if (heroesVector[focusedHero]->skill1Charged == true && (heroesVector[focusedHero]->GetEnergyPoints() >= heroesVector[focusedHero]->GetSkill1Cost()))
		{
			doSkill = heroesVector[focusedHero]->PrepareSkill1();
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
		if (heroesVector[focusedHero]->skill2Charged == true)
		{
			doSkill = heroesVector[focusedHero]->PrepareSkill2();
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
		if (heroesVector[focusedHero]->skill3Charged == true)
		{
			doSkill = heroesVector[focusedHero]->PrepareSkill3();
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
		heroesVector[focusedHero]->SkillCanceled();


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
			if (heroesVector[focusedHero]->ActivateSkill1(app->input->GetMousePosWorld()) == true)
			{
				skill1 = false;
				doSkill = false;
				selectUnits = false;
			}
		}

		else if (skill2 == true)
		{
			if (heroesVector[focusedHero]->ActivateSkill2() == true)
			{
				skill2 = false;
				doSkill = false;
				selectUnits = false;
			}
		}

		else if (skill3 == true)
		{
			if (heroesVector[focusedHero]->ActivateSkill3() == true)
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


	if (baseInBuild != nullptr)
	{
		if (buildingPrevPosition.x != INT_MIN)
		{
			switch (buildingToBuild)
			{
			case ENTITY_TYPE::BLDG_TURRET:

				baseInBuild->AddTurret((Turret*)app->entityManager->AddEntity(buildingToBuild, x, y, ENTITY_ALIGNEMENT::PLAYER));
				break;
			}
			SubstractBuildResources();
			DesactivateBuildMode();
		}
	}
	else
	{
		app->entityManager->AddEntity(buildingToBuild, x, y, ENTITY_ALIGNEMENT::PLAYER);
		DesactivateBuildMode();
	}



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
	iMPoint mouse = app->input->GetMousePosScreen();
	Hero* hero;
	int numHeroes;

	switch (eventId)
	{
	case EVENT_ENUM::SELECT_UNITS:
		if (app->minimap->ClickingOnMinimap(mouse.x, mouse.y) == false && app->uiManager->mouseOverUI == false)
		{
			selectUnits = true;
			doingAction = true;
			focusedHero = 0;
		}
		break;

	case EVENT_ENUM::STOP_SELECTING_UNITS:
		selectUnits = false;
		doingAction = false;
		focusedHero = 0;

		if (heroesVector.size() > 0)
		{
			int random = rand() % heroesVector.size();

			heroesVector[random]->PlayGenericNoise(100);
		}


		break;

	case EVENT_ENUM::ENTITY_COMMAND:
		if (app->minimap->ClickingOnMinimap(mouse.x, mouse.y) == false && app->uiManager->mouseOverUI == false)
		{
			entityComand = true;
			doingAction = true;
		}
		break;

	case EVENT_ENUM::ENTITY_INTERACTION:
		if (app->minimap->ClickingOnMinimap(mouse.x, mouse.y) == false && app->uiManager->mouseOverUI == false)
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
		if (prepareSkill == false && doSkill == false)
		{
			int numHeroes = heroesVector.size();

			if (numHeroes - 1 > focusedHero)
			{
				focusedHero++;
			}
			else
			{
				focusedHero = 0;
			}

		}
	}
	break;

	case EVENT_ENUM::GIVE_RESOURCES:
		resources += 1000;
		break;

	case EVENT_ENUM::GIVE_RESOURCES_SKILL:
		resourcesSkill += 1;
		break;

	case EVENT_ENUM::GIVE_RESOURCES_BOOST:
		resourcesBoost+= 300;
		break;

	case EVENT_ENUM::TURRET_CONSTRUCT:
		if (focusedEntity->GetType() == ENTITY_TYPE::BLDG_BASE)
		{
			Base* base = (Base*)focusedEntity;

			if (resources >= turretCost && base->TurretCapacityExceed())
			{
				ActivateBuildMode(ENTITY_TYPE::BLDG_TURRET, base);
			}
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


	case EVENT_ENUM::FOCUS_HERO_GATHERER:

		numHeroes = heroesVector.size();

		for (int i = 0; i < numHeroes; i++)
		{
			heroesVector[i]->selectedByPlayer = false;
		}

		heroesVector.clear();


		hero = (Hero*)app->entityManager->SearchEntity(ENTITY_TYPE::HERO_GATHERER);
		hero->selectedByPlayer = true;

		heroesVector.push_back(hero);
		break;


	case EVENT_ENUM::FOCUS_HERO_MELEE:

		numHeroes = heroesVector.size();

		for (int i = 0; i < numHeroes; i++)
		{
			heroesVector[i]->selectedByPlayer = false;
		}

		heroesVector.clear();

		hero = (Hero*)app->entityManager->SearchEntity(ENTITY_TYPE::HERO_MELEE);
		hero->selectedByPlayer = true;

		heroesVector.push_back(hero);
		break;


	case EVENT_ENUM::FOCUS_HERO_RANGED:

		numHeroes = heroesVector.size();

		for (int i = 0; i < numHeroes; i++)
		{
			heroesVector[i]->selectedByPlayer = false;
		}

		heroesVector.clear();

		hero = (Hero*)app->entityManager->SearchEntity(ENTITY_TYPE::HERO_RANGED);
		hero->selectedByPlayer = true;

		heroesVector.push_back(hero);
		break;


	case EVENT_ENUM::FOCUS_HERO_ROBO:

		numHeroes = heroesVector.size();

		for (int i = 0; i < numHeroes; i++)
		{
			heroesVector[i]->selectedByPlayer = false;
		}

		heroesVector.clear();

		hero = (Hero*)app->entityManager->SearchEntity(ENTITY_TYPE::HERO_ROBO);
		hero->selectedByPlayer = true;

		heroesVector.push_back(hero);
		break;
	

	case EVENT_ENUM::LVL_UP_ALL:

		app->audio->PlayFx(app->entityManager->lvlup, 0, -1);

		for (int aux = 0; aux < heroesVector.size(); aux++) {

			if (heroesVector[aux] != nullptr) {
			
				heroesVector[aux]->LevelUp();
			}
		
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

void ModulePlayer::AddResourcesSkill(int gain)
{
	resourcesSkill += gain;
}

void ModulePlayer::AddResourcesBoost(int gain)
{
	resourcesBoost += gain;
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

bool ModulePlayer::UseResourcesSkill(int cost)
{
	if (cost > resourcesSkill)
		return false;
	else
	{
		resourcesSkill -= cost;
		return true;
	}
}

bool ModulePlayer::UseResourcesBoost(int cost)
{
	if (cost > resourcesBoost)
		return false;
	else
	{
		resourcesBoost -= cost;
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
		baseInBuild = contrBase;

		app->eventManager->GenerateEvent(EVENT_ENUM::HIDE_MENU, EVENT_ENUM::NULL_EVENT);

		if (baseInBuild != nullptr)
		{
			baseDrawCenter = baseInBuild->GetPosition();

			iMPoint origin = app->map->WorldToMap(round(baseDrawCenter.x), round(baseDrawCenter.y));
			origin = app->map->MapToWorld(origin.x, origin.y);

			constrAreaInfo = app->entityManager->RequestAreaInfo(buildAreaRadius);

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

	baseInBuild = nullptr;
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


bool ModulePlayer::CheckFocusedHero()
{
	if (heroesVector.size() <= focusedHero)
	{
		if (heroesVector.empty())
		{
			prepareSkill = false;
			skill1 = false;
			skill2 = false;
			skill3 = false;
			doSkill = false;
			return false;
		}

		else
		{
			focusedHero = 0;
			return true;
		}

	}

	else
		if (heroesVector[focusedHero] == nullptr)
		{
			prepareSkill = false;
			skill1 = false;
			skill2 = false;
			skill3 = false;
			doSkill = false;

			return false;
		}
		else
			return true;
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

int ModulePlayer::GetResourcesSkill() const
{
	return resourcesSkill;
}

int ModulePlayer::GetResourcesBoost() const
{
	return resourcesBoost;
}

bool ModulePlayer::IsBuilding() const
{
	return buildMode;
}


int ModulePlayer::GetTurretCost() const
{
	return turretCost;
}

bool ModulePlayer::SetMenuState(bool menuState)
{
	if (menuState != UIMenuOn)
	{
		UIMenuOn = !UIMenuOn;
	}

	return UIMenuOn;
}


bool ModulePlayer::Load(pugi::xml_node& data)
{
	pugi::xml_node iterator = data.first_child();

	resources = iterator.attribute("cristals").as_int();
	resourcesSkill = iterator.attribute("skillCoin").as_int();
	resourcesBoost = iterator.attribute("enemyCoin").as_int();

	return true;
}


bool ModulePlayer::Save(pugi::xml_node& data) const
{
	pugi::xml_node iterator = data.append_child("resources");

	iterator.append_attribute("cristals") = resources;
	iterator.append_attribute("skillCoin") = resourcesSkill;
	iterator.append_attribute("enemyCoin") = resourcesBoost;

	return true;
}