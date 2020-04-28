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
#include "Player.h"
#include "TestScene.h"

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


ModuleEntityManager::ModuleEntityManager() :

	base2Texture(nullptr),
	base2TextureEnemy(nullptr),
	base2TextureSelected(nullptr),
	base2TextureSelectedEnemy(nullptr),
	suitManTexture(nullptr),
	armorMaleTexture(nullptr),
	combatFemaleTexture(nullptr),
	buildingTexture(nullptr),
	base1Texture(nullptr),
	deco3Selected(nullptr),
	turretTexture(nullptr),
	enemyTexture(nullptr),

	sampleGatherer(nullptr),
	sampleMelee(nullptr),
	sampleEnemy(nullptr),
	sampleSpawner(nullptr),
	sampleBuilding(nullptr),
	sampleBase(nullptr),
	sampleTurret(nullptr)

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


	// Sample Hero Gatherer---------------------
	

	filename = config.child("load").attribute("docnameSuitman").as_string();
	pugi::xml_document suitmandoc;
	suitmandoc.load_file(filename.GetString());
	pugi::xml_node suitman = suitmandoc.child("suitman");
	
	LoadSampleHero(ENTITY_TYPE::HERO_GATHERER, suitman, config);

	suitmandoc.reset();

	// Sample Hero Melee---------------------
	filename = config.child("load").attribute("docnameArmoredman").as_string();
	pugi::xml_document armoredmanDoc;
	armoredmanDoc.load_file(filename.GetString());
	pugi::xml_node armoredman = armoredmanDoc.child("armoredman");
	
	LoadSampleHero(ENTITY_TYPE::HERO_MELEE, armoredman, config);
	armoredmanDoc.reset();


	// Sample Ranged Melee---------------------
	filename = config.child("load").attribute("docnameRangedman").as_string();
	pugi::xml_document rangedmanDoc;
	rangedmanDoc.load_file(filename.GetString());
	pugi::xml_node rangedman = rangedmanDoc.child("rangedman");

	LoadSampleHero(ENTITY_TYPE::HERO_RANGED, rangedman, config);
	rangedmanDoc.reset();

	// Sample Enemy---------------------
	filename = config.child("load").attribute("docnameWanamingo").as_string();
	pugi::xml_document wanamingodoc;
	wanamingodoc.load_file(filename.GetString());
	pugi::xml_node wanamingo = wanamingodoc.child("wanamingo");

	LoadSampleEnemy(wanamingo);

	wanamingodoc.reset();

	// Sample Crazy Turret Melee---------------------
	filename = config.child("load").attribute("docnameTurret").as_string();
	pugi::xml_document turretdoc;
	turretdoc.load_file(filename.GetString());
	pugi::xml_node turret = turretdoc.child("turret");

	Animation turretIdleRight = turretIdleRight.PushAnimation(turret, "turretIdleRight"); //goes up then bumps right
	Animation turretIdleRightUp = turretIdleRightUp.PushAnimation(turret, "turretIdleRightUp"); //bumps left
	Animation turretIdleRightDown = turretIdleRightDown.PushAnimation(turret, "turretIdleRightDown"); //bumps right
	Animation turretIdleLeft = turretIdleLeft.PushAnimation(turret, "turretIdleLeft"); //bumps left
	Animation turretIdleLeftUp = turretIdleLeftUp.PushAnimation(turret, "turretIdleLeftUp"); //bumps right
	Animation turretIdleLeftDown = turretIdleLeftDown.PushAnimation(turret, "turretIdleLeftDown"); //bumps right

	Animation turretShootingRight = turretShootingRight.PushAnimation(turret, "turretShootingRight"); //goes up then bumps right
	Animation turretShootingRightUp = turretShootingRightUp.PushAnimation(turret, "turretShootingRightUp"); //bumps left
	Animation turretShootingRightDown = turretShootingRightDown.PushAnimation(turret, "turretShootingRightDown"); //bumps right
	Animation turretShootingLeft = turretShootingLeft.PushAnimation(turret, "turretShootingLeft"); //bumps left
	Animation turretShootingLeftUp = turretShootingLeftUp.PushAnimation(turret, "turretShootingLeftUp"); //bumps right
	Animation turretShootingLeftDown = turretShootingLeftDown.PushAnimation(turret, "turretShootingLeftDown"); //bumps right

	turretdoc.reset();

	//Enemy collider and spawner
	Collider* spawnerCollider = new Collider({ 0,0,5,5 }, COLLIDER_RECLUIT_IA, app->ai);

	int maxHP = 25;
	int recoveryHP = 1;

	int atkDmg = 2;
	float atkSpd = 1.75f;
	int atkRange = 40;

	int movSpd = 125;
	int vision = 200;
	int xp = 15;

	sampleSpawner = new Spawner(fMPoint{ 150, 250 }, ENTITY_TYPE::ENEMY, spawnerCollider, sampleEnemy->hitPointsMax, sampleEnemy->hitPointsCurrent);

	//Test building
	Collider* buildingCollider = new Collider({ -150,130,430, 330 }, COLLIDER_VISIBILITY, this);
	sampleBuilding = new Building(fMPoint{ 0,0 }, 100, 100, 100, 100, 100, 100, buildingCollider);

	// Test Turret
	maxHP = 110;
	recoveryHP = 0;

	atkDmg = 3;
	atkSpd = 2.25f;
	atkRange = 175;
	xp = 35;


	Collider* turretCollider = new Collider({ 150,130,70,80 }, COLLIDER_VISIBILITY, this);
	sampleTurret = new Turret(1, atkDmg, atkSpd, atkRange, fMPoint{ 0, 0 }, turretCollider, turretIdleRight, turretIdleRightUp, turretIdleRightDown, turretIdleLeft, turretIdleLeftUp, turretIdleLeftDown,
		turretShootingRight, turretShootingRightUp, turretShootingRightDown, turretShootingLeft, turretShootingLeftUp, turretShootingLeftDown,
		maxHP, maxHP, recoveryHP, xp, 95, 160);

	//Template base
	maxHP = 500;
	int recoverHPRate = 20;

	int maxTurrets = 2;
	int resourcesProd = 5;
	int resourcesRate = 5;

	Collider* baseCollider = new Collider({ -150,130,480,410 }, COLLIDER_VISIBILITY, this);
	Collider* baseAlarmCollider = new Collider({ 0, 0, 700, 650 }, COLLIDER_BASE_ALERT, app->ai);
	sampleBase = new Base(fMPoint{ 0, 0 }, baseCollider, maxTurrets, 5, nullptr, baseAlarmCollider, resourcesProd, resourcesRate,
		maxHP, maxHP, recoverHPRate, 100);


	//Generate Areas------------------------------------
	skillArea gathererSkill1AreaRange;
	gathererSkill1AreaRange.form = AREA_TYPE::CIRCLE;
	BuildArea(&gathererSkill1AreaRange, 0, 0, 6);
	skillAreas.insert({ SKILL_ID::GATHERER_SKILL1, gathererSkill1AreaRange });

	skillArea gathererSkill1AreaExplosion;
	gathererSkill1AreaRange.form = AREA_TYPE::CIRCLE;
	BuildArea(&gathererSkill1AreaRange, 0, 0, 2);
	skillAreas.insert({ SKILL_ID::GATHERER_SKILL1_MOUSE, gathererSkill1AreaRange });

	skillArea meleeSkill1Area;
	meleeSkill1Area.form = AREA_TYPE::CIRCLE;
	BuildArea(&meleeSkill1Area, 0, 0, 2);
	skillAreas.insert({ SKILL_ID::MELEE_SKILL1, meleeSkill1Area });

	skillArea baseConstruction;
	baseConstruction.form = AREA_TYPE::CIRCLE;
	BuildArea(&baseConstruction, 0, 0, 8);
	skillAreas.insert({ SKILL_ID::BASE_AREA, baseConstruction });

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
	base2TextureSelected = app->tex->Load("maps/base02_selected.png");
	base2TextureEnemy = app->tex->Load("maps/base02_enemy.png");
	base2TextureSelectedEnemy = app->tex->Load("maps/base02_enemy_selected.png");

	deco3Selected = app->tex->Load("maps/base03_selected.png");

	selectedTexture = app->tex->Load("spritesheets/VFX/selected.png");
	targetedTexture = app->tex->Load("spritesheets/VFX/target.png");

	explosionTexture = app->tex->Load("spritesheets/VFX/explosion.png");


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

	app->eventManager->EventRegister(EVENT_ENUM::RANGED_RESURRECT, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_RESURRECT, this);
	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_RESURRECT, this);


	sampleBuilding->SetTexture(base1Texture);
	sampleBase->SetTexture(base2Texture);

	sampleTurret->SetTexture(turretTexture);

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

	noise1Armored = app->audio->LoadFx("audio/sfx/Heroes/Suitman/noise1.wav");
	noise2Armored = app->audio->LoadFx("audio/sfx/Heroes/Suitman/noise2.wav");
	noise3Armored = app->audio->LoadFx("audio/sfx/Heroes/Suitman/noise3.wav");
	noise4Armored = app->audio->LoadFx("audio/sfx/Heroes/Suitman/noise4.wav");

	//Buildings sfx--------
	buildingGetsHit = app->audio->LoadFx("audio/sfx/Buildings/hit1.wav");
	buildingGetsHit2 = app->audio->LoadFx("audio/sfx/Buildings/hit2.wav");
	turretShooting = app->audio->LoadFx("audio/sfx/Buildings/shooting1.wav");

	//Armored sfx--------
	noise1Suitman = app->audio->LoadFx("audio/sfx/Heroes/Armoredman/noise1.wav");
	noise2Suitman = app->audio->LoadFx("audio/sfx/Heroes/Armoredman/noise2.wav");
	noise3Suitman = app->audio->LoadFx("audio/sfx/Heroes/Armoredman/noise3.wav");
	noise4Suitman = app->audio->LoadFx("audio/sfx/Heroes/Armoredman/noise4.wav");

	armored1Skill2 = app->audio->LoadFx("audio/sfx/Heroes/Armoredman/Skill1_2.wav");

	//General hero sfx--------
	lvlup = app->audio->LoadFx("audio/sfx/Heroes/lvlup.wav");

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
	ENTITY_ALIGNEMENT alignement = ENTITY_ALIGNEMENT::UNKNOWN;

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

				entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::HERO, entityVector[i]->GetCenter());
				break;

			case ENTITY_TYPE::HERO_RANGED:
				entityVector[i]->Start(combatFemaleTexture);
				app->eventManager->GenerateEvent(EVENT_ENUM::HERO_RANGED_CREATED, EVENT_ENUM::NULL_EVENT);

				entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::HERO, entityVector[i]->GetCenter());
				break;

			case ENTITY_TYPE::HERO_GATHERER:
				entityVector[i]->Start(suitManTexture);
				app->eventManager->GenerateEvent(EVENT_ENUM::HERO_GATHERER_CREATED, EVENT_ENUM::NULL_EVENT);

				entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::HERO, entityVector[i]->GetCenter());
				break;

			case ENTITY_TYPE::ENEMY:
				entityVector[i]->Start(enemyTexture);

				entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::ENEMY, entityVector[i]->GetCenter());
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
					bld->selectedTexture = deco3Selected;
					break;
				default:
					DecorTex = base1Texture;
					break;
				}

				entityVector[i]->Start(DecorTex);

				break;

			case ENTITY_TYPE::BLDG_TURRET:
				entityVector[i]->Start(turretTexture);

				alignement = entityVector[i]->GetAlignment();

				if (alignement == ENTITY_ALIGNEMENT::PLAYER)
				{

					entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::TURRET, entityVector[i]->GetCenter());
				}
				else if (alignement == ENTITY_ALIGNEMENT::ENEMY)
				{
					entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::ENEMY_TURRET, entityVector[i]->GetCenter()); //TODO CHANGE THIS FOR ENEMY TURRET
				}
				break;

			case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
				break;

			case ENTITY_TYPE::BLDG_BASE:

				Base* auxBase; auxBase = (Base*)entityVector[i];
				alignement = entityVector[i]->GetAlignment();

				if (alignement == ENTITY_ALIGNEMENT::PLAYER || alignement == ENTITY_ALIGNEMENT::NEUTRAL)
				{
					entityVector[i]->Start(base2Texture);
					entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::BASE, entityVector[i]->GetCenter());

					auxBase->selectedTexture = base2TextureSelected;
				}
				else if (alignement == ENTITY_ALIGNEMENT::ENEMY)
				{
					entityVector[i]->Start(base2TextureEnemy);
					entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::BASE, entityVector[i]->GetCenter());

					auxBase->selectedTexture = base2TextureSelectedEnemy;
				}

				break;

			case ENTITY_TYPE::BLDG_BARRICADE:
				break;

			case ENTITY_TYPE::SPAWNER:
				entityVector[i]->Start(nullptr);
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
	LOG("Entity Manager Clean Up");
	DeleteAllEntities();

	app->tex->UnLoad(suitManTexture);				suitManTexture = nullptr;
	app->tex->UnLoad(armorMaleTexture);				armorMaleTexture = nullptr;
	app->tex->UnLoad(combatFemaleTexture);			combatFemaleTexture = nullptr;
	app->tex->UnLoad(enemyTexture);					enemyTexture = nullptr;

	app->tex->UnLoad(buildingTexture);				buildingTexture = nullptr;
	app->tex->UnLoad(base1Texture);					base1Texture = nullptr;
	app->tex->UnLoad(base2Texture);					base2Texture = nullptr;
	app->tex->UnLoad(base2TextureEnemy);			base2TextureEnemy = nullptr;
	app->tex->UnLoad(base2TextureSelected);			base2TextureSelected = nullptr;
	app->tex->UnLoad(base2TextureSelectedEnemy);	base2TextureSelectedEnemy = nullptr;

	app->tex->UnLoad(deco3Selected);				deco3Selected = nullptr;

	app->tex->UnLoad(turretTexture);				turretTexture = nullptr;

	app->tex->UnLoad(debugPathTexture);				debugPathTexture = nullptr;

	app->tex->UnLoad(selectedTexture);				selectedTexture = nullptr;
	app->tex->UnLoad(explosionTexture);				explosionTexture = nullptr;
	app->tex->UnLoad(targetedTexture);				targetedTexture = nullptr;

	RELEASE(sampleGatherer);						sampleGatherer = nullptr;
	RELEASE(sampleMelee);							sampleMelee = nullptr;

	RELEASE(sampleEnemy);							sampleEnemy = nullptr;
	RELEASE(sampleSpawner);							sampleSpawner = nullptr;

	RELEASE(sampleBuilding);						sampleBuilding = nullptr;
	RELEASE(sampleBase);							sampleBase = nullptr;
	RELEASE(sampleTurret);							sampleTurret = nullptr;


	for (std::unordered_map<SKILL_ID, skillArea> ::iterator it = skillAreas.begin(); it != skillAreas.end(); it++)
	{
		delete it->second.area;
		it->second.area = nullptr;

	}
	skillAreas.clear();

	renderVector.clear();
	movableEntityVector.clear();
	buildingVector.clear();
	selectedVector.clear();

	app->eventManager->EventUnRegister(EVENT_ENUM::DAY_START, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::NIGHT_START, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::ENTITY_DEAD, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::ACTIVATE_GODMODE_HEROES, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::DESACTIVATE_GODMODE_HEROES, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::KILL_ALL_ENEMIES, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::SPAWN_BASE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SPAWN_BUILDING, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SPAWN_ENEMY, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SPAWN_GATHERER_HERO, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SPAWN_MELEE_HERO, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SPAWN_RANGED_HERO, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SPAWN_TURRET, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_RESURRECT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_RESURRECT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_RESURRECT, this);

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
		ret = new MeleeHero({ (float)x,(float)y }, sampleMelee, ENTITY_ALIGNEMENT::PLAYER);
		break;

	case ENTITY_TYPE::HERO_RANGED:
		ret = new RangedHero({ (float)x,(float)y }, sampleRanged, ENTITY_ALIGNEMENT::PLAYER);
		break;

	case ENTITY_TYPE::HERO_GATHERER:
		ret = new GathererHero({ (float)x,(float)y }, sampleGatherer, ENTITY_ALIGNEMENT::PLAYER);
		break;

	case ENTITY_TYPE::BUILDING:
		ret = new Building({ (float)x,(float)y }, sampleBuilding, alignement);
		break;

	case ENTITY_TYPE::BLDG_TURRET:
		ret = new Turret({ (float)x,(float)y }, sampleTurret, alignement);

		break;

	case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
		break;

	case ENTITY_TYPE::BLDG_BASE:
		ret = new Base({ (float)x,(float)y }, sampleBase, alignement);
		app->ai->PushBase((Base*)ret);
		break;

	case ENTITY_TYPE::BLDG_BARRICADE:
		break;

	case ENTITY_TYPE::ENEMY:
		ret = new Enemy({ (float)x,(float)y }, sampleEnemy, ENTITY_ALIGNEMENT::ENEMY);
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


Entity* ModuleEntityManager::GetSample(ENTITY_TYPE type)
{
	switch (type)
	{
	case ENTITY_TYPE::SPAWNER:
		return sampleSpawner;
		break;

	case ENTITY_TYPE::HERO_MELEE:
		return sampleMelee;
		break;

	case ENTITY_TYPE::HERO_RANGED:
		return sampleRanged;
		break;

	case ENTITY_TYPE::HERO_GATHERER:
		return sampleGatherer;
		break;

	case ENTITY_TYPE::ENEMY:
		return sampleEnemy;
		break;

	case ENTITY_TYPE::BUILDING:
		return sampleBuilding;
		break;

	case ENTITY_TYPE::BLDG_TURRET:
		return sampleTurret;
		break;

	case ENTITY_TYPE::BLDG_BASE:
		return sampleBase;
		break;

	default:
		return nullptr;
		break;
	}
}


// Checks if there is an entity in the mouse Click position 
Entity* ModuleEntityManager::CheckEntityOnClick(iMPoint mousePos, bool focus)
{
	Entity* ret = nullptr;

	Collider* col;
	ENTITY_TYPE type;

	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		type = entityVector[i]->GetType();
		col = entityVector[i]->GetCollider();

		//dynamic entities get priority over static entities
		if (mousePos.PointInRect(&col->rect))
		{
			if (col != nullptr && (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED))
			{
				if (focus == true)
				{
					for (int j = i + 1; j < numEntities; j++)
					{
						entityVector[j]->selectedByPlayer = false;
					}

					entityVector[i]->selectedByPlayer = true;
				}

				return entityVector[i];
			}

			ret = entityVector[i];
		}

		else
		{
			if (focus == true)
				entityVector[i]->selectedByPlayer = false;
		}
	}

	if (ret != nullptr && focus == true)
	{
		ret->selectedByPlayer = true;
	}

	return ret;
}


void ModuleEntityManager::CheckHeroOnSelection(SDL_Rect& selection, std::vector<Hero*>* heroPlayerVector)
{
	int numHeroes = entityVector.size();

	heroPlayerVector->clear();

	Collider* col;
	ENTITY_TYPE type;

	for (int i = 0; i < numHeroes; i++)
	{
		type = entityVector[i]->GetType();

		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_RANGED || type == ENTITY_TYPE::HERO_MELEE)
		{
			col = entityVector[i]->GetCollider();

			Hero* thisHero;
			thisHero = (Hero*)entityVector[i];

			if (col != nullptr)
			{
				if (col->CheckCollision(selection))
				{
					thisHero->selectedByPlayer = true;
					heroPlayerVector->push_back(thisHero);
				}
				else
				{
					thisHero->selectedByPlayer = false;
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
		entityVector[i]->CheckObjective(entity);
	}
}


Entity* ModuleEntityManager::SearchEntityRect(SDL_Rect* rect, ENTITY_ALIGNEMENT alig)
{
	ENTITY_ALIGNEMENT alignementToSearch = ENTITY_ALIGNEMENT::UNKNOWN;
	ENTITY_ALIGNEMENT alignement;

	Collider* col;
	Entity* ret = nullptr;
	float x, y;
	x = rect->x + (rect->w * 0.5);
	y = rect->y + (rect->h * 0.5);
	fMPoint center = { x,  y };

	fMPoint pos1, pos2;

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

		if (alignement != alignementToSearch || entityVector[i]->toDelete == true)
		{
			continue;
		}

		col = entityVector[i]->GetCollider();

		if (col != nullptr)
		{
			if (col->CheckCollision(*rect))
			{
				if (ret == nullptr)
				{
					ret = entityVector[i];
				}
				else
				{
					pos1 = ret->GetPosition();
					pos1.y -= ret->GetCenter().y;

					pos2 = entityVector[i]->GetPosition();
					pos2.y -= entityVector[i]->GetCenter().y;
					if (pos1.DistanceNoSqrt(center) > pos2.DistanceNoSqrt(center))
					{
						ret = entityVector[i];
					}
				}
			}
		}
	}

	return ret;
}


void ModuleEntityManager::RemoveDeletedEntities()
{
	ENTITY_TYPE type;

	for (int i = 0; i < entityVector.size(); i++)
	{
		if (entityVector[i]->toDelete == true)
		{
			CheckDynamicEntitysObjectives(entityVector[i]);
			app->player->CheckFocusedEntity(entityVector[i]);

			type = entityVector[i]->GetType();

			delete entityVector[i];
			entityVector[i] = nullptr;
			entityVector.erase(entityVector.begin() + i);

			i--;

			if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED)
			{
				SearchHeroesAlive();
			}

		}
	}

}


void ModuleEntityManager::SearchHeroesAlive()
{
	ENTITY_TYPE type;

	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		type = entityVector[i]->GetType();

		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED)
			return;
	}

	app->eventManager->GenerateEvent(EVENT_ENUM::GAME_LOSE, EVENT_ENUM::NULL_EVENT);
}


void ModuleEntityManager::SearchEnemiesAlive()
{
	ENTITY_TYPE type;

	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		type = entityVector[i]->GetType();

		if (type == ENTITY_TYPE::ENEMY)
			return;
	}

	if (app->testScene->IsNight() == true)
	{
		app->eventManager->GenerateEvent(EVENT_ENUM::GAME_WIN, EVENT_ENUM::NULL_EVENT);
	}


}


void ModuleEntityManager::SpriteOrdering(float dt)
{
	int numEntities = entityVector.size();

	float w, h;

	Collider* col = nullptr;
	fMPoint pos{ 0,0 };

	for (int i = 0; i < numEntities; i++)
	{
		pos = entityVector[i]->GetPosition();

		col = entityVector[i]->GetCollider();
		if (col != nullptr)
		{
			w = col->rect.w;
			h = col->rect.h;
		}


		if (app->map->EntityInsideCamera(pos.x, pos.y, w, h) == true)
		{
			//If a Entity Type is added, update the switch :D

			switch (entityVector[i]->GetType())
			{
			case ENTITY_TYPE::BUILDING:
			case ENTITY_TYPE::BLDG_BARRICADE:
			case ENTITY_TYPE::BLDG_BASE:
			case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
				//case ENTITY_TYPE::BLDG_CORE:	CORE_CONSTRUCTOR_NEEDED
				buildingVector.push_back(entityVector[i]);
				break;
			case ENTITY_TYPE::BLDG_TURRET:
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

	selectedVector = movableEntityVector;

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

	//icons
	for (int i = 0; i < selectedVector.size(); i++)
	{
		if ((selectedVector[i]->GetType() == ENTITY_TYPE::HERO_GATHERER) || (selectedVector[i]->GetType() == ENTITY_TYPE::HERO_MELEE) || (selectedVector[i]->GetType() == ENTITY_TYPE::HERO_RANGED))
		{

			if (selectedVector[i]->visionEntity != nullptr)
			{
				if (selectedVector[i]->visionEntity->isVisible)
				{
					Hero* thisHero = (Hero*)selectedVector[i];
					thisHero->DrawSelected();
				}
			}
			else if (selectedVector[i]->visionEntity != nullptr)
			{
				if (selectedVector[i]->visionEntity->isVisible)
				{
					Hero* thisHero = (Hero*)selectedVector[i];
					thisHero->DrawSelected();
				}
			}
		}

		if ((selectedVector[i]->GetType() == ENTITY_TYPE::BLDG_TURRET))
		{

			if (selectedVector[i]->visionEntity != nullptr)
			{
				if (selectedVector[i]->visionEntity->isVisible)
				{
					Turret* thisTurret = (Turret*)selectedVector[i];
					thisTurret->DrawSelected();
				}
			}
			else if (selectedVector[i]->visionEntity != nullptr)
			{
				if (selectedVector[i]->visionEntity->isVisible)
				{
					Turret* thisTurret = (Turret*)selectedVector[i];
					thisTurret->DrawSelected();
				}
			}
		}

		if (selectedVector[i]->GetType() == ENTITY_TYPE::ENEMY)
		{
			if (selectedVector[i]->visionEntity != nullptr)
			{
				if (selectedVector[i]->visionEntity->isVisible)
				{
					Enemy* thisEnemy = (Enemy*)selectedVector[i];
					thisEnemy->DrawOnSelect();
				}
			}
			else if (selectedVector[i]->visionEntity != nullptr)
			{
				if (selectedVector[i]->visionEntity->isVisible)
				{
					Enemy* thisEnemy = (Enemy*)selectedVector[i];
					thisEnemy->DrawOnSelect();
				}
			}
		}

	}
	selectedVector.clear();
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
	iMPoint pos = app->input->GetMousePosScreen();
	int entityNum = entityVector.size();
	pos.x = (-app->render->currentCamX + pos.x) / app->win->GetScale();
	pos.y = (-app->render->currentCamY + pos.y) / app->win->GetScale();

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

		AddEntity(ENTITY_TYPE::BLDG_BASE, pos.x, pos.y);
		break;


	case EVENT_ENUM::SPAWN_BUILDING:

		AddEntity(ENTITY_TYPE::BUILDING, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_ENEMY:

		AddEntity(ENTITY_TYPE::ENEMY, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_GATHERER_HERO:

		AddEntity(ENTITY_TYPE::HERO_GATHERER, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_MELEE_HERO:

		AddEntity(ENTITY_TYPE::HERO_MELEE, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_RANGED_HERO:

		AddEntity(ENTITY_TYPE::HERO_RANGED, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_TURRET:

		AddEntity(ENTITY_TYPE::BLDG_TURRET, pos.x, pos.y);
		break;

	case EVENT_ENUM::GATHERER_RESURRECT:
		// TODO REVIVE HEROES FUNCTION
		break;

	case EVENT_ENUM::RANGED_RESURRECT:

		break;

	case EVENT_ENUM::MELEE_RESURRECT:

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

		SDL_QueryTexture(sampleBuilding->GetTexture(), NULL, NULL, &rect.w, &rect.h);

		x -= rect.w / 2;
		y -= rect.h / 2;

		sampleBuilding->ActivateTransparency();
		sampleBuilding->SetPosition(x, y);
		sampleBuilding->Draw(0);
		break;


	case ENTITY_TYPE::BLDG_TURRET:

		rect = sampleTurret->GetCollider()->rect;

		x -= rect.w * 0.5f;
		y -= rect.h;

		sampleTurret->ActivateTransparency();
		sampleTurret->SetPosition(x, y);
		sampleTurret->Draw(0.0000001);


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
		app->player->CheckFocusedEntity(entityVector[i]);

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


int ModuleEntityManager::ExecuteSkill(int dmg, iMPoint pivot, skillArea* area, ENTITY_ALIGNEMENT target,
	SKILL_TYPE type, bool hurtYourself, Entity* objective)
{
	int ret = -1;

	switch (type)
	{
	case SKILL_TYPE::SINGLE_TARGET:
	{}
	break;
	case SKILL_TYPE::AREA_OF_EFFECT:
	{
		ret = 0;
		int numEntities = entityVector.size();
		Collider* entColl = nullptr;
		float halfH = app->map->data.tileHeight * 0.5;
		float halfW = app->map->data.tileWidth * 0.5;
		float newRad = sqrt(halfW * halfW + halfH * halfH) * area->radius + 0.5f * area->radius;

		for (int i = 0; i < numEntities; i++)
		{
			if (entityVector[i]->GetAlignment() != target)
			{
				if (!hurtYourself || entityVector[i] != objective)
				{
					continue;
				}
			}

			entColl = entityVector[i]->GetCollider();

			if (entColl == nullptr)
				continue;

			switch (area->form)
			{
			case AREA_TYPE::CIRCLE:
			{
				if (entColl->CheckCollisionCircle(pivot, newRad))
					ret += entityVector[i]->RecieveDamage(dmg);
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
	return ret;
}


void ModuleEntityManager::ResetEntityManager()
{
	DeleteAllEntities();

	if (base1Texture != nullptr)
	{
		SDL_SetTextureColorMod(buildingTexture, 255, 255, 255);
	}

	if (buildingTexture != nullptr)
	{
		SDL_SetTextureColorMod(base1Texture, 255, 255, 255);
	}

	app->fowManager->DeleteAllFoWEntites();
}

//returns true if a hero has been loaded into its sample, otherwise false
bool ModuleEntityManager::LoadSampleHero(ENTITY_TYPE heroType, pugi::xml_node& heroNode,pugi::xml_node& config)
{
	bool ret = false;
	//collider
	SDL_Rect r;
	r.x = heroNode.child("sample").child("collider").child("rect").attribute("x").as_int(0);
	r.y = heroNode.child("sample").child("collider").child("rect").attribute("y").as_int(0);
	r.w = heroNode.child("sample").child("collider").child("rect").attribute("w").as_int(0);
	r.h = heroNode.child("sample").child("collider").child("rect").attribute("h").as_int(0);
	COLLIDER_TYPE cType = (COLLIDER_TYPE)heroNode.child("sample").child("collider").child("type").attribute("id").as_int(0);

	Collider* collider = new Collider(r, cType, this);

	//stats
	fMPoint pos;
	pos.x = heroNode.child("sample").child("position").attribute("x").as_float(0);
	pos.y = heroNode.child("sample").child("position").attribute("y").as_float(0);

	int level = heroNode.child("sample").child("stats").attribute("level").as_int(0);
	int movSpd = heroNode.child("sample").child("stats").attribute("movementSpeed").as_int(0);
	int visTiles = heroNode.child("sample").child("stats").attribute("vision").as_int(0);


	int maxHP = heroNode.child("sample").child("stats").child("hitPoints").attribute("max").as_int(0);
	int currentHP = heroNode.child("sample").child("stats").child("hitPoints").attribute("current").as_int(0);
	int recoveryHP = heroNode.child("sample").child("stats").child("hitPoints").attribute("recoveryRate").as_int(0);

	int maxEnergy = heroNode.child("sample").child("stats").child("energyPoints").attribute("max").as_int(0);
	int currentEnergy = heroNode.child("sample").child("stats").child("energyPoints").attribute("current").as_int(0);
	int recoveryE = heroNode.child("sample").child("stats").child("energyPoints").attribute("recoveryRate").as_int(0);

	int atkDmg = heroNode.child("sample").child("stats").child("attack").attribute("damage").as_int(0);
	float atkSpd = heroNode.child("sample").child("stats").child("attack").attribute("speed").as_float(0);
	int atkRange = heroNode.child("sample").child("stats").child("attack").attribute("range").as_int(0);

	//skill1
	float skill1ExecTime = heroNode.child("sample").child("skills").child("skill1").attribute("executionTime").as_float(0);
	float skill1RecovTime = heroNode.child("sample").child("skills").child("skill1").attribute("recoverTime").as_float(0);
	int skill1Dmg = heroNode.child("sample").child("skills").child("skill1").attribute("damage").as_int(0);
	SKILL_ID skill1ID = (SKILL_ID)heroNode.child("sample").child("skills").child("skill1").attribute("id").as_int(0);
	SKILL_TYPE skill1Type = (SKILL_TYPE)heroNode.child("sample").child("skills").child("skill1").attribute("type").as_int(0);
	ENTITY_ALIGNEMENT skill1Target = (ENTITY_ALIGNEMENT)heroNode.child("sample").child("skills").child("skill1").attribute("targetAligment").as_int(0);

	//skill2
	float skill2ExecTime = heroNode.child("sample").child("skills").child("skill2").attribute("executionTime").as_float(0);
	float skill2RecovTime = heroNode.child("sample").child("skills").child("skill2").attribute("recoverTime").as_float(0);
	//skill3
	float skill3ExecTime = heroNode.child("sample").child("skills").child("skill3").attribute("executionTime").as_float(0);
	float skill3RecovTime = heroNode.child("sample").child("skills").child("skill3").attribute("recoverTime").as_float(0);


	//Animations Load ----------------------------
	Animation walkLeft = walkLeft.PushAnimation(heroNode, "walk_left");
	Animation walkLeftUp = walkLeftUp.PushAnimation(heroNode, "walk_left_up");
	Animation walkLeftDown = walkLeftDown.PushAnimation(heroNode, "walk_left_down");
	Animation walkRightUp = walkRightUp.PushAnimation(heroNode, "walk_right_up");
	Animation walkRightDown = walkRightDown.PushAnimation(heroNode, "walk_right_down");
	Animation walkRight = walkRight.PushAnimation(heroNode, "walk_right");

	Animation idleRight = idleRight.PushAnimation(heroNode, "idle_right");
	Animation idleRightUp = idleRightUp.PushAnimation(heroNode, "idle_right_up");
	Animation idleRightDown = idleRightDown.PushAnimation(heroNode, "idle_right_down");
	Animation idleLeft = idleLeft.PushAnimation(heroNode, "idle_left");
	Animation idleLeftUp = idleLeftUp.PushAnimation(heroNode, "idle_left_up");
	Animation idleLeftDown = idleLeftDown.PushAnimation(heroNode, "idle_left_down");

	Animation punchRight = punchRight.PushAnimation(heroNode, "punch_right");
	Animation punchRightUp = punchRightUp.PushAnimation(heroNode, "punch_right_up");
	Animation punchRightDown = punchRightDown.PushAnimation(heroNode, "punch_right_down");
	Animation punchLeft = punchLeft.PushAnimation(heroNode, "punch_left");
	Animation punchLeftUp = punchLeftUp.PushAnimation(heroNode, "punch_left_up");
	Animation punchLeftDown = punchLeftDown.PushAnimation(heroNode, "punch_left_down");

	Animation skill1Right = skill1Right.PushAnimation(heroNode, "skill_1_right");
	Animation skill1RightUp = skill1RightUp.PushAnimation(heroNode, "skill_1_right_up");
	Animation skill1RightDown = skill1RightDown.PushAnimation(heroNode, "skill_1_right_down");
	Animation skill1Left = skill1Left.PushAnimation(heroNode, "skill_1_left");
	Animation skill1LeftUp = skill1LeftUp.PushAnimation(heroNode, "skill_1_left_up");
	Animation skill1LeftDown = skill1LeftDown.PushAnimation(heroNode, "skill_1_left_down");

	switch (heroType)
	{
	case ENTITY_TYPE::HERO_MELEE:
				
		//Sample Creation ----------------------------
		sampleMelee = new MeleeHero(pos, collider, walkLeft, walkLeftUp,
			walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightUp, idleRightDown, idleLeft,
			idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp, punchRightDown, punchRight, skill1Right,
			skill1RightUp, skill1RightDown, skill1Left, skill1LeftUp, skill1LeftDown,
			level, maxHP, maxHP, recoveryHP, maxEnergy, maxEnergy, recoveryE, atkDmg, atkSpd, atkRange,
			movSpd, visTiles, skill1ExecTime, skill2ExecTime, skill3ExecTime, skill1RecovTime, skill2RecovTime, skill3RecovTime,
			skill1Dmg, skill1ID, skill1Type, skill1Target);

		ret = true;
		break;


	case ENTITY_TYPE::HERO_RANGED:
		
		//Sample Creation ----------------------------
		sampleRanged = new RangedHero(pos, collider, walkLeft, walkLeftUp,
			walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightUp, idleRightDown, idleLeft,
			idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp, punchRightDown, punchRight, skill1Right,
			skill1RightUp, skill1RightDown, skill1Left, skill1LeftUp, skill1LeftDown,
			1, maxHP, maxHP, recoveryHP, maxEnergy, maxEnergy, recoveryE, atkDmg, atkSpd, atkRange,
			movSpd, visTiles, skill1ExecTime, skill2ExecTime, skill3ExecTime, skill1RecovTime, skill2RecovTime, skill3RecovTime,
			skill1Dmg, skill1ID, skill1Type, skill1Target);

		ret = true;
		break;


	case ENTITY_TYPE::HERO_GATHERER:


		//Vfx load -----------------------------------
		P2SString filename = config.child("load").attribute("docnamevfx").as_string();
		pugi::xml_document vfxDoc;
		vfxDoc.load_file(filename.GetString());
		pugi::xml_node explosion = vfxDoc.child("Vfx");

		Animation vfxExplosion = vfxExplosion.PushAnimation(explosion, "explosion");
		vfxDoc.reset();
		
		//Sample Creation ----------------------------
		sampleGatherer = new GathererHero(pos, collider, walkLeft, walkLeftUp,
			walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightUp, idleRightDown, idleLeft,
			idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp, punchRightDown, punchRight, skill1Right,
			skill1RightUp, skill1RightDown, skill1Left, skill1LeftUp, skill1LeftDown,
			level, maxHP, currentHP, recoveryHP, maxEnergy, maxEnergy, recoveryE, atkDmg, atkSpd, atkRange,
			movSpd, visTiles, skill1ExecTime, skill2ExecTime, skill3ExecTime, skill1RecovTime, skill2RecovTime, skill3RecovTime,
			skill1Dmg, skill1ID, skill1Type, skill1Target, vfxExplosion);

		ret = true;
		break;


	}

	return ret;
}

bool ModuleEntityManager::LoadSampleEnemy(pugi::xml_node& enemyNode)
{
	bool ret = true;
	//collider
	SDL_Rect r;
	r.x = enemyNode.child("sample").child("collider").child("rect").attribute("x").as_int(0);
	r.y = enemyNode.child("sample").child("collider").child("rect").attribute("y").as_int(0);
	r.w = enemyNode.child("sample").child("collider").child("rect").attribute("w").as_int(0);
	r.h = enemyNode.child("sample").child("collider").child("rect").attribute("h").as_int(0);
	COLLIDER_TYPE cType = (COLLIDER_TYPE)enemyNode.child("sample").child("collider").child("type").attribute("id").as_int(0);
	Collider* enemyCollider = new Collider(r, cType, this);

	//stats
	fMPoint pos;
	pos.x = enemyNode.child("sample").child("position").attribute("x").as_float(0);
	pos.y = enemyNode.child("sample").child("position").attribute("y").as_float(0);

	int maxHP = enemyNode.child("sample").child("stats").child("hitPoints").attribute("max").as_int(0);
	int recoveryHP = enemyNode.child("sample").child("stats").child("hitPoints").attribute("recoveryRate").as_int(0);

	int atkDmg = enemyNode.child("sample").child("stats").child("attack").attribute("damage").as_int(0);
	float atkSpd = enemyNode.child("sample").child("stats").child("attack").attribute("speed").as_float(0);
	int atkRange = enemyNode.child("sample").child("stats").child("attack").attribute("range").as_int(0);

	int movSpd = enemyNode.child("sample").child("stats").attribute("movementSpeed").as_int(0);
	int vision = enemyNode.child("sample").child("stats").attribute("vision").as_int(0);
	int xp = enemyNode.child("sample").child("stats").attribute("xp").as_int(0);

	Animation enemyWalkLeft = enemyWalkLeft.PushAnimation(enemyNode, "wanamingoLeftWalk"); // looks good
	Animation enemyWalkLeftUp = enemyWalkLeftUp.PushAnimation(enemyNode, "wanamingoUpLeftWalk");// looks good
	Animation enemyWalkLeftDown = enemyWalkLeftDown.PushAnimation(enemyNode, "wanamingoDownLeftWalk"); // last frame teleports to the left
	Animation enemyWalkRightUp = enemyWalkRightUp.PushAnimation(enemyNode, "wanamingoUpRightWalk"); // looks good
	Animation enemyWalkRightDown = enemyWalkRightDown.PushAnimation(enemyNode, "wanamingoDownRightWalk"); // looks good
	Animation enemyWalkRight = enemyWalkRight.PushAnimation(enemyNode, "wanamingoRightWalk");// looks good

	Animation enemyIdleRight = enemyIdleRight.PushAnimation(enemyNode, "wanamingoRightIdle"); //goes up then bumps right
	Animation enemyIdleRightUp = enemyIdleRightUp.PushAnimation(enemyNode, "wanamingoUpRightIdle"); //bumps left
	Animation enemyIdleRightDown = enemyIdleRightDown.PushAnimation(enemyNode, "wanamingoDownRightIdle"); //bumps right
	Animation enemyIdleLeft = enemyIdleLeft.PushAnimation(enemyNode, "wanamingoLeftIdle"); //bumps left
	Animation enemyIdleLeftUp = enemyIdleLeftUp.PushAnimation(enemyNode, "wanamingoUpLeftIdle"); //bumps right
	Animation enemyIdleLeftDown = enemyIdleLeftDown.PushAnimation(enemyNode, "wanamingoDownLeftIdle"); //bumps right

	Animation enemyPunchRight = enemyPunchRight.PushAnimation(enemyNode, "wanamingoRightPunch"); //looks good
	Animation enemyPunchRightUp = enemyPunchRightUp.PushAnimation(enemyNode, "wanamingoUpRightPunch"); //jesus christ 
	Animation enemyPunchRightDown = enemyPunchRightDown.PushAnimation(enemyNode, "wanamingoDownRightPunch"); //goes back and forth
	Animation enemyPunchLeft = enemyPunchLeft.PushAnimation(enemyNode, "wanamingoLeftPunch"); //It should bump to the other side!
	Animation enemyPunchLeftUp = enemyPunchLeftUp.PushAnimation(enemyNode, "wanamingoUpLeftPunch"); //jesus christ 
	Animation enemyPunchLeftDown = enemyPunchLeftDown.PushAnimation(enemyNode, "wanamingoDownLeftPunch"); //jesus christ 

	sampleEnemy = new Enemy(pos, ENTITY_TYPE::ENEMY, enemyCollider, enemyWalkLeft, enemyWalkLeftUp,
		enemyWalkLeftDown, enemyWalkRightUp, enemyWalkRightDown, enemyWalkRight, enemyIdleRight, enemyIdleRightUp, enemyIdleRightDown, enemyIdleLeft,
		enemyIdleLeftUp, enemyIdleLeftDown, enemyPunchLeft, enemyPunchLeftUp, enemyPunchLeftDown, enemyPunchRightUp, enemyPunchRightDown, enemyPunchRight,
		maxHP, maxHP, recoveryHP, vision, atkDmg, atkSpd, atkRange, movSpd, xp);

	return ret;
}
