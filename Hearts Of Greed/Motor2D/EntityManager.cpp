#include "App.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "Textures.h"
#include "Entity.h"
#include "Map.h"
#include "Collision.h"
#include "AI.h"
#include "FoWManager.h"
#include "Pathfinding.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Minimap.h"

#include "DynamicEntity.h"
#include "GathererHero.h"
#include "MeleeHero.h"
#include "RangedHero.h"
#include "Enemy.h"

#include "Spawner.h"

#include "Building.h"
#include "Base.h"
#include "Turret.h"

#include "Brofiler/Brofiler/Brofiler.h"


ModuleEntityManager::ModuleEntityManager()
{
	name.create("entityManager");
}


// Destructor
ModuleEntityManager::~ModuleEntityManager()
{}


// Called before render is available
bool ModuleEntityManager::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("Entity Manager Awake", Profiler::Color::DarkCyan);

	bool ret = true;

	//Vfx load -----------------------------------
	P2SString filename = config.child("load").attribute("docnamevfx").as_string();
	pugi::xml_document vfxDoc;
	vfxDoc.load_file(filename.GetString());
	pugi::xml_node explosion = vfxDoc.child("Vfx");

	Animation vfxExplosion = vfxExplosion.PushAnimation(explosion, "explosion");


	// Sample Hero Melee---------------------
	fMPoint pos;
	pos.create(100, 600);

	filename = config.child("load").attribute("docnameSuitman").as_string();
	pugi::xml_document suitmandoc;
	suitmandoc.load_file(filename.GetString());
	pugi::xml_node suitman = suitmandoc.child("suitman");

	Animation walkLeft = walkLeft.PushAnimation(suitman, "walk_left");
	Animation walkLeftUp = walkLeftUp.PushAnimation(suitman, "walk_left_up");
	Animation walkLeftDown = walkLeftDown.PushAnimation(suitman, "walk_left_down");
	Animation walkRightUp = walkRightUp.PushAnimation(suitman, "walk_right_up");
	Animation walkRightDown = walkRightDown.PushAnimation(suitman, "walk_right_down");
	Animation walkRight = walkRight.PushAnimation(suitman, "walk_right");

	Animation idleRight = idleRight.PushAnimation(suitman, "idle_right");
	Animation idleRightUp = idleRightUp.PushAnimation(suitman, "idle_right_up");
	Animation idleRightDown = idleRightDown.PushAnimation(suitman, "idle_right_down");
	Animation idleLeft = idleLeft.PushAnimation(suitman, "idle_left");
	Animation idleLeftUp = idleLeftUp.PushAnimation(suitman, "idle_left_up");
	Animation idleLeftDown = idleLeftDown.PushAnimation(suitman, "idle_left_down");

	Animation punchRight = punchRight.PushAnimation(suitman, "punch_right");
	Animation punchRightUp = punchRightUp.PushAnimation(suitman, "punch_right_up");
	Animation punchRightDown = punchRightDown.PushAnimation(suitman, "punch_right_down");
	Animation punchLeft = punchLeft.PushAnimation(suitman, "punch_left");
	Animation punchLeftUp = punchLeftUp.PushAnimation(suitman, "punch_left_up");
	Animation punchLeftDown = punchLeftDown.PushAnimation(suitman, "punch_left_down");
	
	Animation skill1Right = skill1Right.PushAnimation(suitman, "skill_1_right");
	Animation skill1RightUp = skill1RightUp.PushAnimation(suitman, "skill_1_right_up");
	Animation skill1RightDown = skill1RightDown.PushAnimation(suitman, "skill_1_right_down");
	Animation skill1Left = skill1Left.PushAnimation(suitman, "skill_1_left");
	Animation skill1LeftUp = skill1LeftUp.PushAnimation(suitman, "skill_1_left_up");
	Animation skill1LeftDown = skill1LeftDown.PushAnimation(suitman, "skill_1_left_down");

	// Hero collider
	Collider* collider = new Collider({ 0,0,30,65 }, COLLIDER_HERO, this);
	sampleGatherer = new GathererHero(fMPoint{ pos.x, pos.y }, collider, walkLeft, walkLeftUp,
		walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightUp, idleRightDown, idleLeft,
		idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp, punchRightDown, punchRight, skill1Right,
		skill1RightUp, skill1RightDown, skill1Left, skill1LeftUp, skill1LeftDown,
		1, 100, 100, 1, 40, 1, 20, 1, 45, 100, 5, 2.65f, 20.f, 20.f, 15.f, 15.f, 15.f,
		50, SKILL_ID::GATHERER_SKILL1, SKILL_TYPE::AREA_OF_EFFECT, ENTITY_ALIGNEMENT::ENEMY, vfxExplosion);

		// Sample Enemy---------------------
	filename = config.child("load").attribute("docnameWanamingo").as_string();
	pugi::xml_document wanamingodoc;
	wanamingodoc.load_file(filename.GetString());
	pugi::xml_node wanamingo = wanamingodoc.child("wanamingo");

	Animation enemyWalkLeft = enemyWalkLeft.PushAnimation(wanamingo, "wanamingoLeftWalk"); // looks good
	Animation enemyWalkLeftUp = enemyWalkLeftUp.PushAnimation(wanamingo, "wanamingoUpLeftWalk");// looks good
	Animation enemyWalkLeftDown = enemyWalkLeftDown.PushAnimation(wanamingo, "wanamingoDownLeftWalk"); // last frame teleports to the left
	Animation enemyWalkRightUp = enemyWalkRightUp.PushAnimation(wanamingo, "wanamingoUpRightWalk"); // looks good
	Animation enemyWalkRightDown = enemyWalkRightDown.PushAnimation(wanamingo, "wanamingoDownRightWalk"); // looks good
	Animation enemyWalkRight = enemyWalkRight.PushAnimation(wanamingo, "wanamingoRightWalk");// looks good

	Animation enemyIdleRight = enemyIdleRight.PushAnimation(wanamingo, "wanamingoRightIdle"); //goes up then bumps right
	Animation enemyIdleRightUp = enemyIdleRightUp.PushAnimation(wanamingo, "wanamingoUpRightIdle"); //bumps left
	Animation enemyIdleRightDown = enemyIdleRightDown.PushAnimation(wanamingo, "wanamingoDownRightIdle"); //bumps right
	Animation enemyIdleLeft = enemyIdleLeft.PushAnimation(wanamingo, "wanamingoLeftIdle"); //bumps left
	Animation enemyIdleLeftUp = enemyIdleLeftUp.PushAnimation(wanamingo, "wanamingoUpLeftIdle"); //bumps right
	Animation enemyIdleLeftDown = enemyIdleLeftDown.PushAnimation(wanamingo, "wanamingoDownLeftIdle"); //bumps right

	Animation enemyPunchRight = enemyPunchRight.PushAnimation(wanamingo, "wanamingoRightPunch"); //looks good
	Animation enemyPunchRightUp = enemyPunchRightUp.PushAnimation(wanamingo, "wanamingoUpRightPunch"); //jesus christ 
	Animation enemyPunchRightDown = enemyPunchRightDown.PushAnimation(wanamingo, "wanamingoDownRightPunch"); //goes back and forth
	Animation enemyPunchLeft = enemyPunchLeft.PushAnimation(wanamingo, "wanamingoLeftPunch"); //It should bump to the other side!
	Animation enemyPunchLeftUp = enemyPunchLeftUp.PushAnimation(wanamingo, "wanamingoUpLeftPunch"); //jesus christ 
	Animation enemyPunchLeftDown = enemyPunchLeftDown.PushAnimation(wanamingo, "wanamingoDownLeftPunch"); //jesus christ 


	// Sample Crazy Turret Melee---------------------
	filename = config.child("load").attribute("docnameTurret").as_string();
	pugi::xml_document turretdoc;
	turretdoc.load_file(filename.GetString());
	pugi::xml_node turret = turretdoc.child("turret");

	Animation turretCrazyIdle = turretCrazyIdle.PushAnimation(turret, "crazyIdle"); // looks good


	//Enemy collider and spawner
	Collider* enemyCollider = new Collider({ 0,0,50,50 }, COLLIDER_ENEMY, this);

	sampleEnemy = new Enemy(fMPoint{ 150, 250 }, ENTITY_TYPE::ENEMY, enemyCollider, enemyWalkLeft, enemyWalkLeftUp,
	enemyWalkLeftDown, enemyWalkRightUp, enemyWalkRightDown, enemyWalkRight, enemyIdleRight, enemyIdleRightUp, enemyIdleRightDown, enemyIdleLeft,
	enemyIdleLeftUp, enemyIdleLeftDown, enemyPunchLeft, enemyPunchLeftUp, enemyPunchLeftDown, enemyPunchRightUp, enemyPunchRightDown, enemyPunchRight,
	5000, 5000, 0, 250, 1, 1, 35, 100, 50);
	sampleSpawner = new Spawner(fMPoint{ 150, 250 }, ENTITY_TYPE::ENEMY, sampleEnemy->hitPointsMax, sampleEnemy->hitPointsCurrent);

	//Test building
	Collider* buildingCollider = new Collider({ -150,130,350,280 }, COLLIDER_VISIBILITY, this);
	testBuilding = new Building(fMPoint{ 0,0 }, 100, 100, 100, 100, 100, 100, buildingCollider);

	// Test Turret
	Collider* turretCollider = new Collider({ 150,130,350,280 }, COLLIDER_VISIBILITY, this);
	testTurret = new Turret(1, 2, 3, 4, fMPoint{ 0, 0 }, turretCollider, turretCrazyIdle);

	//Template base
	Collider* baseAlarmCollider = new Collider({ 0, 0, 800, 800 }, COLLIDER_BASE_ALERT, app->ai);
	sampleBase = new Base(fMPoint{ 0, 0 }, buildingCollider, 5, 5, nullptr, baseAlarmCollider, 5, 3, 500, 500, 20, 100);


	//Generate Areas------------------------------------
	skillArea gathererSkill1AreaRange;
	gathererSkill1AreaRange.form = AREA_TYPE::CIRCLE;
	BuildArea(&gathererSkill1AreaRange, 0, 0, 7);
	skillAreas.insert({ SKILL_ID::GATHERER_SKILL1, gathererSkill1AreaRange });

	skillArea gathererSkill1AreaExplosion;
	gathererSkill1AreaRange.form = AREA_TYPE::CIRCLE;
	BuildArea(&gathererSkill1AreaRange, 0, 0, 2);
	skillAreas.insert({ SKILL_ID::GATHERER_SKILL1_MOUSE, gathererSkill1AreaRange });

	skillArea meleeSkill1Area;
	meleeSkill1Area.form = AREA_TYPE::CIRCLE;
	BuildArea(&meleeSkill1Area, 0, 0, 2);
	skillAreas.insert({ SKILL_ID::MELEE_SKILL1, meleeSkill1Area });

	return ret;
}



bool ModuleEntityManager::Start()
{
	bool ret = true;

	suitManTexture = app->tex->Load("spritesheets/characters/suitmale.png");
	armorMaleTexture = app->tex->Load("spritesheets/characters/armormale.png");
	combatFemaleTexture = app->tex->Load("spritesheets/characters/combatfemale.png");

	enemyTexture = app->tex->Load("spritesheets/Enemies/WanamingoAlien.png");

	buildingTexture = app->tex->Load("maps/base03.png");
	base1Texture = app->tex->Load("maps/base01.png");
	base2Texture = app->tex->Load("maps/base02.png");

	//turretTexture = nullptr;
	turretTexture = app->tex->Load("spritesheets/Structures/turretSpritesheet.png");

	debugPathTexture = app->tex->Load("maps/path.png");

	app->eventManager->EventRegister(EVENT_ENUM::DAY_START, this);
	app->eventManager->EventRegister(EVENT_ENUM::NIGHT_START, this);

	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_DEAD, this);

	app->eventManager->EventRegister(EVENT_ENUM::ACTIVATE_GODMODE_HEROES, this);
	app->eventManager->EventRegister(EVENT_ENUM::DESACTIVATE_GODMODE_HEROES, this);
	app->eventManager->EventRegister(EVENT_ENUM::KILL_ALL_ENEMIES, this);

	app->eventManager->EventRegister(EVENT_ENUM::SPAWN_BASE, this);
	app->eventManager->EventRegister(EVENT_ENUM::SPAWN_BUILDING, this);
	app->eventManager->EventRegister(EVENT_ENUM::SPAWN_ENEMY, this);
	app->eventManager->EventRegister(EVENT_ENUM::SPAWN_GATHERER_HERO, this);
	app->eventManager->EventRegister(EVENT_ENUM::SPAWN_MELEE_HERO, this);
	app->eventManager->EventRegister(EVENT_ENUM::SPAWN_RANGED_HERO, this);
	app->eventManager->EventRegister(EVENT_ENUM::SPAWN_TURRET, this);
	

	testBuilding->SetTexture(base1Texture);
	sampleBase->SetTexture(base2Texture);

	//Wanamingo Sfx----
	wanamingoRoar = app->audio->LoadFx("audio/sfx/Wanamingo/Roar.wav");
	wanamingoRoar2 = app->audio->LoadFx("audio/sfx/Wanamingo/Roar2.wav");
	wanamingoGetsHit = app->audio->LoadFx("audio/sfx/Wanamingo/Hit.wav");
	wanamingoDies = app->audio->LoadFx("audio/sfx/Wanamingo/Death.wav");
	wanamingoDies2 = app->audio->LoadFx("audio/sfx/Wanamingo/Death2.wav");

	//Suitman sfx-----
	suitmanGetsHit = app->audio->LoadFx("audio/sfx/Heroes/Suitman/GetHit.wav");
	suitmanGetsHit2 = app->audio->LoadFx("audio/sfx/Heroes/Suitman/GetsHit2.wav");
	suitmanGetsDeath = app->audio->LoadFx("audio/sfx/Heroes/Suitman/Death.wav");
	suitmanGetsDeath2 = app->audio->LoadFx("audio/sfx/Heroes/Suitman/Death2.wav");
	suitman1Skill = app->audio->LoadFx("audio/sfx/Heroes/Suitman/Skill1.wav");
	suitman1Skill2 = app->audio->LoadFx("audio/sfx/Heroes/Suitman/Skill1_2.wav");

	return ret;
}



bool ModuleEntityManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Entity Manager Pre-Update", Profiler::Color::Blue);

	CheckListener(this);

	CheckIfStarted();

	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		entityVector[i]->PreUpdate(dt);
	}

	return true;
}



void ModuleEntityManager::CheckIfStarted() {

	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		if (entityVector[i]->started == false) {

			switch (entityVector[i]->GetType())
			{
			case ENTITY_TYPE::PARTICLE:
				break;

			case ENTITY_TYPE::HERO_MELEE:
				entityVector[i]->Start(armorMaleTexture);
				app->eventManager->GenerateEvent(EVENT_ENUM::HERO_MELEE_CREATED, EVENT_ENUM::NULL_EVENT);
				break;

			case ENTITY_TYPE::HERO_RANGED:
				entityVector[i]->Start(combatFemaleTexture);
				app->eventManager->GenerateEvent(EVENT_ENUM::HERO_RANGED_CREATED, EVENT_ENUM::NULL_EVENT);
				break;

			case ENTITY_TYPE::HERO_GATHERER:
				entityVector[i]->Start(suitManTexture);
				app->eventManager->GenerateEvent(EVENT_ENUM::HERO_GATHERER_CREATED, EVENT_ENUM::NULL_EVENT);
				break;

			case ENTITY_TYPE::ENEMY:
				entityVector[i]->Start(enemyTexture);
				break;

			case ENTITY_TYPE::BUILDING:

				SDL_Texture* DecorTex;

				Building* bld;
				bld = (Building*)entityVector[i];
				//bld = static_cast<Building*>(entityVector[i]);		http://www.cplusplus.com/doc/tutorial/typecasting/

				switch (bld->GetDecor())
				{
				case BUILDING_DECOR::ST_01:
					DecorTex = base1Texture;
					break;
				case BUILDING_DECOR::ST_02:
					DecorTex = base2Texture;
					break;
				case BUILDING_DECOR::ST_03:
					DecorTex = buildingTexture;
					break;
				default:
					DecorTex = base1Texture;
					break;
				}

				entityVector[i]->Start(DecorTex);

				break;

			case ENTITY_TYPE::BLDG_TURRET:
				entityVector[i]->Start(turretTexture);
				break;

			case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
				break;

			case ENTITY_TYPE::BLDG_BASE:
				entityVector[i]->Start(base2Texture);
				break;

			case ENTITY_TYPE::BLDG_BARRICADE:
				break;

			default:
				entityVector[i]->started = true;
				break;
			}
		}
	}
}



bool ModuleEntityManager::Update(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::Blue);

	CheckListener(this);

	int numEntities = entityVector.size();


	for (int i = 0; i < numEntities; i++)
	{
		entityVector[i]->Update(dt);
	}

	return true;
}


// Called each loop iteration
bool ModuleEntityManager::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Entity Manager Update", Profiler::Color::Blue);

	int numEntities = entityVector.size();


	for (int i = 0; i < numEntities; i++)
	{
		entityVector[i]->PostUpdate(dt);
	}

	SpriteOrdering(dt);

	return true;
}


bool ModuleEntityManager::CleanUp()
{
	DeleteAllEntities();

	app->tex->UnLoad(suitManTexture);
	app->tex->UnLoad(armorMaleTexture);
	app->tex->UnLoad(combatFemaleTexture);
	app->tex->UnLoad(enemyTexture);

	app->tex->UnLoad(buildingTexture);
	app->tex->UnLoad(base1Texture);
	app->tex->UnLoad(base2Texture);

	app->tex->UnLoad(turretTexture);

	app->tex->UnLoad(debugPathTexture);

	suitManTexture = nullptr;
	armorMaleTexture = nullptr;
	combatFemaleTexture = nullptr;
	enemyTexture = nullptr;

	buildingTexture = nullptr;
	base1Texture = nullptr;
	base2Texture = nullptr;

	turretTexture = nullptr;

	debugPathTexture = nullptr;

	RELEASE(sampleGatherer);
	RELEASE(sampleEnemy);
	RELEASE(sampleSpawner);
	RELEASE(testBuilding);
	RELEASE(blueBuilding);
	RELEASE(sampleBase);

	sampleGatherer = nullptr;
	sampleEnemy = nullptr;
	sampleSpawner = nullptr;
	testBuilding = nullptr;
	blueBuilding = nullptr;
	sampleBase = nullptr;

	for (std::unordered_map<SKILL_ID, skillArea> ::iterator it = skillAreas.begin(); it != skillAreas.end(); it++)
	{
		delete it->second.area;
		it->second.area = nullptr;

	}
	skillAreas.clear();

	return true;
}

void ModuleEntityManager::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->thisEntity != nullptr)
	{
		c1->thisEntity->OnCollision(c2);
	}
}


//Add an entity
Entity* ModuleEntityManager::AddEntity(ENTITY_TYPE type, int x, int y, ENTITY_ALIGNEMENT alignement)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case ENTITY_TYPE::SPAWNER:
		ret = new Spawner({ (float)x,(float)y }, sampleSpawner);
		app->ai->PushSpawner((Spawner*)ret);
		break;
	case ENTITY_TYPE::PARTICLE:
		break;

	case ENTITY_TYPE::EMITER:
		break;

	case ENTITY_TYPE::PARTICLE_SYSTEM:
		break;

	case ENTITY_TYPE::HERO_MELEE:
		break;

	case ENTITY_TYPE::HERO_RANGED:
		break;

	case ENTITY_TYPE::HERO_GATHERER:
		ret = new GathererHero({ (float)x,(float)y }, sampleGatherer, ENTITY_ALIGNEMENT::PLAYER);
		ret->minimapIcon = app->minimap->CreateIcon(&ret->position, MINIMAP_ICONS::HERO);
		break;

	case ENTITY_TYPE::BUILDING:
		ret = new Building({ (float)x,(float)y }, testBuilding, alignement);
		break;

	case ENTITY_TYPE::BLDG_TURRET:
		ret = new Turret({ (float)x,(float)y }, testTurret, alignement);
		if (alignement == ENTITY_ALIGNEMENT::PLAYER)
		{
			ret->minimapIcon = app->minimap->CreateIcon(&ret->position, MINIMAP_ICONS::TURRET);
		}
		else if (alignement == ENTITY_ALIGNEMENT::ENEMY)
		{
			ret->minimapIcon = app->minimap->CreateIcon(&ret->position, MINIMAP_ICONS::TURRET); //TODO CHANGE THIS FOR ENEMY TURRET
		}

		break;

	case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
		break;

	case ENTITY_TYPE::BLDG_BASE:
		ret = new Base({ (float)x,(float)y }, sampleBase, alignement);
		if (alignement == ENTITY_ALIGNEMENT::PLAYER)
		{
			ret->minimapIcon = app->minimap->CreateIcon(&ret->position, MINIMAP_ICONS::BASE);
		}
		else if (alignement == ENTITY_ALIGNEMENT::ENEMY)
		{
			ret->minimapIcon = app->minimap->CreateIcon(&ret->position, MINIMAP_ICONS::BASE);
		}

		app->ai->PushBase((Base*)ret);
		break;

	case ENTITY_TYPE::BLDG_BARRICADE:
		break;

	case ENTITY_TYPE::ENEMY:
		ret = new Enemy({ (float)x,(float)y }, sampleEnemy, ENTITY_ALIGNEMENT::ENEMY);
		ret->minimapIcon = app->minimap->CreateIcon(&ret->position, MINIMAP_ICONS::ENEMY);
		break;

	default:
		break;
	}

	if (ret != nullptr)
	{
		entityVector.push_back(ret);
	}

	return ret;
}


// Checks if there is an entity in the mouse Click position 
Entity* ModuleEntityManager::CheckEntityOnClick(iMPoint mousePos)
{
	int numEntities = entityVector.size();

	Collider* col;

	for (int i = 0; i < numEntities; i++)
	{
		col = entityVector[i]->GetCollider();

		//dynamic entities get priority over static entities

		if (col != nullptr && (entityVector[i]->GetType() == ENTITY_TYPE::HERO_GATHERER || entityVector[i]->GetType() == ENTITY_TYPE::HERO_MELEE || entityVector[i]->GetType() == ENTITY_TYPE::HERO_RANGED || entityVector[i]->GetType() ==  ENTITY_TYPE::ENEMY))
		{
			if (mousePos.PointInRect(&col->rect))
			{
				return entityVector[i];
			}
		}
	}
	for (int i = 0; i < numEntities; i++)
	{
		col = entityVector[i]->GetCollider();

		if (col != nullptr) {
			{
				if (mousePos.PointInRect(&col->rect))
				{
					return entityVector[i];
				}
			}
		}
	}

	return nullptr;
}


void ModuleEntityManager::CheckHeroOnSelection(SDL_Rect& selection, std::vector<Hero*>* heroPlayerVector)
{
	int numHeroes = entityVector.size();

	heroPlayerVector->clear();

	Collider* col;

	for (int i = 0; i < numHeroes; i++)
	{
		if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_GATHERER || entityVector[i]->GetType() == ENTITY_TYPE::HERO_RANGED || entityVector[i]->GetType() == ENTITY_TYPE::HERO_MELEE)
		{
			col = entityVector[i]->GetCollider();

			if (col != nullptr)
			{
				if (col->CheckCollision(selection))
				{
					heroPlayerVector->push_back((Hero*)entityVector[i]);
				}
			}
		}
	}
}


void ModuleEntityManager::CheckDynamicEntitysObjectives(Entity* entity)
{
	int numEntities = entityVector.size();

	ENTITY_TYPE type;

	for (int i = 0; i < numEntities; i++)
	{
		type = entityVector[i]->GetType();

		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED)
		{
			Hero* hero = (Hero*)entityVector[i];
			hero->CheckObjecive(entity);
		}

		else if (type == ENTITY_TYPE::ENEMY)
		{
			Enemy* enemy = (Enemy*)entityVector[i];
			enemy->CheckObjecive(entity);
		}
	}
}


Entity* ModuleEntityManager::SearchEntityRect(SDL_Rect* rect, ENTITY_ALIGNEMENT alig)
{
	ENTITY_ALIGNEMENT alignementToSearch = ENTITY_ALIGNEMENT::UNKNOWN;
	ENTITY_ALIGNEMENT alignement;

	Collider* col;

	if (alig == ENTITY_ALIGNEMENT::ENEMY)
	{
		alignementToSearch = ENTITY_ALIGNEMENT::PLAYER;
	}

	else if (alig == ENTITY_ALIGNEMENT::PLAYER)
	{
		alignementToSearch = ENTITY_ALIGNEMENT::ENEMY;
	}


	int numEntities = entityVector.size();

	for (int i = numEntities - 1; i >= 0; i--)
	{
		alignement = entityVector[i]->GetAlignment();

		if (alignement != alignementToSearch)
		{
			continue;
		}

		col = entityVector[i]->GetCollider();

		if (col != nullptr)
		{
			if (col->CheckCollision(*rect))
			{
				return entityVector[i];
			}
		}
	}

	return nullptr;
}


void ModuleEntityManager::RemoveDeletedEntities()
{
	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		if (entityVector[i]->toDelete == true)
		{
			CheckDynamicEntitysObjectives(entityVector[i]);

			delete entityVector[i];
			entityVector[i] = nullptr;
			entityVector.erase(entityVector.begin() + i);
		}
	}

}


void ModuleEntityManager::SpriteOrdering(float dt)
{
	int numEntities = entityVector.size();

	float w, h;

	Collider* col;
	fMPoint pos;

	for (int i = 0; i < numEntities; i++)
	{
		pos = entityVector[i]->GetPosition();
		
		col = entityVector[i]->GetCollider();
		if (col != nullptr)
		{
			w = col->rect.w;
			h = col->rect.h;
		}
		

		if (app->map->EntityInsideCamera(pos.x, pos.y, w, h) == true) {

			assert((int)ENTITY_TYPE::MAX_TYPE == MAX_ENTITY_TYPES);

			switch (entityVector[i]->GetType())
			{
			case ENTITY_TYPE::BUILDING:
			case ENTITY_TYPE::BLDG_BARRICADE:
			case ENTITY_TYPE::BLDG_BASE:
			case ENTITY_TYPE::BLDG_TURRET:
			case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
				//case ENTITY_TYPE::BLDG_CORE:	CORE_CONSTRUCTOR_NEEDED
				buildingVector.push_back(entityVector[i]);
				break;
			case ENTITY_TYPE::ENEMY:
			case ENTITY_TYPE::HERO_GATHERER:
			case ENTITY_TYPE::HERO_MELEE:
			case ENTITY_TYPE::HERO_RANGED:
				movableEntityVector.push_back(entityVector[i]);
				break;

			default:
				break;
			}
		}
	}

	EntityQuickSort(movableEntityVector, 0, movableEntityVector.size());
	EntityQuickSort(buildingVector, 0, buildingVector.size());

	while (buildingVector.size() != 0 || movableEntityVector.size() != 0)
	{
		SPRITE_POSITION pivotEnum = SPRITE_POSITION::BOTH_NULL;

		if (buildingVector.size() == 0)
		{
			pivotEnum = SPRITE_POSITION::NULL_BUILDING;
		}

		else if (movableEntityVector.size() == 0)
		{
			pivotEnum = SPRITE_POSITION::NULL_MOVABLE_ENTITY;
		}

		else
		{
			pivotEnum = CheckSpriteHeight(movableEntityVector.front(), buildingVector.front());
		}

		switch (pivotEnum)
		{
		case SPRITE_POSITION::BEHIND_BUILDING:
		case SPRITE_POSITION::HIGHER_THAN_BUILDING:
		case SPRITE_POSITION::NULL_BUILDING:
			renderVector.push_back(movableEntityVector.front());
			movableEntityVector.erase(movableEntityVector.cbegin());
			break;
		case SPRITE_POSITION::LOWER_THAN_BUILDING:
		case SPRITE_POSITION::NULL_MOVABLE_ENTITY:
			renderVector.push_back(buildingVector.front());
			buildingVector.erase(buildingVector.cbegin());
			break;
		case SPRITE_POSITION::BOTH_NULL:
			break;
		}

	}

	numEntities = renderVector.size();


	for (int i = 0; i < numEntities; i++)
	{
		if (renderVector[i]->visionEntity != nullptr)
		{
			if (renderVector[i]->visionEntity->isVisible)
			{
				renderVector[i]->Draw(dt);
			}
		}
		else
		{
			renderVector[i]->Draw(dt);
		}
	}

	renderVector.clear();

}

void ModuleEntityManager::DrawOnlyStaticBuildings()
{
	int numEntities = entityVector.size();
	float scale = app->minimap->minimapScaleRelation;
	float halfWidth = app->minimap->minimapWidth * 0.5f;
	
	for (int i = 0; i < numEntities; i++)
	{
		if (entityVector[i]->GetType() == ENTITY_TYPE::BUILDING)
		{

			entityVector[i]->MinimapDraw(scale, halfWidth);
		}
		
	}

}



void ModuleEntityManager::EntityQuickSort(std::vector<Entity*>& vector, int low, int high)
{
	if (low < high)
	{
		int numElem = vector.size();
		int pivotLocation = EntityPartition(vector, low, high);
		EntityQuickSort(vector, low, pivotLocation);
		EntityQuickSort(vector, pivotLocation + 1, high);
	}
}


int ModuleEntityManager::EntityPartition(std::vector<Entity*>& vector, int low, int high)
{
	Entity* pivot = vector[low];
	Entity* auxVec = nullptr;
	int left = low;

	for (int i = low + 1; i < high; i++)
	{
		if (vector[i]->GetPosition().y + (float)vector[i]->GetCollider()->rect.h <= pivot->GetPosition().y + (float)pivot->GetCollider()->rect.h)
		{
			auxVec = vector[i];
			vector[i] = vector[left];
			vector[left] = auxVec;
			left++;
		}
	}

	auxVec = pivot;
	pivot = vector[left];

	return left;
}


void ModuleEntityManager::ExecuteEvent(EVENT_ENUM eventId)
{
	iMPoint pos;

	switch (eventId)
	{
	case EVENT_ENUM::ENTITY_DEAD:
		RemoveDeletedEntities();
		break;

	case EVENT_ENUM::DAY_START:
		SDL_SetTextureColorMod(buildingTexture, 255, 255, 255);
		SDL_SetTextureColorMod(base1Texture, 255, 255, 255);
		break;

	case EVENT_ENUM::NIGHT_START:
		SDL_SetTextureColorMod(buildingTexture, 86, 53, 138);
		SDL_SetTextureColorMod(base1Texture, 86, 53, 138);
		break;

	case EVENT_ENUM::KILL_ALL_ENEMIES:
		KillAllEnemies();
		break;

	case EVENT_ENUM::ACTIVATE_GODMODE_HEROES:
		ActivateGodModeHeroes();
		break;

	case EVENT_ENUM::DESACTIVATE_GODMODE_HEROES:
		DesactivateGodModeHeroes();
		break;

	case EVENT_ENUM::SPAWN_BASE:

		app->input->GetMousePositionRaw(pos.x, pos.y);
		pos.x = (-app->render->currentCamX + pos.x) / app->win->GetScale();
		pos.y = (-app->render->currentCamY + pos.y) / app->win->GetScale();
		AddEntity(ENTITY_TYPE::BLDG_BASE, pos.x, pos.y);
		break;


	case EVENT_ENUM::SPAWN_BUILDING:
		
		app->input->GetMousePositionRaw(pos.x, pos.y);
		pos.x = (-app->render->currentCamX + pos.x) / app->win->GetScale();
		pos.y = (-app->render->currentCamY + pos.y) / app->win->GetScale();
		AddEntity(ENTITY_TYPE::BUILDING, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_ENEMY:
		
		app->input->GetMousePositionRaw(pos.x, pos.y);
		pos.x = (-app->render->currentCamX + pos.x) / app->win->GetScale();
		pos.y = (-app->render->currentCamY + pos.y) / app->win->GetScale();
		AddEntity(ENTITY_TYPE::ENEMY, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_GATHERER_HERO:
		
		app->input->GetMousePositionRaw(pos.x, pos.y);
		pos.x = (-app->render->currentCamX + pos.x) / app->win->GetScale();
		pos.y = (-app->render->currentCamY + pos.y) / app->win->GetScale();
		AddEntity(ENTITY_TYPE::HERO_GATHERER, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_MELEE_HERO:
		
		app->input->GetMousePositionRaw(pos.x, pos.y);
		pos.x = (-app->render->currentCamX + pos.x) / app->win->GetScale();
		pos.y = (-app->render->currentCamY + pos.y) / app->win->GetScale();
		AddEntity(ENTITY_TYPE::HERO_MELEE, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_RANGED_HERO:
		
		app->input->GetMousePositionRaw(pos.x, pos.y);
		pos.x = (-app->render->currentCamX + pos.x) / app->win->GetScale();
		pos.y = (-app->render->currentCamY + pos.y) / app->win->GetScale();
		AddEntity(ENTITY_TYPE::HERO_RANGED, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_TURRET:
		
		app->input->GetMousePositionRaw(pos.x, pos.y);
		pos.x = (-app->render->currentCamX + pos.x) / app->win->GetScale();
		pos.y = (-app->render->currentCamY + pos.y) / app->win->GetScale();
		AddEntity(ENTITY_TYPE::BLDG_TURRET, pos.x, pos.y);
		break;
	}

}


void ModuleEntityManager::GetEntityNeighbours(std::vector<DynamicEntity*>* close_entity_list, std::vector<DynamicEntity*>* colliding_entity_list, DynamicEntity* thisUnit)
{
	close_entity_list->clear();
	colliding_entity_list->clear();

	DynamicEntity* it;

	for (uint i = 0; i < entityVector.size(); ++i)
	{
		if (!entityVector[i]->dynamic)
		{
			continue;
		}
		else
		{
			it = (DynamicEntity*)entityVector[i];

			if (it != thisUnit && it->GetAlignment() == thisUnit->GetAlignment())
			{
				fMPoint pos = it->GetPosition();

				float distance = pos.DistanceTo(thisUnit->GetPosition());
				if (distance < thisUnit->moveRange2)
				{
					colliding_entity_list->push_back(it);

				}
				if (distance < thisUnit->moveRange1)
				{
					close_entity_list->push_back(it);
				}
			}
		}
	}

}


SPRITE_POSITION ModuleEntityManager::CheckSpriteHeight(Entity* movEntity, Entity* building) const
{
	if (movEntity->GetPosition().y + movEntity->GetCollider()->rect.h < building->GetPosition().y)
	{
		return SPRITE_POSITION::HIGHER_THAN_BUILDING;
	}

	else if ((movEntity->GetPosition().y < building->GetPosition().y && movEntity->GetPosition().y + movEntity->GetCollider()->rect.h > building->GetPosition().y)
		|| (movEntity->GetPosition().y > building->GetPosition().y && movEntity->GetPosition().y + movEntity->GetCollider()->rect.h < building->GetPosition().y + building->GetCollider()->rect.h))
	{
		return SPRITE_POSITION::BEHIND_BUILDING;
	}

	else
	{
		return SPRITE_POSITION::LOWER_THAN_BUILDING;
	}
}


void ModuleEntityManager::PlayerBuildPreview(int x, int y, ENTITY_TYPE type)
{
	SDL_Rect rect;

	switch (type)
	{
	case ENTITY_TYPE::BUILDING:

		SDL_QueryTexture(testBuilding->GetTexture(), NULL, NULL, &rect.w, &rect.h);

		x -= rect.w / 2;
		y -= rect.h / 2;

		testBuilding->ActivateTransparency();
		testBuilding->SetPosition(x, y);
		testBuilding->Draw(0);
		break;


	case ENTITY_TYPE::BLDG_TURRET:

		/* ¿WHY?
		
		SDL_QueryTexture(testTurret->GetTexture(), NULL, NULL, &rect.w, &rect.h);

		x -= rect.w / 2;
		y -= rect.h / 2;

		sampleBase->ActivateTransparency();
		sampleBase->SetPosition(x, y);
		sampleBase->Draw(0);*/


		break;


	case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
		break;


	case ENTITY_TYPE::BLDG_BASE:

		SDL_QueryTexture(sampleBase->GetTexture(), NULL, NULL, &rect.w, &rect.h);

		x -= rect.w / 2;
		y -= rect.h / 2;

		sampleBase->ActivateTransparency();
		sampleBase->SetPosition(x, y);
		sampleBase->Draw(0);
		break;


	case ENTITY_TYPE::BLDG_BARRICADE:
		break;



	default:

		break;
	}

}

void ModuleEntityManager::DeleteAllEntities()
{
	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		RELEASE(entityVector[i]);
		entityVector[i] = nullptr;
	}

	entityVector.clear();
}

Hero* ModuleEntityManager::CheckUIAssigned(int& anotherHeroWithoutUI)
{
	int numEntities = entityVector.size();

	Hero* hero = nullptr;

	anotherHeroWithoutUI = 1;

	for (int i = 0; i < numEntities; i++)
	{
		switch (entityVector[i]->GetType())
		{
		case ENTITY_TYPE::HERO_GATHERER:
		case ENTITY_TYPE::HERO_MELEE:
		case ENTITY_TYPE::HERO_RANGED:
			if (entityVector[i]->UIAssigned == false)
			{
				if (hero == nullptr)
				{
					hero = (Hero*)entityVector[i];
					entityVector[i]->UIAssigned = true;
				}
				else
				{
					anotherHeroWithoutUI = 0;
				}
			}
			break;
		}
	}

	return hero;
}

Entity* ModuleEntityManager::SearchUnitsInRange(float checkdistance, Entity* thisUnit)
{
	fMPoint pos = thisUnit->GetPosition();
	float minDistance = checkdistance;
	Entity* ret = nullptr;
	float currDistance = 0.f;

	for (uint i = 0; i < entityVector.size(); ++i)
	{
		if (entityVector[i] != thisUnit && thisUnit->IsOpositeAlignement(entityVector[i]->GetAlignment()) && !entityVector[i]->toDelete)
		{
			currDistance = pos.DistanceTo(thisUnit->GetPosition());

			if (currDistance <= minDistance)
			{
				minDistance = currDistance;
				ret = entityVector[i];
			}
		}
	}

	return ret;
}


void ModuleEntityManager::ActivateGodModeHeroes()
{
	ENTITY_TYPE type;
	Hero* hero;

	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		type = entityVector[i]->GetType();

		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED)
		{
			hero = (Hero*)entityVector[i];
			hero->godMode = true;
		}
	}
}


void ModuleEntityManager::DesactivateGodModeHeroes()
{
	ENTITY_TYPE type;
	Hero* hero;

	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		type = entityVector[i]->GetType();

		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED)
		{
			hero = (Hero*)entityVector[i];
			hero->godMode = false;
		}
	}
}


void ModuleEntityManager::KillAllEnemies()
{
	Enemy* enemy;

	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		if (entityVector[i]->GetType() == ENTITY_TYPE::ENEMY)
		{
			enemy = (Enemy*)entityVector[i];
			enemy->Die();
		}
	}
}

bool ModuleEntityManager::BuildArea(skillArea* areaToGenerate, int width, int height, int radius)
{
	switch (areaToGenerate->form)
	{
	case AREA_TYPE::CIRCLE:
	{
		areaToGenerate->area = BuildCircleArea(radius);
		areaToGenerate->radius = radius;
		return true;
	}
	break;
	case AREA_TYPE::QUAD:
	{
		areaToGenerate->area = BuildQuadArea(width, height);
		areaToGenerate->width = width;
		areaToGenerate->heigth = height;
		return true;
	}
	break;
	}

	return false;
}


unsigned short* ModuleEntityManager::BuildCircleArea(int radius)
{
	unsigned short* circle = nullptr;

	int diameter = (radius * 2) + 1;
	iMPoint center = { radius,radius };
	circle = new unsigned short[diameter * diameter];

	for (int y = 0; y < diameter; y++)
	{
		for (int x = 0; x < diameter; x++)
		{
			if (app->fowManager->InsideCircle(center, { x,y }, radius) == true)
			{
				circle[(y * diameter) + x] = 1;
			}
			else
			{
				circle[(y * diameter) + x] = 0;
			}
		}
	}

	return circle;
}

unsigned short* ModuleEntityManager::BuildQuadArea(int w, int h)
{
	unsigned short* quad = nullptr;

	quad = new unsigned short[w * h];

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			quad[y + x] = 1;
		}
	}

	return quad;
}

skillArea* ModuleEntityManager::RequestArea(SKILL_ID callback, std::vector <iMPoint>* toFill, iMPoint center)
{
	skillArea* ret = nullptr;
	std::unordered_map<SKILL_ID, skillArea> ::iterator it;

	for (it = skillAreas.begin(); it != skillAreas.end(); it++)
	{
		if (it->first == callback)
		{
			ret = &it->second;
			break;
		}
	}

	if (it == skillAreas.end())
		return ret;

	center = app->map->WorldToMap(round(center.x), round(center.y));
	GenerateDynArea(toFill, ret, center);

	return ret;
}

void ModuleEntityManager::GenerateDynArea(std::vector <iMPoint>* toFill, skillArea* area, iMPoint center)
{
	switch (area->form)
	{
	case AREA_TYPE::CIRCLE:
	{
		int diameter = (area->radius * 2) + 1;
		iMPoint posCheck = center - area->radius;
		toFill->clear();

		for (int y = 0; y < diameter; y++)
		{
			for (int x = 0; x < diameter; x++)
			{
				if (area->area[(y * diameter) + x] == 1 && app->pathfinding->IsWalkable(posCheck + iMPoint{ x, y }))
				{
					toFill->push_back(posCheck + iMPoint{ x, y });
				}
			}
		}
	}
	break;
	case AREA_TYPE::QUAD:
	{}
	break;
	}
}

bool ModuleEntityManager::ExecuteSkill(int dmg, iMPoint pivot, skillArea* area, ENTITY_ALIGNEMENT target,
	SKILL_TYPE type, bool hurtYourself,  Entity* objective)
{
	bool ret = false;

	switch (type)
	{
	case SKILL_TYPE::SINGLE_TARGET:
	{}
	break;
	case SKILL_TYPE::AREA_OF_EFFECT:
	{
		int numEntities = entityVector.size();
		Collider* entColl = nullptr;
		float halfH = app->map->data.tileHeight * 0.5;
		float halfW = app->map->data.tileWidth * 0.5;
		float newRad = sqrt(halfW * halfW + halfH * halfH) * area->radius + 0.5f * area->radius;

		for (int i = 0; i < numEntities; i++)
		{
			if (entityVector[i]->GetAlignment() != target && (hurtYourself && entityVector[i] != objective))
				continue;

			entColl = entityVector[i]->GetCollider();

			if (entColl == nullptr)
				continue;

			switch (area->form)
			{
			case AREA_TYPE::CIRCLE:
			{
				if (entColl->CheckCollisionCircle(pivot, newRad))
					entityVector[i]->RecieveDamage(dmg);
			}
			break;
			case AREA_TYPE::QUAD:
			{}
			break;

			}
		}
	}
	break;
	}
	return true;
}