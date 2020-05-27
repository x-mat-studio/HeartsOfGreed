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
#include "UIManager.h"
#include "QuestManager.h"

#include "DynamicEntity.h"
#include "GathererHero.h"
#include "MeleeHero.h"
#include "RangedHero.h"
#include "RoboHero.h"
#include "Enemy.h"
#include "RangedEnemy.h"
#include "GigaEnemy.h"
#include "NightEnemy.h"

#include "Spawner.h"
#include "Quest.h"

#include "Building.h"
#include "Base.h"
#include "Turret.h"
#include "Barricade.h"
#include "UpgradeCenter.h"

#include "ParticleSystem.h"
#include "Emitter.h"

#include "p2SString.h"
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
	snowball(nullptr),
	deco3Selected(nullptr),
	turretTexture(nullptr),
	barricadeTexture(nullptr),
	upgradeCenterPlayerTexture(nullptr),
	upgradeCenterPlayerSelectedTexture(nullptr),
	upgradeCenterEnemyTexture(nullptr),
	upgradeCenterEnemySelectedTexture(nullptr),
	enemyTexture(nullptr),
	explosionTexture(nullptr),
	targetedTexture(nullptr),
	selectedTexture(nullptr),
	roboTexture(nullptr),
	debugPathTexture(nullptr),

	sampleGatherer(nullptr),
	sampleMelee(nullptr),
	sampleEnemy(nullptr),
	sampleSpawner(nullptr),
	sampleBuilding(nullptr),
	sampleBase(nullptr),
	sampleTurret(nullptr),
	sampleBarricade(nullptr),
	sampleUpgradeCenter(nullptr),
	moveCommandTileRng(nullptr),
	moveCommandTileGath(nullptr),
	moveCommandTileMelee(nullptr),
	sampleRanged(nullptr),
	sampleRobo(nullptr),
	sampleEnemyGiga(nullptr),
	sampleEnemyRanged(nullptr),
	sampleEnemyNight(nullptr),
	enemyNightTexture(nullptr),
	enemyGigaTexture(nullptr),
	enemyRangedTexture(nullptr),
	sampleEmitter(nullptr),
	sampleEmitter2(nullptr),
	sampleEmitter3(nullptr),
	sampleEmitter4(nullptr),
	sampleParticleSystem(nullptr),
	deadGatherer(nullptr),
	deadMelee(nullptr),
	deadRanged(nullptr),
	deadRobo(nullptr),
	moveCommandTileRobot(nullptr),

	wanamingoRoar2(-1),
	wanamingoRoar(-1),
	wanamingoGetsHit(-1),
	wanamingoDies2(-1),
	wanamingoDies(-1),
	turretShooting(-1),
	suitmanGetsHit(-1),
	suitmanGetsHit2(-1),
	suitmanGetsDeath(-1),
	suitmanGetsDeath2(-1),
	rangedGetsHit(-1),
	rangedDies(-1),
	noise1Armored(-1),
	noise2Armored(-1),
	noise3Armored(-1),
	noise4Armored(-1),
	noise1Ranged(-1),
	noise2Ranged(-1),
	noise3Ranged(-1),
	noise4Ranged(-1),
	noise1Suitman(-1),
	noise2Suitman(-1),
	noise3Suitman(-1),
	noise4Suitman(-1),
	moveHero(0),
	lvlup(-1),
	selectHero(-1),
	suitman1Skill(0),
	suitman1Skill2(0),
	buildingGetsHit(-1),
	buildingGetsHit2(-1),
	armored1Skill2(-1),
	ranged1Skill(-1),
	ranged1Skil2(-1),

	gathererLifeUpgradeValue(1),
	gathererDamageUpgradeValue(1),
	gathererEnergyUpgradeValue(1),
	gathererAtkSpeedUpgradeValue(1),
	meleeLifeUpgradeValue(1),
	meleeDamageUpgradeValue(1),
	meleeEnergyUpgradeValue(1),
	meleeAtkSpeedUpgradeValue(1),
	rangedLifeUpgradeValue(1),
	rangedDamageUpgradeValue(1),
	rangedEnergyUpgradeValue(1),
	rangedAtkSpeedUpgradeValue(1),
	robottoLifeUpgradeValue(1),
	robottoDamageUpgradeValue(1),
	robottoEnergyUpgradeValue(1),
	robottoAtkSpeedUpgradeValue(1),

	upgradeValue(1.1f),
	skillFileName()

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

	// Skills filename-----
	skillFileName = config.child("load").attribute("docnameSkillAreas").as_string();

	// Sample Hero Gatherer---------------------

	suitmanFileName = config.child("load").attribute("docnameSuitman").as_string();
	pugi::xml_document suitmandoc;
	suitmandoc.load_file(suitmanFileName.GetString());
	pugi::xml_node suitman = suitmandoc.child("suitman");

	LoadSampleHero(ENTITY_TYPE::HERO_GATHERER, suitman, config);

	suitmandoc.reset();

	// Sample Hero Melee---------------------
	armoredFileName = config.child("load").attribute("docnameArmoredman").as_string();
	pugi::xml_document armoredmanDoc;
	armoredmanDoc.load_file(armoredFileName.GetString());
	pugi::xml_node armoredman = armoredmanDoc.child("armoredman");

	LoadSampleHero(ENTITY_TYPE::HERO_MELEE, armoredman, config);
	armoredmanDoc.reset();


	// Sample Ranged Hero---------------------
	rangedFileName = config.child("load").attribute("docnameRangedman").as_string();
	pugi::xml_document rangedmanDoc;
	rangedmanDoc.load_file(rangedFileName.GetString());
	pugi::xml_node rangedman = rangedmanDoc.child("rangedman");

	LoadSampleHero(ENTITY_TYPE::HERO_RANGED, rangedman, config);
	rangedmanDoc.reset();


	// Sample Robo Hero---------------------
	roboFileName = config.child("load").attribute("docnameRobotto").as_string();
	pugi::xml_document robottoDoc;
	robottoDoc.load_file(roboFileName.GetString());
	pugi::xml_node robotto = robottoDoc.child("robotto");

	LoadSampleHero(ENTITY_TYPE::HERO_ROBO, robotto, config);
	robottoDoc.reset();


	// Sample Enemy---------------------
	P2SString filename = config.child("load").attribute("docnameWanamingo").as_string();
	pugi::xml_document wanamingodoc;
	wanamingodoc.load_file(filename.GetString());
	pugi::xml_node wanamingo = wanamingodoc.child("wanamingo").child("sample");

	LoadSampleEnemy(wanamingo, ENTITY_TYPE::ENEMY);

	wanamingo = wanamingodoc.child("wanamingo").child("sampleRanged");

	LoadSampleEnemy(wanamingo, ENTITY_TYPE::ENEMY_RANGED);

	wanamingo = wanamingodoc.child("wanamingo").child("sampleSpeed");

	LoadSampleEnemy(wanamingo, ENTITY_TYPE::ENEMY_NIGHT);

	wanamingo = wanamingodoc.child("wanamingo").child("sampleGiga");

	LoadSampleEnemy(wanamingo, ENTITY_TYPE::ENEMY_GIGA);

	wanamingodoc.reset();

	// Sample Turret ---------------------
	filename = config.child("load").attribute("docnameTurret").as_string();
	pugi::xml_document turretdoc;
	turretdoc.load_file(filename.GetString());
	pugi::xml_node turret = turretdoc.child("turret");

	LoadSampleTurret(turret);
	turretdoc.reset();


	//Sample Barricade -------------------
	filename = config.child("load").attribute("docnameBarricade").as_string();
	pugi::xml_document barricadedoc;
	barricadedoc.load_file(filename.GetString());
	pugi::xml_node barricadeNode = barricadedoc.child("barricade");

	LoadSampleBarricade(barricadeNode);
	barricadedoc.reset();


	//Sample Upgrade center --------------
	filename = config.child("load").attribute("docnameUpgradeCenter").as_string();
	pugi::xml_document upgradeCenterdoc;
	upgradeCenterdoc.load_file(filename.GetString());
	pugi::xml_node upgradeCenterNode = upgradeCenterdoc.child("upgradeCenter");

	LoadSampleUpgradeCenter(upgradeCenterNode);
	upgradeCenterdoc.reset();


	//spawner
	filename = config.child("load").attribute("docnameSpawner").as_string();
	pugi::xml_document spawnerdoc;
	spawnerdoc.load_file(filename.GetString());
	pugi::xml_node spawner = spawnerdoc.child("spawner");

	LoadSampleSpawner(spawner);
	spawnerdoc.reset();

	//Building & Base templates
	filename = config.child("load").attribute("docnameBuildings").as_string();
	pugi::xml_document buildingsdoc;
	buildingsdoc.load_file(filename.GetString());
	pugi::xml_node buildings = buildingsdoc.child("buildings");

	pugi::xml_node building = buildings.child("building");
	pugi::xml_node base = buildings.child("base");

	LoadSampleBuilding(building);
	LoadSampleBase(base);

	buildingsdoc.reset();

	//ParticleSystems and emitters
	filename = config.child("load").attribute("docnameParticleSystems").as_string();
	pugi::xml_document particleSystemsdoc;
	particleSystemsdoc.load_file(filename.GetString());
	pugi::xml_node particleSystem = particleSystemsdoc.child("particle_systems");

	LoadSampleParticleSystemsAndEmitters(particleSystem);

	particleSystemsdoc.reset();

	//Generate Areas------------------------------------
	filename = config.child("load").attribute("docnameSkillAreas").as_string();
	pugi::xml_document skillAreassdoc;
	skillAreassdoc.load_file(filename.GetString());
	pugi::xml_node skillAreasNode = skillAreassdoc.child("skillAreas");

	LoadSkillAreas(skillAreasNode);
	skillAreassdoc.reset();

	return ret;
}



bool ModuleEntityManager::Start()
{
	bool ret = true;

	//Textures load-----


	//HEROES------
	suitManTexture = app->tex->Load("spritesheets/characters/suitmale.png");
	armorMaleTexture = app->tex->Load("spritesheets/characters/armormale.png");
	combatFemaleTexture = app->tex->Load("spritesheets/characters/combatfemale.png");
	roboTexture = app->tex->Load("spritesheets/characters/robotto.png");

	//ENEMIES--------
	enemyTexture = app->tex->Load("spritesheets/Enemies/WanamingoAlien.png");
	enemyRangedTexture = app->tex->Load("spritesheets/Enemies/Snipermingo.png");
	enemyGigaTexture = app->tex->Load("spritesheets/Enemies/Gigamingo.png");
	enemyNightTexture = app->tex->Load("spritesheets/Enemies/Speedomingo.png");

	//BUILDINGS--------
	buildingTexture = app->tex->Load("maps/base03.png");
	base1Texture = app->tex->Load("maps/base01.png");
	base2Texture = app->tex->Load("maps/base02.png");
	base2TextureSelected = app->tex->Load("maps/base02_selected.png");
	base2TextureEnemy = app->tex->Load("maps/base02_enemy.png");
	base2TextureSelectedEnemy = app->tex->Load("maps/base02_enemy_selected.png");

	turretTexture = app->tex->Load("spritesheets/Structures/turretSpritesheet.png");
	barricadeTexture = app->tex->Load("spritesheets/Structures/barricade.png");

	upgradeCenterPlayerTexture = app->tex->Load("maps/upgradeCenter.png");
	upgradeCenterPlayerSelectedTexture = app->tex->Load("maps/upgradeCenter_selected.png");
	upgradeCenterEnemyTexture = app->tex->Load("maps/upgradeCenter_enemy.png");
	upgradeCenterEnemySelectedTexture = app->tex->Load("maps/upgradeCenter_enemy_selected.png");

	sampleBuilding->SetTexture(base1Texture);
	sampleBase->SetTexture(base2Texture);
	sampleTurret->SetTexture(turretTexture);
	sampleBarricade->SetTexture(barricadeTexture);
	sampleUpgradeCenter->SetTexture(upgradeCenterPlayerTexture);


	//SELECTIONS & FEEDBACK---------
	deco3Selected = app->tex->Load("maps/base03_selected.png");
	selectedTexture = app->tex->Load("spritesheets/VFX/selected.png");
	targetedTexture = app->tex->Load("spritesheets/VFX/target.png");
	explosionTexture = app->tex->Load("spritesheets/VFX/explosion.png");
	moveCommandTileRng = app->tex->Load("spritesheets/VFX/OnMyWayRanged.png");
	moveCommandTileGath = app->tex->Load("spritesheets/VFX/OnMyWaySuit.png");
	moveCommandTileMelee = app->tex->Load("spritesheets/VFX/OnMyWayMelee.png");
	moveCommandTileRobot = app->tex->Load("spritesheets/VFX/OnMyWatRobotto.png");
	debugPathTexture = app->tex->Load("maps/path.png");


	app->eventManager->EventRegister(EVENT_ENUM::DAY_START, this);
	app->eventManager->EventRegister(EVENT_ENUM::NIGHT_START, this);

	app->eventManager->EventRegister(EVENT_ENUM::ENTITY_DEAD, this);
	app->eventManager->EventRegister(EVENT_ENUM::PLAYER_CONQUERED_A_BASE, this);

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

	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_RESURRECT, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_RESURRECT, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_RESURRECT, this);
	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_RESURRECT, this);

	app->eventManager->EventRegister(EVENT_ENUM::SPAWN_ENEMY_RANGED, this);
	app->eventManager->EventRegister(EVENT_ENUM::SPAWN_ENEMY_GIGA, this);
	app->eventManager->EventRegister(EVENT_ENUM::SPAWN_ENEMY_NIGHT, this);

	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_LIFE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_DAMAGE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_ENERGY_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::GATHERER_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_LIFE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_DAMAGE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_ENERGY_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::MELEE_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_LIFE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_DAMAGE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_ENERGY_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::RANGED_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_LIFE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_DAMAGE_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_ENERGY_UPGRADE, this);
	app->eventManager->EventRegister(EVENT_ENUM::ROBOTTO_ATTACK_SPEED_UPGRADE, this);



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

	//Ranged sfx--------
	noise1Ranged = app->audio->LoadFx("audio/sfx/Heroes/Ranged/noise1.wav");
	noise2Ranged = app->audio->LoadFx("audio/sfx/Heroes/Ranged/noise2.wav");
	noise3Ranged = app->audio->LoadFx("audio/sfx/Heroes/Ranged/noise3.wav");
	noise4Ranged = app->audio->LoadFx("audio/sfx/Heroes/Ranged/noise4.wav");

	rangedGetsHit = app->audio->LoadFx("audio/sfx/Heroes/Ranged/rng_getsHit.wav");
	rangedDies = app->audio->LoadFx("audio/sfx/Heroes/Ranged/rng_dies.wav");

	ranged1Skill = app->audio->LoadFx("audio/sfx/Heroes/Ranged/skill1_launch.wav");
	ranged1Skil2 = app->audio->LoadFx("audio/sfx/Heroes/Ranged/skill1_cast.wav");

	//Robotto sfx-----------
	roboDying = app->audio->LoadFx("audio/sfx/Heroes/Robotto/provisionalDie.wav"); //Provisional SFX

	//General hero sfx--------
	lvlup = app->audio->LoadFx("audio/sfx/Heroes/lvlup.wav");
	selectHero = app->audio->LoadFx("audio/sfx/Heroes/heroSelect.wav");
	moveHero = app->audio->LoadFx("audio/sfx/Heroes/heroMove.wav");


	//Emitters and particles systems------------

	snowball = app->tex->Load("spritesheets/Particles/snowball.png");

	sampleEmitter->SetTextureNStart(snowball);  sampleParticleSystem->PushEmiter(*sampleEmitter);
	sampleEmitter2->SetTextureNStart(snowball); sampleParticleSystem->PushEmiter(*sampleEmitter2);
	sampleEmitter3->SetTextureNStart(snowball); sampleParticleSystem->PushEmiter(*sampleEmitter3);
	sampleEmitter4->SetTextureNStart(snowball); sampleParticleSystem->PushEmiter(*sampleEmitter4);


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

			case ENTITY_TYPE::HERO_MELEE:
				entityVector[i]->Start(armorMaleTexture);
				app->uiManager->AddPortrait((Hero*)entityVector[i]);

				entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::HERO, entityVector[i]->GetCenter());
				break;

			case ENTITY_TYPE::HERO_RANGED:
				entityVector[i]->Start(combatFemaleTexture);
				app->uiManager->AddPortrait((Hero*)entityVector[i]);

				entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::HERO, entityVector[i]->GetCenter());
				break;

			case ENTITY_TYPE::HERO_GATHERER:
				entityVector[i]->Start(suitManTexture);
				app->uiManager->AddPortrait((Hero*)entityVector[i]);

				entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::HERO, entityVector[i]->GetCenter());
				break;

			case ENTITY_TYPE::HERO_ROBO:
				entityVector[i]->Start(roboTexture);
				app->uiManager->AddPortrait((Hero*)entityVector[i]);

				entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::HERO, entityVector[i]->GetCenter());
				break;

			case ENTITY_TYPE::ENEMY:
				entityVector[i]->Start(enemyTexture);

				entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::ENEMY, entityVector[i]->GetCenter());
				break;

			case ENTITY_TYPE::ENEMY_NIGHT:
				entityVector[i]->Start(enemyNightTexture);

				entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::ENEMY, entityVector[i]->GetCenter());
				break;

			case ENTITY_TYPE::ENEMY_RANGED:
				entityVector[i]->Start(enemyRangedTexture);

				entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::ENEMY, entityVector[i]->GetCenter());
				break;

			case ENTITY_TYPE::ENEMY_GIGA:
				entityVector[i]->Start(enemyGigaTexture);

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

				UpgradeCenter* upCenter;
				upCenter = (UpgradeCenter*)entityVector[i];

				alignement = entityVector[i]->GetAlignment();

				if (alignement == ENTITY_ALIGNEMENT::PLAYER)
				{
					entityVector[i]->Start(upgradeCenterPlayerTexture);
					upCenter->selectedTexture = upgradeCenterPlayerSelectedTexture;
				}

				else if (alignement == ENTITY_ALIGNEMENT::ENEMY)
				{
					entityVector[i]->Start(upgradeCenterEnemyTexture);
					upCenter->selectedTexture = upgradeCenterEnemySelectedTexture;
				}
				break;


			case ENTITY_TYPE::BLDG_BASE:

				Base* auxBase;
				auxBase = (Base*)entityVector[i];

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
					entityVector[i]->minimapIcon = app->minimap->CreateIcon(&entityVector[i]->position, MINIMAP_ICONS::ENEMY_BASE, entityVector[i]->GetCenter());

					auxBase->selectedTexture = base2TextureSelectedEnemy;
				}

				break;

			case ENTITY_TYPE::BLDG_BARRICADE:
				entityVector[i]->Start(barricadeTexture);
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
	BROFILER_CATEGORY("Entity Manager Post Update", Profiler::Color::Blue);

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
	DeleteAllDeadHeroes();

	for (std::map<int, skillArea> ::iterator it = skillAreas.begin(); it != skillAreas.end(); it++)
	{
		delete it->second.area;
		it->second.area = nullptr;

	}
	skillAreas.clear();
	renderVector.clear();
	movableEntityVector.clear();
	buildingVector.clear();
	selectedVector.clear();


	//Heros-----
	app->tex->UnLoad(suitManTexture);				suitManTexture = nullptr;
	app->tex->UnLoad(armorMaleTexture);				armorMaleTexture = nullptr;
	app->tex->UnLoad(combatFemaleTexture);			combatFemaleTexture = nullptr;
	app->tex->UnLoad(roboTexture);					roboTexture = nullptr;

	RELEASE(sampleGatherer);						sampleGatherer = nullptr;
	RELEASE(sampleMelee);							sampleMelee = nullptr;
	RELEASE(sampleRanged);							sampleRanged = nullptr;
	RELEASE(sampleRobo);							sampleRobo = nullptr;

	//Enemies-------
	app->tex->UnLoad(enemyTexture);					enemyTexture = nullptr;
	app->tex->UnLoad(enemyNightTexture);			enemyNightTexture = nullptr;
	app->tex->UnLoad(enemyGigaTexture);				enemyGigaTexture = nullptr;
	app->tex->UnLoad(enemyRangedTexture);			enemyRangedTexture = nullptr;

	RELEASE(sampleEnemy);							sampleEnemy = nullptr;
	RELEASE(sampleEnemyNight);						sampleEnemyNight = nullptr;
	RELEASE(sampleEnemyGiga);						sampleEnemyGiga = nullptr;
	RELEASE(sampleEnemyRanged);						sampleEnemyRanged = nullptr;

	//Spawners----------
	RELEASE(sampleSpawner);							sampleSpawner = nullptr;

	//Buildings----------
	app->tex->UnLoad(buildingTexture);						buildingTexture = nullptr;
	app->tex->UnLoad(base1Texture);							base1Texture = nullptr;
	app->tex->UnLoad(base2Texture);							base2Texture = nullptr;
	app->tex->UnLoad(base2TextureEnemy);					base2TextureEnemy = nullptr;
	app->tex->UnLoad(base2TextureSelected);					base2TextureSelected = nullptr;
	app->tex->UnLoad(base2TextureSelectedEnemy);			base2TextureSelectedEnemy = nullptr;
	app->tex->UnLoad(turretTexture);						turretTexture = nullptr;
	app->tex->UnLoad(barricadeTexture);						barricadeTexture = nullptr;
	app->tex->UnLoad(upgradeCenterPlayerTexture);			upgradeCenterPlayerTexture = nullptr;
	app->tex->UnLoad(upgradeCenterPlayerSelectedTexture);	upgradeCenterPlayerSelectedTexture = nullptr;
	app->tex->UnLoad(upgradeCenterEnemyTexture);			upgradeCenterEnemyTexture = nullptr;
	app->tex->UnLoad(upgradeCenterEnemySelectedTexture);	upgradeCenterEnemySelectedTexture = nullptr;


	RELEASE(sampleBuilding);						sampleBuilding = nullptr;
	RELEASE(sampleBase);							sampleBase = nullptr;
	RELEASE(sampleTurret);							sampleTurret = nullptr;
	RELEASE(sampleBarricade);						sampleBarricade = nullptr;
	RELEASE(sampleUpgradeCenter);					sampleUpgradeCenter = nullptr;


	//Feedback------------
	app->tex->UnLoad(deco3Selected);				deco3Selected = nullptr;
	app->tex->UnLoad(debugPathTexture);				debugPathTexture = nullptr;
	app->tex->UnLoad(selectedTexture);				selectedTexture = nullptr;
	app->tex->UnLoad(explosionTexture);				explosionTexture = nullptr;
	app->tex->UnLoad(targetedTexture);				targetedTexture = nullptr;
	app->tex->UnLoad(moveCommandTileRng);			moveCommandTileRng = nullptr;
	app->tex->UnLoad(moveCommandTileGath);			moveCommandTileGath = nullptr;
	app->tex->UnLoad(moveCommandTileMelee);			moveCommandTileMelee = nullptr;

	//Particles---------
	app->tex->UnLoad(snowball);						snowball = nullptr;

	RELEASE(sampleEmitter);							sampleEmitter = nullptr;
	RELEASE(sampleEmitter2);						sampleEmitter2 = nullptr;
	RELEASE(sampleEmitter3);						sampleEmitter3 = nullptr;
	RELEASE(sampleEmitter4);						sampleEmitter4 = nullptr;
	RELEASE(sampleParticleSystem);					sampleParticleSystem = nullptr;

	RELEASE(sampleEnemy);							sampleEnemy = nullptr;
	RELEASE(sampleSpawner);							sampleSpawner = nullptr;

	RELEASE(sampleBuilding);						sampleBuilding = nullptr;
	RELEASE(sampleBase);							sampleBase = nullptr;
	RELEASE(sampleTurret);							sampleTurret = nullptr;

	//Events-----------
	app->eventManager->EventUnRegister(EVENT_ENUM::DAY_START, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::NIGHT_START, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::ENTITY_DEAD, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::PLAYER_CONQUERED_A_BASE, this);

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

	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_RESURRECT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_RESURRECT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_RESURRECT, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_RESURRECT, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::SPAWN_ENEMY_RANGED, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SPAWN_ENEMY_GIGA, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::SPAWN_ENEMY_NIGHT, this);

	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_LIFE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_DAMAGE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_ENERGY_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::GATHERER_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_LIFE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_DAMAGE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_ENERGY_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::MELEE_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_LIFE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_DAMAGE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_ENERGY_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::RANGED_ATTACK_SPEED_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_LIFE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_DAMAGE_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_ENERGY_UPGRADE, this);
	app->eventManager->EventUnRegister(EVENT_ENUM::ROBOTTO_ATTACK_SPEED_UPGRADE, this);
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
	case ENTITY_TYPE::QUEST:
		ret = new Quest(x, y);

		break;
	case ENTITY_TYPE::SPAWNER:
		ret = new Spawner({ (float)x,(float)y }, sampleSpawner);
		app->ai->PushSpawner((Spawner*)ret);
		break;

	case ENTITY_TYPE::PARTICLE_SYSTEM:
		assert("Not here, call add particle system");
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

	case ENTITY_TYPE::HERO_ROBO:
		ret = new RoboHero({ (float)x,(float)y }, sampleRobo, ENTITY_ALIGNEMENT::PLAYER);
		break;

	case ENTITY_TYPE::BUILDING:
		ret = new Building({ (float)x,(float)y }, sampleBuilding, alignement);
		break;

	case ENTITY_TYPE::BLDG_TURRET:
		ret = new Turret({ (float)x,(float)y }, sampleTurret, alignement);
		break;

	case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
		ret = new UpgradeCenter({ (float)x,(float)y }, sampleUpgradeCenter, alignement);
		break;

	case ENTITY_TYPE::BLDG_BASE:
		ret = new Base({ (float)x,(float)y }, sampleBase, alignement);
		app->ai->PushBase((Base*)ret);
		break;

	case ENTITY_TYPE::BLDG_BARRICADE:
		ret = new Barricade({ (float)x,(float)y }, sampleBarricade, alignement);
		break;

	case ENTITY_TYPE::ENEMY:
		ret = new Enemy({ (float)x,(float)y }, sampleEnemy, ENTITY_ALIGNEMENT::ENEMY);
		break;

	case ENTITY_TYPE::ENEMY_RANGED:
		ret = new RangedEnemy({ (float)x,(float)y }, sampleEnemyRanged, ENTITY_ALIGNEMENT::ENEMY);
		break;
	case ENTITY_TYPE::ENEMY_GIGA:
		ret = new Enemy({ (float)x,(float)y }, sampleEnemyGiga, ENTITY_ALIGNEMENT::ENEMY);
		break;
	case ENTITY_TYPE::ENEMY_NIGHT:
		ret = new Enemy({ (float)x,(float)y }, sampleEnemyNight, ENTITY_ALIGNEMENT::ENEMY);
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


Entity* ModuleEntityManager::AddDecorativeBuilding(BUILDING_DECOR decor, int x, int y)
{
	Entity* ret = nullptr;
	Building* building = nullptr;

	switch (decor)
	{
	case BUILDING_DECOR::ST_01:
		ret = building = new Building({ (float)x,(float)y }, sampleBuilding, ENTITY_ALIGNEMENT::NEUTRAL);
		building->myDecor = decor;
		break;

	case BUILDING_DECOR::ST_02:
		ret = building = new Building({ (float)x,(float)y }, sampleBuilding, ENTITY_ALIGNEMENT::NEUTRAL);
		building->myDecor = decor;
		break;

	case BUILDING_DECOR::ST_03:
		ret = building = new Building({ (float)x,(float)y }, sampleBuilding, ENTITY_ALIGNEMENT::NEUTRAL);
		building->myDecor = decor;
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


Entity* ModuleEntityManager::AddParticleSystem(TYPE_PARTICLE_SYSTEM type, int x, int y)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case TYPE_PARTICLE_SYSTEM::MAX:
		ret = new ParticleSystem(x, y, sampleParticleSystem, true);
		break;

	case TYPE_PARTICLE_SYSTEM::NONE:
		LOG(" Tried to add particle system, no type was found");
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

	case ENTITY_TYPE::HERO_ROBO:
		return sampleRobo;
		break;

	case ENTITY_TYPE::ENEMY:
		return sampleEnemy;
		break;

	case ENTITY_TYPE::ENEMY_RANGED:
		return sampleEnemyRanged;
		break;

	case ENTITY_TYPE::ENEMY_GIGA:
		return sampleEnemyGiga;
		break;

	case ENTITY_TYPE::ENEMY_NIGHT:
		return sampleEnemyNight;
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
Entity* ModuleEntityManager::CheckEntityOnClick(iMPoint mousePos, bool focus, ENTITY_ALIGNEMENT alignement)
{
	Entity* ret = nullptr;

	Collider* col = nullptr;
	ENTITY_TYPE type;
	ENTITY_ALIGNEMENT align;

	int numEntities = entityVector.size();

	for (int i = 0; i < numEntities; i++)
	{
		type = entityVector[i]->GetType();
		col = entityVector[i]->GetCollider();
		align = entityVector[i]->GetAlignment();

		if (col == nullptr)
			continue;

		//dynamic entities get priority over static entities
		if (mousePos.PointInRect(&col->rect))
		{
			if (col != nullptr && (align == alignement))
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

Entity* ModuleEntityManager::CheckEntityOnClickbyPriority(iMPoint mousePos)
{
	Entity* ret = nullptr;

	Collider* col = nullptr;
	ENTITY_TYPE type;
	ENTITY_ALIGNEMENT align;

	int numEntities = entityVector.size();

	for (int j = 0; j < numEntities; j++)
	{
		entityVector[j]->selectedByPlayer = false;
	}

	for (int k = (int)ENTITY_TYPE::UNKNOWN + 1; k != (int)ENTITY_TYPE::MAX_TYPE; k++)
	{
		for (int i = 0; i < numEntities; i++)
		{



			type = entityVector[i]->GetType();
			col = entityVector[i]->GetCollider();
			align = entityVector[i]->GetAlignment();

			if (col == nullptr || k != (int)type)
				continue;

			//dynamic entities get priority over static entities
			if (mousePos.PointInRect(&col->rect))
			{
				if (col != nullptr)
				{

					entityVector[i]->selectedByPlayer = true;


					return entityVector[i];
				}

				ret = entityVector[i];
			}
			else
			{
				entityVector[i]->selectedByPlayer = false;
			}
		}
	}

	if (ret != nullptr)
	{
		ret->selectedByPlayer = true;
	}

	return ret;
}

Entity* ModuleEntityManager::CheckEntityOnClickbyPriorityandAlignment(iMPoint mousePos, bool focus, ENTITY_ALIGNEMENT alignement)
{

	Entity* ret = nullptr;

	Collider* col = nullptr;
	ENTITY_TYPE type;
	ENTITY_ALIGNEMENT align;

	int numEntities = entityVector.size();

	for (int k = (int)ENTITY_TYPE::UNKNOWN + 1; k != (int)ENTITY_TYPE::MAX_TYPE; k++)
	{
		for (int i = 0; i < numEntities; i++)
		{
			type = entityVector[i]->GetType();
			col = entityVector[i]->GetCollider();
			align = entityVector[i]->GetAlignment();

			if (col == nullptr || k != (int)type)
				continue;

			//dynamic entities get priority over static entities
			if (mousePos.PointInRect(&col->rect))
			{
				if (col != nullptr && (align == alignement))
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

		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_RANGED || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_ROBO)
		{
			col = entityVector[i]->GetCollider();

			Hero* thisHero;
			thisHero = (Hero*)entityVector[i];

			if (col != nullptr)
			{
				if (col->CheckCollision(selection))
				{

					if (!thisHero->selectedByPlayer)
					{
						thisHero->selectedByPlayer = true;
						app->audio->PlayFx(selectHero, 0, -1);
					}

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

	for (int i = 0; i < numEntities; i++)
	{
		entityVector[i]->CheckObjective(entity);
	}
}


//Note that a hero is dead only if it has been alive first, so this function doesn't include heroes not found yet
bool ModuleEntityManager::CheckIfHeroIsDead(ENTITY_TYPE heroType) const
{
	switch (heroType)
	{
	case ENTITY_TYPE::HERO_MELEE:
		if (deadMelee != nullptr)
		{
			return true;
		}
		break;
	case ENTITY_TYPE::HERO_RANGED:
		if (deadRanged != nullptr)
		{
			return true;
		}
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		if (deadGatherer != nullptr)
		{
			return true;
		}
		break;
	case ENTITY_TYPE::HERO_ROBO:
		if (deadRobo != nullptr)
		{
			return true;
		}
		break;
	}
	return false;
}


//Note that this function erases what was inside the target variable (dead hero) before creating a new one to prevent 
//memory leaks when Saving/Loading and creating more than 1 hero of type. To check if a certain hero is dead use the CheckIfHeroIsDead() function
DeadHero* ModuleEntityManager::AssignNewDeadHero(Hero& dyingHero)
{
	ENTITY_TYPE heroType = dyingHero.GetType();
	DeadHero** refhero;

	//assinging the correct dead hero variable to refhero to make it easy to work with
	switch (heroType)
	{
	case ENTITY_TYPE::HERO_MELEE:
		refhero = &deadMelee;
		break;
	case ENTITY_TYPE::HERO_RANGED:
		refhero = &deadRanged;
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		refhero = &deadGatherer;
		break;
	case ENTITY_TYPE::HERO_ROBO:
		refhero = &deadRobo;
		break;

	default:
		return nullptr;
	}

	//deletes information about any hero stored in that variable
	if (*refhero != nullptr)
	{
		delete* refhero;
		*refhero = nullptr;
	}


	*refhero = new DeadHero(dyingHero.GetHeroLevel(), dyingHero.GetType(), dyingHero.GetSkill1(), dyingHero.GetPassiveSkill());

	return *refhero;
}

DeadHero* ModuleEntityManager::AssignNewDeadHero(int level, ENTITY_TYPE type, Skill& skill, Skill& passiveSkill)
{

	DeadHero** refhero;

	//assinging the correct dead hero variable to refhero to make it easy to work with
	switch (type)
	{
	case ENTITY_TYPE::HERO_MELEE:
		refhero = &deadMelee;
		break;
	case ENTITY_TYPE::HERO_RANGED:
		refhero = &deadRanged;
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		refhero = &deadGatherer;
		break;
	case ENTITY_TYPE::HERO_ROBO:
		refhero = &deadRobo;
		break;

	default:
		return nullptr;
	}

	//deletes information about any hero stored in that variable
	if (*refhero != nullptr)
	{
		delete* refhero;
		*refhero = nullptr;
	}


	*refhero = new DeadHero(level, type, skill, passiveSkill);

	return *refhero;
}


void ModuleEntityManager::DeleteDeadHero(ENTITY_TYPE heroType)
{
	switch (heroType)
	{
	case ENTITY_TYPE::HERO_MELEE:
		if (deadMelee != nullptr)
		{
			delete deadMelee;
			deadMelee = nullptr;
		}
		break;
	case ENTITY_TYPE::HERO_RANGED:
		if (deadRanged != nullptr)
		{
			delete deadRanged;
			deadRanged = nullptr;
		}
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		if (deadGatherer != nullptr)
		{
			delete deadGatherer;
			deadGatherer = nullptr;
		}
		break;
	case ENTITY_TYPE::HERO_ROBO:
		if (deadRobo != nullptr)
		{
			delete deadRobo;
			deadRobo = nullptr;
		}
		break;
	}

}


void ModuleEntityManager::DeleteAllDeadHeroes()
{

	if (deadMelee != nullptr)
	{
		delete deadMelee;
		deadMelee = nullptr;
	}

	if (deadRanged != nullptr)
	{
		delete deadRanged;
		deadRanged = nullptr;
	}

	if (deadGatherer != nullptr)
	{
		delete deadGatherer;
		deadGatherer = nullptr;
	}

	if (deadRobo != nullptr)
	{
		delete deadRobo;
		deadRobo = nullptr;
	}

}


void ModuleEntityManager::SaveDeadHero(pugi::xml_node& deadHeroesNode, ENTITY_TYPE heroType) const
{
	DeadHero* refhero = nullptr;

	switch (heroType)
	{
	case ENTITY_TYPE::HERO_MELEE:
		refhero = deadMelee;
		break;
	case ENTITY_TYPE::HERO_RANGED:
		refhero = deadRanged;
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		refhero = deadGatherer;
		break;
	case ENTITY_TYPE::HERO_ROBO:
		refhero = deadRobo;
		break;
	}

	if (refhero != nullptr)
	{
		pugi::xml_node statsnode = deadHeroesNode.append_child("stats");

		statsnode.append_attribute("level") = refhero->GetLevel();
		statsnode.append_attribute("type") = (int)heroType;

		SKILL_ID skillId;
		int skillLevel;
		refhero->GetSkillInfo(skillId, skillLevel);
		statsnode.append_attribute("skillId") = (int)skillId;
		statsnode.append_attribute("skillLvl") = skillLevel;

		refhero->GetPassiveSkillInfo(skillId, skillLevel);
		statsnode.append_attribute("passiveSkillId") = (int)skillId;
		statsnode.append_attribute("passiveSkillLvl") = skillLevel;
	}

}

void ModuleEntityManager::LoadDeadHero(pugi::xml_node& deadHeroesNode, ENTITY_TYPE heroType)
{
	DeadHero** refhero = nullptr;

	switch (heroType)
	{
	case ENTITY_TYPE::HERO_MELEE:
		refhero = &deadMelee;
		break;
	case ENTITY_TYPE::HERO_RANGED:
		refhero = &deadRanged;
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		refhero = &deadGatherer;
		break;
	case ENTITY_TYPE::HERO_ROBO:
		refhero = &deadRobo;
		break;
	}


	pugi::xml_node statsnode = deadHeroesNode.child("stats");

	int level = statsnode.attribute("level").as_int(-1);
	ENTITY_TYPE type = (ENTITY_TYPE)statsnode.attribute("type").as_int(-1);
	SKILL_ID skillId;
	int skillLevel;


	skillId = (SKILL_ID)statsnode.attribute("skillId").as_int(-1);
	skillLevel = statsnode.attribute("skillLvl").as_int(-1);

	Skill skill;
	skill.id = skillId;
	skill.lvl = skillLevel;


	skillId = (SKILL_ID)statsnode.attribute("passiveSkillId").as_int(-1);
	skillLevel = statsnode.attribute("passiveSkillLvl").as_int(-1);

	Skill passiveSkill;
	passiveSkill.id = skillId;
	passiveSkill.lvl = skillLevel;

	*refhero = AssignNewDeadHero(level, type, skill, passiveSkill);


	//Work in progress

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
			app->uiManager->CheckFocusEntity(entityVector[i]);
			app->questManager->CheckEntityDead(entityVector[i]);


			type = entityVector[i]->GetType();

			if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED || type == ENTITY_TYPE::HERO_ROBO)
			{
				app->uiManager->RemovePortrait((Hero*)entityVector[i]);
			}

			delete entityVector[i];
			entityVector[i] = nullptr;
			entityVector.erase(entityVector.begin() + i);

			i--;

			if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED || type == ENTITY_TYPE::HERO_ROBO)
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

		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED || type == ENTITY_TYPE::HERO_ROBO)
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

		if (type == ENTITY_TYPE::ENEMY || type == ENTITY_TYPE::ENEMY_GIGA || type == ENTITY_TYPE::ENEMY_RANGED || type == ENTITY_TYPE::ENEMY_NIGHT)
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
			case ENTITY_TYPE::ENEMY_RANGED:
			case ENTITY_TYPE::ENEMY_NIGHT:
			case ENTITY_TYPE::ENEMY_GIGA:
			case ENTITY_TYPE::QUEST:
			case ENTITY_TYPE::HERO_GATHERER:
			case ENTITY_TYPE::HERO_MELEE:
			case ENTITY_TYPE::HERO_RANGED:
			case ENTITY_TYPE::HERO_ROBO:

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
		if ((selectedVector[i]->GetType() == ENTITY_TYPE::HERO_GATHERER) || (selectedVector[i]->GetType() == ENTITY_TYPE::HERO_MELEE) || (selectedVector[i]->GetType() == ENTITY_TYPE::HERO_RANGED) || (selectedVector[i]->GetType() == ENTITY_TYPE::HERO_ROBO))
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

		if (selectedVector[i]->GetType() == ENTITY_TYPE::ENEMY || selectedVector[i]->GetType() == ENTITY_TYPE::ENEMY_RANGED
			|| selectedVector[i]->GetType() == ENTITY_TYPE::ENEMY_NIGHT || selectedVector[i]->GetType() == ENTITY_TYPE::ENEMY_GIGA)
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
			if (entityVector[i]->visionEntity != nullptr && entityVector[i]->visionEntity->isVisible == true)
			{
				entityVector[i]->MinimapDraw(scale, halfWidth);
			}
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

	case EVENT_ENUM::PLAYER_CONQUERED_A_BASE:
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

	case EVENT_ENUM::SPAWN_ENEMY_RANGED:

		AddEntity(ENTITY_TYPE::ENEMY_RANGED, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_ENEMY_NIGHT:

		AddEntity(ENTITY_TYPE::ENEMY_NIGHT, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_ENEMY_GIGA:

		AddEntity(ENTITY_TYPE::ENEMY_GIGA, pos.x, pos.y);
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

	case EVENT_ENUM::SPAWN_ROBO_HERO:

		AddEntity(ENTITY_TYPE::HERO_ROBO, pos.x, pos.y);
		break;

	case EVENT_ENUM::SPAWN_TURRET:

		AddEntity(ENTITY_TYPE::BLDG_TURRET, pos.x, pos.y);
		break;

	case EVENT_ENUM::GATHERER_RESURRECT:
		ReviveHero(*deadGatherer);
		DeleteDeadHero(ENTITY_TYPE::HERO_GATHERER);
		break;

	case EVENT_ENUM::MELEE_RESURRECT:
		ReviveHero(*deadMelee);
		DeleteDeadHero(ENTITY_TYPE::HERO_MELEE);
		break;

	case EVENT_ENUM::RANGED_RESURRECT:
		ReviveHero(*deadRanged);
		DeleteDeadHero(ENTITY_TYPE::HERO_RANGED);
		break;

	case EVENT_ENUM::ROBOTTO_RESURRECT:
		ReviveHero(*deadRobo);
		DeleteDeadHero(ENTITY_TYPE::HERO_ROBO);
		break;

	case EVENT_ENUM::GATHERER_LIFE_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_GATHERER)
			{
				int previousHealth = entityVector[i]->GetMaxHP();
				gathererLifeUpgradeValue *= upgradeValue;
				entityVector[i]->SetMaxHP(round(entityVector[i]->GetMaxHP() * upgradeValue));
				entityVector[i]->SetCurrentHP(entityVector[i]->GetCurrentHP() + entityVector[i]->GetMaxHP() - previousHealth);
				break;
			}
		}

		break;

	case EVENT_ENUM::GATHERER_DAMAGE_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_GATHERER)
			{
				gathererDamageUpgradeValue *= upgradeValue;
				Hero* hero = (Hero*)entityVector[i];
				hero->SetAttackDamage(hero->GetAttackDamage() * upgradeValue);
				break;
			}
		}

		break;

	case EVENT_ENUM::GATHERER_ENERGY_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_GATHERER)
			{
				Hero* hero = (Hero*)entityVector[i];
				int previousEnergy = hero->GetMaxEnergyPoints();
				gathererEnergyUpgradeValue *= upgradeValue;
				hero->SetMaxEnergyPoints(hero->GetMaxEnergyPoints() * upgradeValue);
				hero->AddEnergyPoints(hero->GetMaxEnergyPoints() - previousEnergy);
				break;
			}
		}

		break;

	case EVENT_ENUM::GATHERER_ATTACK_SPEED_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_GATHERER)
			{
				gathererAtkSpeedUpgradeValue *= upgradeValue;
				Hero* hero = (Hero*)entityVector[i];
				hero->SetAttackSpeed(hero->GetAttackSpeed() * upgradeValue);
				break;
			}
		}

		break;

	case EVENT_ENUM::RANGED_LIFE_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_RANGED)
			{
				int previousHealth = entityVector[i]->GetMaxHP();
				rangedLifeUpgradeValue *= upgradeValue;
				entityVector[i]->SetMaxHP(round(entityVector[i]->GetMaxHP() * upgradeValue));
				entityVector[i]->SetCurrentHP(entityVector[i]->GetCurrentHP() + entityVector[i]->GetMaxHP() - previousHealth);
				break;
			}
		}

		break;

	case EVENT_ENUM::RANGED_DAMAGE_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_RANGED)
			{
				rangedDamageUpgradeValue *= upgradeValue;
				Hero* hero = (Hero*)entityVector[i];
				hero->SetAttackDamage(hero->GetAttackDamage() * upgradeValue);
				break;
			}
		}

		break;

	case EVENT_ENUM::RANGED_ENERGY_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_RANGED)
			{
				Hero* hero = (Hero*)entityVector[i];
				int previousEnergy = hero->GetMaxEnergyPoints();
				rangedEnergyUpgradeValue *= upgradeValue;
				hero->SetMaxEnergyPoints(hero->GetMaxEnergyPoints() * upgradeValue);
				hero->AddEnergyPoints(hero->GetMaxEnergyPoints() - previousEnergy);
				break;
			}
		}

		break;

	case EVENT_ENUM::RANGED_ATTACK_SPEED_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_RANGED)
			{
				rangedAtkSpeedUpgradeValue *= upgradeValue;
				Hero* hero = (Hero*)entityVector[i];
				hero->SetAttackSpeed(hero->GetAttackSpeed() * upgradeValue);
				break;
			}
		}

		break;

	case EVENT_ENUM::MELEE_LIFE_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_MELEE)
			{
				int previousHealth = entityVector[i]->GetMaxHP();
				meleeLifeUpgradeValue *= upgradeValue;
				entityVector[i]->SetMaxHP(round(entityVector[i]->GetMaxHP() * upgradeValue));
				entityVector[i]->SetCurrentHP(entityVector[i]->GetCurrentHP() + entityVector[i]->GetMaxHP() - previousHealth);
				break;
			}
		}

		break;

	case EVENT_ENUM::MELEE_DAMAGE_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_MELEE)
			{
				meleeDamageUpgradeValue *= upgradeValue;
				Hero* hero = (Hero*)entityVector[i];
				hero->SetAttackDamage(hero->GetAttackDamage() * upgradeValue);
				break;
			}
		}

		break;

	case EVENT_ENUM::MELEE_ENERGY_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_MELEE)
			{
				Hero* hero = (Hero*)entityVector[i];
				int previousEnergy = hero->GetMaxEnergyPoints();
				meleeEnergyUpgradeValue *= upgradeValue;
				hero->SetMaxEnergyPoints(hero->GetMaxEnergyPoints() * upgradeValue);
				hero->AddEnergyPoints(hero->GetMaxEnergyPoints() - previousEnergy);
				break;
			}
		}

		break;

	case EVENT_ENUM::MELEE_ATTACK_SPEED_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_MELEE)
			{
				meleeAtkSpeedUpgradeValue *= upgradeValue;
				Hero* hero = (Hero*)entityVector[i];
				hero->SetAttackSpeed(hero->GetAttackSpeed() * upgradeValue);
				break;
			}
		}

		break;

	case EVENT_ENUM::ROBOTTO_LIFE_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_ROBO)
			{
				int previousHealth = entityVector[i]->GetMaxHP();
				robottoLifeUpgradeValue *= upgradeValue;
				entityVector[i]->SetMaxHP(round(entityVector[i]->GetMaxHP() * upgradeValue));
				entityVector[i]->SetCurrentHP(entityVector[i]->GetCurrentHP() + entityVector[i]->GetMaxHP() - previousHealth);
				break;
			}
		}

		break;

	case EVENT_ENUM::ROBOTTO_DAMAGE_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_ROBO)
			{
				robottoDamageUpgradeValue *= upgradeValue;
				Hero* hero = (Hero*)entityVector[i];
				hero->SetAttackDamage(hero->GetAttackDamage() * upgradeValue);
				break;
			}
		}

		break;

	case EVENT_ENUM::ROBOTTO_ENERGY_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_ROBO)
			{
				Hero* hero = (Hero*)entityVector[i];
				int previousEnergy = hero->GetMaxEnergyPoints();
				robottoEnergyUpgradeValue *= upgradeValue;
				hero->SetMaxEnergyPoints(hero->GetMaxEnergyPoints() * upgradeValue);
				hero->AddEnergyPoints(hero->GetMaxEnergyPoints() - previousEnergy);
				break;
			}
		}

		break;

	case EVENT_ENUM::ROBOTTO_ATTACK_SPEED_UPGRADE:
		for (int i = 0; i < entityVector.size(); i++)
		{
			if (entityVector[i]->GetType() == ENTITY_TYPE::HERO_ROBO)
			{
				robottoAtkSpeedUpgradeValue *= upgradeValue;
				Hero* hero = (Hero*)entityVector[i];
				hero->SetAttackSpeed(hero->GetAttackSpeed() * upgradeValue);
				break;
			}
		}

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
	switch (type)
	{
	case ENTITY_TYPE::BUILDING:

		sampleBuilding->ActivateTransparency();
		sampleBuilding->SetPosition(x, y);
		sampleBuilding->Draw(0);
		break;


	case ENTITY_TYPE::BLDG_TURRET:

		sampleTurret->ActivateTransparency();
		sampleTurret->SetPosition(x, y);
		sampleTurret->Draw(0.0000001);
		break;


	case ENTITY_TYPE::BLDG_UPGRADE_CENTER:
		sampleUpgradeCenter->ActivateTransparency();
		sampleUpgradeCenter->SetPosition(x, y);
		sampleUpgradeCenter->Draw(0.0000001);
		break;


	case ENTITY_TYPE::BLDG_BASE:

		sampleBase->ActivateTransparency();
		sampleBase->SetPosition(x, y);
		sampleBase->Draw(0);
		break;


	case ENTITY_TYPE::BLDG_BARRICADE:
		sampleBarricade->ActivateTransparency();
		sampleBarricade->SetPosition(x, y);
		sampleBarricade->Draw(0);

		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_STATE::KEY_DOWN)
		{
			sampleBarricade->Flip();
		}
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
		case ENTITY_TYPE::HERO_ROBO:
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

		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED || type == ENTITY_TYPE::HERO_ROBO)
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

		if (type == ENTITY_TYPE::HERO_GATHERER || type == ENTITY_TYPE::HERO_MELEE || type == ENTITY_TYPE::HERO_RANGED || type == ENTITY_TYPE::HERO_ROBO)
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
		if (entityVector[i]->GetType() == ENTITY_TYPE::ENEMY || entityVector[i]->GetType() == ENTITY_TYPE::ENEMY_RANGED
			|| entityVector[i]->GetType() == ENTITY_TYPE::ENEMY_NIGHT || entityVector[i]->GetType() == ENTITY_TYPE::ENEMY_GIGA)
		{
			enemy = (Enemy*)entityVector[i];
			enemy->Die();
		}
	}
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


skillArea* ModuleEntityManager::GenerateNewArea(int radius)
{
	skillArea* ret = nullptr;
	std::map<int, skillArea> ::iterator it;

	if (skillAreas.count(radius) > 0)
	{
		skillAreas.at(radius).users++;
	}
	else
	{
		skillArea newArea;

		newArea.area = BuildCircleArea(radius);
		newArea.users = 1;

		skillAreas.insert({ radius, newArea });

	}

	return &skillAreas.at(radius);
}

skillArea* ModuleEntityManager::RequestAreaInfo(int radius)
{
	if (skillAreas.count(radius) == 0)
	{
		GenerateNewArea(radius);
	}
	return &skillAreas.at(radius);

}

void ModuleEntityManager::CreateDynamicArea(std::vector <iMPoint>* toFill, int radius, iMPoint center, skillArea* skillArea)
{

	center = app->map->WorldToMap(round(center.x), round(center.y));
	toFill->clear();

	int diameter = (radius * 2) + 1;
	iMPoint posCheck = center - radius;

	if (skillArea == nullptr)
	{
		skillArea = RequestAreaInfo(radius);
	}

	if (skillArea != nullptr)
		for (int y = 0; y < diameter; y++)
		{
			for (int x = 0; x < diameter; x++)
			{
				if (skillArea->area[(y * diameter) + x] == 1 && app->pathfinding->IsWalkable(posCheck + iMPoint{ x, y }))
				{
					if (app->pathfinding->CheckBoundaries(posCheck + iMPoint{ x + 1, y }) && app->pathfinding->CheckBoundaries(posCheck + iMPoint{ x, y + 1 }))
						toFill->push_back(posCheck + iMPoint{ x + 1, y });
				}
			}
		}

}

bool ModuleEntityManager::RequestSkill(Skill& skillToFill, SKILL_ID id, int requestLvl)
{
	BROFILER_CATEGORY("Open Skill", Profiler::Color::Cornsilk);

	pugi::xml_document skillDoc;
	skillDoc.load_file(skillFileName.GetString());
	pugi::xml_node skills = skillDoc.child("skills");

	bool toRet(false);
	int currLvl = 1;

	for (pugi::xml_node iterator = skills.first_child(); iterator; iterator = iterator.next_sibling())
	{
		if (iterator.attribute("id").as_int(-1) == (int)id)
		{
			for (pugi::xml_node iterator2 = iterator.first_child(); iterator2; iterator2 = iterator.next_sibling())
			{
				if (currLvl == requestLvl)
				{
					skillToFill.attackRadius = iterator2.attribute("attackRadius").as_int(-1);
					skillToFill.coolDown = iterator2.attribute("cooldown").as_float(-1.f);
					skillToFill.dmg = iterator2.attribute("dmg").as_int(-1);
					skillToFill.effect = (SKILL_EFFECT)iterator2.attribute("effect").as_int(-1);
					skillToFill.executionTime = iterator2.attribute("executionTime").as_float(-1.f);
					skillToFill.hurtYourself = iterator2.attribute("hurtYourself").as_bool(false);
					skillToFill.lvl = currLvl;
					skillToFill.rangeRadius = iterator2.attribute("rangeRadius").as_int(-1);
					skillToFill.energyCost = iterator2.attribute("energyCost").as_int(-1);;


					skillToFill.type = (SKILL_TYPE)iterator.attribute("type").as_int(-1);
					skillToFill.target = (ENTITY_ALIGNEMENT)iterator.attribute("alignTarget").as_int(-1);
					skillToFill.id = (SKILL_ID)iterator.attribute("id").as_int();


					toRet = true;
					break;
				}

				currLvl++;
			}

			break;
		}

	}

	skillDoc.reset();

	return toRet;
}

bool ModuleEntityManager::RequestHeroStats(HeroStats& hero, ENTITY_TYPE id, int lvl)
{
	BROFILER_CATEGORY("Open Skill", Profiler::Color::Cornsilk);

	P2SString heroFileName;

	switch (id)
	{
	case ENTITY_TYPE::HERO_MELEE:
		heroFileName = armoredFileName;
		break;
	case ENTITY_TYPE::HERO_RANGED:
		heroFileName = rangedFileName;
		break;
	case ENTITY_TYPE::HERO_GATHERER:
		heroFileName = suitmanFileName;
		break;
	case ENTITY_TYPE::HERO_ROBO:
		heroFileName = roboFileName;
		break;
	default:
		break;
	}

	if (heroFileName.Length() == 0)
		return false;

	pugi::xml_document heroDoc;
	heroDoc.load_file(heroFileName.GetString());
	pugi::xml_node stats = heroDoc.first_child().child("sample").child("stats");

	bool toRet(false);
	int currLvl = 1;


	for (pugi::xml_node iterator = stats.first_child(); iterator; iterator = iterator.next_sibling())
	{
		if (currLvl == lvl)
		{
			//hero.SetMaxHP(iterator.attribute("hp").as_int(-1));
			//hero.SetAttackDamage(iterator.attribute("damage").as_int(-1));
			//hero.SetMaxEnergyPoints(iterator.attribute("energy").as_int(-1));
			//hero.SetAttackSpeed(iterator.attribute("atkSpeed").as_float(-1));
			//hero.SetRecoveryHitPointsRate(iterator.attribute("liferecoveryRate").as_float(-1));
			//hero.SetRecoveryEnergyRate(iterator.attribute("energyrecoveryRate").as_float(-1));
			//hero.SetHeroLevel(lvl);

			//hero.SetSpeed(stats.attribute("movementSpeed").as_int(-1));
			//hero.SetVisionDistance(stats.attribute("vision").as_int(-1));
			//hero.SetAttackRange(stats.attribute("attackRange").as_float(-1));

			hero.maxHP = (iterator.attribute("hp").as_int(-1));
			hero.currHP = hero.maxHP;
			hero.damage = (iterator.attribute("damage").as_int(-1));
			hero.maxEnergy = (iterator.attribute("energy").as_int(-1));
			hero.currEnergy = hero.maxEnergy;
			hero.atkSpeed = (iterator.attribute("atkSpeed").as_float(-1));
			hero.recoveryHPRate = (iterator.attribute("liferecoveryRate").as_float(-1));
			hero.recoveryEnergyRate = (iterator.attribute("energyrecoveryRate").as_float(-1));
			hero.heroLevel = (lvl);
			hero.xpToLvlUp = iterator.attribute("xpToLvlUp").as_int(-1);

			hero.movSpeed = (stats.attribute("movementSpeed").as_int(-1));
			hero.visionDistance = (stats.attribute("vision").as_int(-1));
			hero.attackRange = (stats.attribute("attackRange").as_float(-1));

			toRet = true;
			break;
		}

		currLvl++;
	}


	heroDoc.reset();

	return toRet;
}



bool ModuleEntityManager::ReviveHero(DeadHero heroToRevive)
{
	HeroStats newStats;

	Skill newSkill;
	SKILL_ID newSkillId;
	int newSkillLvl;

	Skill newPassiveSkill;
	SKILL_ID newPassiveSkillId;
	int newPassiveSkillLvl;

	//Requests the hero stats and skill to build a new hero
	RequestHeroStats(newStats, heroToRevive.GetType(), heroToRevive.GetLevel());
	heroToRevive.GetSkillInfo(newSkillId, newSkillLvl);
	RequestSkill(newSkill, newSkillId, newSkillLvl);

	heroToRevive.GetPassiveSkillInfo(newPassiveSkillId, newPassiveSkillLvl);
	RequestSkill(newPassiveSkill, newPassiveSkillId, newPassiveSkillLvl);

	Hero* newHero = nullptr;
	fMPoint spawnpoint = app->player->focusedEntity->position;
	newHero = (Hero*)AddEntity(heroToRevive.GetType(), spawnpoint.x, spawnpoint.y, ENTITY_ALIGNEMENT::PLAYER);

	if (newHero != nullptr)
	{
		newHero->ReplaceSkill1(newSkill);
		newHero->ReplacePassiveSkill(newPassiveSkill);
		newHero->ReplaceHeroStats(newStats);

		return true;
	}

	return false;
}


int ModuleEntityManager::ExecuteSkill(Skill& skill, iMPoint pivot, Entity* objective)
{
	int ret = -1;

	switch (skill.type)
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
		float newRad = sqrt(halfW * halfW + halfH * halfH) * skill.attackRadius + 0.5f * skill.attackRadius;

		for (int i = 0; i < numEntities; i++)
		{
			if (entityVector[i]->GetAlignment() != skill.target)
			{
				if (!skill.hurtYourself || entityVector[i] != objective)
				{
					continue;
				}
			}

			entColl = entityVector[i]->GetCollider();

			if (entColl == nullptr)
				continue;


			if (entColl->CheckCollisionCircle(pivot, newRad))
			{
				ret += entityVector[i]->RecieveDamage(skill.dmg);

				if (skill.effect != SKILL_EFFECT::NO_EFFECT)
				{

				}
			}

		}
	}

	return ret;
	}
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
bool ModuleEntityManager::LoadSampleHero(ENTITY_TYPE heroType, pugi::xml_node& heroNode, pugi::xml_node& config)
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



	//skill1

	SKILL_ID skill1ID = (SKILL_ID)heroNode.child("sample").child("skills").child("skill1").attribute("id").as_int(0);
	Skill heroSkill;
	RequestSkill(heroSkill, skill1ID, 1);

	SKILL_ID passiveSkillID = (SKILL_ID)heroNode.child("sample").child("skills").child("passive").attribute("id").as_int(0);
	Skill passiveSkill;
	RequestSkill(passiveSkill, passiveSkillID, 1);

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

	Animation deathRight = deathRight.PushAnimation(heroNode, "death_right");
	Animation deathRightUp = deathRightUp.PushAnimation(heroNode, "death_right_up");
	Animation deathRightDown = deathRightDown.PushAnimation(heroNode, "death_right_down");
	Animation deathLeft = deathLeft.PushAnimation(heroNode, "death_left");
	Animation deathLeftUp = deathLeftUp.PushAnimation(heroNode, "death_left_up");
	Animation deathLeftDown = deathLeftDown.PushAnimation(heroNode, "death_left_down");

	//General Vfx load -----------------------------------
	P2SString filename = config.child("load").attribute("docnamevfx").as_string();
	pugi::xml_document vfxDoc;
	vfxDoc.load_file(filename.GetString());
	pugi::xml_node vfx = vfxDoc.child("Vfx");

	Animation tileOnWalk = tileOnWalk.PushAnimation(vfx, "onMyWay");

	//Stats ---------------------------------
	HeroStats sampleStats;

	RequestHeroStats(sampleStats, heroType);

	switch (heroType)
	{
	case ENTITY_TYPE::HERO_MELEE:

		//Sample Creation ----------------------------
		sampleMelee = new MeleeHero(pos, collider, walkLeft, walkLeftUp,
			walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightUp, idleRightDown, idleLeft,
			idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp, punchRightDown, punchRight, skill1Right,
			skill1RightUp, skill1RightDown, skill1Left, skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, tileOnWalk,
			sampleStats, heroSkill, passiveSkill);

		ret = true;
		break;


	case ENTITY_TYPE::HERO_RANGED:

		//Sample Creation ----------------------------
		sampleRanged = new RangedHero(pos, collider, walkLeft, walkLeftUp,
			walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightUp, idleRightDown, idleLeft,
			idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp, punchRightDown, punchRight, skill1Right,
			skill1RightUp, skill1RightDown, skill1Left, skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, tileOnWalk,
			sampleStats, heroSkill, passiveSkill);

		ret = true;
		break;


	case ENTITY_TYPE::HERO_ROBO:

		//Sample Creation ----------------------------
		sampleRobo = new RoboHero(pos, collider, walkLeft, walkLeftUp,
			walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightUp, idleRightDown, idleLeft,
			idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp, punchRightDown, punchRight, skill1Right,
			skill1RightUp, skill1RightDown, skill1Left, skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, tileOnWalk,
			sampleStats, heroSkill, passiveSkill);

		ret = true;
		break;


	case ENTITY_TYPE::HERO_GATHERER:


		//Vfx load -----------------------------------
		Animation vfxExplosion = vfxExplosion.PushAnimation(vfx, "explosion");

		//Sample Creation ----------------------------
		sampleGatherer = new GathererHero(pos, collider, walkLeft, walkLeftUp,
			walkLeftDown, walkRightUp, walkRightDown, walkRight, idleRight, idleRightUp, idleRightDown, idleLeft,
			idleLeftUp, idleLeftDown, punchLeft, punchLeftUp, punchLeftDown, punchRightUp, punchRightDown, punchRight, skill1Right,
			skill1RightUp, skill1RightDown, skill1Left, skill1LeftUp, skill1LeftDown, deathRight, deathRightUp, deathRightDown, deathLeft, deathLeftUp, deathLeftDown, tileOnWalk,
			sampleStats, heroSkill, passiveSkill, vfxExplosion);

		ret = true;
		break;


	}
	vfxDoc.reset();

	return ret;
}


bool ModuleEntityManager::LoadSampleEnemy(pugi::xml_node& enemyNode, ENTITY_TYPE enemyType)
{
	bool ret = true;

	//collider
	SDL_Rect r;
	r.x = enemyNode.child("collider").child("rect").attribute("x").as_int(0);
	r.y = enemyNode.child("collider").child("rect").attribute("y").as_int(0);
	r.w = enemyNode.child("collider").child("rect").attribute("w").as_int(0);
	r.h = enemyNode.child("collider").child("rect").attribute("h").as_int(0);
	COLLIDER_TYPE cType = (COLLIDER_TYPE)enemyNode.child("collider").child("type").attribute("id").as_int(0);
	Collider* enemyCollider = new Collider(r, cType, this);

	//stats
	fMPoint pos;
	pos.x = enemyNode.child("position").attribute("x").as_float(0);
	pos.y = enemyNode.child("position").attribute("y").as_float(0);


	int maxHP = enemyNode.child("stats").child("hitPoints").attribute("max").as_int(0);
	int currentHP = enemyNode.child("stats").child("hitPoints").attribute("current").as_int(0);
	int recoveryHP = enemyNode.child("stats").child("hitPoints").attribute("recoveryRate").as_int(0);

	int atkDmg = enemyNode.child("stats").child("attack").attribute("damage").as_int(0);
	float atkSpd = enemyNode.child("stats").child("attack").attribute("speed").as_float(0);
	int atkRange = enemyNode.child("stats").child("attack").attribute("range").as_int(0);

	int movSpd = enemyNode.child("stats").attribute("movementSpeed").as_int(0);
	int vision = enemyNode.child("stats").attribute("vision").as_int(0);
	int xp = enemyNode.child("stats").attribute("xp").as_int(0);

	float scale = enemyNode.child("stats").attribute("scale").as_float(1.0f);

	enemyNode = enemyNode.parent();
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

	Animation enemyDeathRight = enemyDeathRight.PushAnimation(enemyNode, "wanamingoRightDeath"); //looks good
	Animation enemyDeathRightUp = enemyDeathRightUp.PushAnimation(enemyNode, "wanamingoUpRightDeath"); //jesus christ 
	Animation enemyDeathRightDown = enemyDeathRightDown.PushAnimation(enemyNode, "wanamingoDownRightDeath"); //goes back and forth
	Animation enemyDeathLeft = enemyDeathLeft.PushAnimation(enemyNode, "wanamingoLeftDeath"); //It should bump to the other side!
	Animation enemyDeathLeftUp = enemyDeathLeftUp.PushAnimation(enemyNode, "wanamingoUpLeftDeath"); //jesus christ 
	Animation enemyDeathLeftDown = enemyDeathLeftDown.PushAnimation(enemyNode, "wanamingoDownLeftDeath"); //jesus christ 

	switch (enemyType)
	{
	case ENTITY_TYPE::ENEMY:
		sampleEnemy = new Enemy(pos, ENTITY_TYPE::ENEMY, enemyCollider, enemyWalkLeft, enemyWalkLeftUp,
			enemyWalkLeftDown, enemyWalkRightUp, enemyWalkRightDown, enemyWalkRight, enemyIdleRight, enemyIdleRightUp, enemyIdleRightDown, enemyIdleLeft,
			enemyIdleLeftUp, enemyIdleLeftDown, enemyPunchLeft, enemyPunchLeftUp, enemyPunchLeftDown, enemyPunchRightUp, enemyPunchRightDown, enemyPunchRight,
			enemyDeathRight, enemyDeathRightUp, enemyDeathRightDown, enemyDeathLeft, enemyDeathLeftUp, enemyDeathLeftDown, maxHP, currentHP, recoveryHP, vision, atkDmg, atkSpd, atkRange, movSpd, xp, scale);
		break;
	case ENTITY_TYPE::ENEMY_RANGED:
		sampleEnemyRanged = new RangedEnemy(pos, ENTITY_TYPE::ENEMY_RANGED, enemyCollider, enemyWalkLeft, enemyWalkLeftUp,
			enemyWalkLeftDown, enemyWalkRightUp, enemyWalkRightDown, enemyWalkRight, enemyIdleRight, enemyIdleRightUp, enemyIdleRightDown, enemyIdleLeft,
			enemyIdleLeftUp, enemyIdleLeftDown, enemyPunchLeft, enemyPunchLeftUp, enemyPunchLeftDown, enemyPunchRightUp, enemyPunchRightDown, enemyPunchRight,
			enemyDeathRight, enemyDeathRightUp, enemyDeathRightDown, enemyDeathLeft, enemyDeathLeftUp, enemyDeathLeftDown, maxHP, currentHP, recoveryHP, vision, atkDmg, atkSpd, atkRange, movSpd, xp, scale);
		break;
	case ENTITY_TYPE::ENEMY_NIGHT:
		sampleEnemyNight = new NightEnemy(pos, ENTITY_TYPE::ENEMY_NIGHT, enemyCollider, enemyWalkLeft, enemyWalkLeftUp,
			enemyWalkLeftDown, enemyWalkRightUp, enemyWalkRightDown, enemyWalkRight, enemyIdleRight, enemyIdleRightUp, enemyIdleRightDown, enemyIdleLeft,
			enemyIdleLeftUp, enemyIdleLeftDown, enemyPunchLeft, enemyPunchLeftUp, enemyPunchLeftDown, enemyPunchRightUp, enemyPunchRightDown, enemyPunchRight,
			enemyDeathRight, enemyDeathRightUp, enemyDeathRightDown, enemyDeathLeft, enemyDeathLeftUp, enemyDeathLeftDown, maxHP, currentHP, recoveryHP, vision, atkDmg, atkSpd, atkRange, movSpd, xp, scale);
		break;
	case ENTITY_TYPE::ENEMY_GIGA:
		sampleEnemyGiga = new GigaEnemy(pos, ENTITY_TYPE::ENEMY_GIGA, enemyCollider, enemyWalkLeft, enemyWalkLeftUp,
			enemyWalkLeftDown, enemyWalkRightUp, enemyWalkRightDown, enemyWalkRight, enemyIdleRight, enemyIdleRightUp, enemyIdleRightDown, enemyIdleLeft,
			enemyIdleLeftUp, enemyIdleLeftDown, enemyPunchLeft, enemyPunchLeftUp, enemyPunchLeftDown, enemyPunchRightUp, enemyPunchRightDown, enemyPunchRight,
			enemyDeathRight, enemyDeathRightUp, enemyDeathRightDown, enemyDeathLeft, enemyDeathLeftUp, enemyDeathLeftDown, maxHP, currentHP, recoveryHP, vision, atkDmg, atkSpd, atkRange, movSpd, xp, scale);
		break;


	}


	return ret;
}


bool ModuleEntityManager::LoadSampleTurret(pugi::xml_node& turretNode)
{
	bool ret = true;
	//collider
	SDL_Rect r;
	r.x = turretNode.child("sample").child("collider").child("rect").attribute("x").as_int(0);
	r.y = turretNode.child("sample").child("collider").child("rect").attribute("y").as_int(0);
	r.w = turretNode.child("sample").child("collider").child("rect").attribute("w").as_int(0);
	r.h = turretNode.child("sample").child("collider").child("rect").attribute("h").as_int(0);

	COLLIDER_TYPE cType = (COLLIDER_TYPE)turretNode.child("sample").child("collider").child("type").attribute("id").as_int(0);

	Collider* turretCollider = new Collider(r, cType, this);

	//stats
	fMPoint pos;
	pos.x = turretNode.child("sample").child("position").attribute("x").as_float(0);
	pos.y = turretNode.child("sample").child("position").attribute("y").as_float(0);

	int level = turretNode.child("sample").child("stats").attribute("level").as_int(0);
	int vision = turretNode.child("sample").child("stats").attribute("vision").as_int(0);
	int xp = turretNode.child("sample").child("stats").attribute("xp").as_int(0);
	int buildingCost = turretNode.child("sample").child("stats").attribute("buildingCost").as_int(0);
	int transparency = turretNode.child("sample").child("stats").attribute("transparency").as_int(0);

	int maxHP = turretNode.child("sample").child("stats").child("hitPoints").attribute("max").as_int(0);
	int currentHP = turretNode.child("sample").child("stats").child("hitPoints").attribute("current").as_int(0);
	int recoveryHP = turretNode.child("sample").child("stats").child("hitPoints").attribute("recoveryRate").as_int(0);

	int atkDmg = turretNode.child("sample").child("stats").child("attack").attribute("damage").as_int(0);
	float atkSpd = turretNode.child("sample").child("stats").child("attack").attribute("speed").as_float(0);
	int atkRange = turretNode.child("sample").child("stats").child("attack").attribute("range").as_int(0);



	//Animations
	Animation turretIdleRight = turretIdleRight.PushAnimation(turretNode, "turretIdleRight"); //goes up then bumps right
	Animation turretIdleRightUp = turretIdleRightUp.PushAnimation(turretNode, "turretIdleRightUp"); //bumps left
	Animation turretIdleRightDown = turretIdleRightDown.PushAnimation(turretNode, "turretIdleRightDown"); //bumps right
	Animation turretIdleLeft = turretIdleLeft.PushAnimation(turretNode, "turretIdleLeft"); //bumps left
	Animation turretIdleLeftUp = turretIdleLeftUp.PushAnimation(turretNode, "turretIdleLeftUp"); //bumps right
	Animation turretIdleLeftDown = turretIdleLeftDown.PushAnimation(turretNode, "turretIdleLeftDown"); //bumps right

	Animation turretShootingRight = turretShootingRight.PushAnimation(turretNode, "turretShootingRight"); //goes up then bumps right
	Animation turretShootingRightUp = turretShootingRightUp.PushAnimation(turretNode, "turretShootingRightUp"); //bumps left
	Animation turretShootingRightDown = turretShootingRightDown.PushAnimation(turretNode, "turretShootingRightDown"); //bumps right
	Animation turretShootingLeft = turretShootingLeft.PushAnimation(turretNode, "turretShootingLeft"); //bumps left
	Animation turretShootingLeftUp = turretShootingLeftUp.PushAnimation(turretNode, "turretShootingLeftUp"); //bumps right
	Animation turretShootingLeftDown = turretShootingLeftDown.PushAnimation(turretNode, "turretShootingLeftDown"); //bumps right


	//sample
	sampleTurret = new Turret(level, atkDmg, atkSpd, atkRange, vision, pos, turretCollider, turretIdleRight, turretIdleRightUp, turretIdleRightDown, turretIdleLeft, turretIdleLeftUp, turretIdleLeftDown,
		turretShootingRight, turretShootingRightUp, turretShootingRightDown, turretShootingLeft, turretShootingLeftUp, turretShootingLeftDown,
		maxHP, currentHP, recoveryHP, xp, buildingCost, transparency);

	return ret;
}


bool ModuleEntityManager::LoadSampleBarricade(pugi::xml_node& barricadeNode)
{
	barricadeNode = barricadeNode.first_child();

	//Collider
	int colW = barricadeNode.child("collider").child("rect").attribute("w").as_int();
	int colH = barricadeNode.child("collider").child("rect").attribute("h").as_int();
	COLLIDER_TYPE type = (COLLIDER_TYPE)barricadeNode.child("collider").child("type").attribute("id").as_int();

	//Rects
	SDL_Rect verticalRect{ barricadeNode.child("rect").child("verticalRect").attribute("x").as_int(), barricadeNode.child("rect").child("verticalRect").attribute("y").as_int(),
	barricadeNode.child("rect").child("verticalRect").attribute("w").as_int() , barricadeNode.child("rect").child("verticalRect").attribute("h").as_int() };

	SDL_Rect horizontalRect{ barricadeNode.child("rect").child("horizontalRect").attribute("x").as_int(), barricadeNode.child("rect").child("horizontalRect").attribute("y").as_int(),
	barricadeNode.child("rect").child("horizontalRect").attribute("w").as_int() , barricadeNode.child("rect").child("horizontalRect").attribute("h").as_int() };

	//Stats
	int level = barricadeNode.child("stats").attribute("level").as_int(0);
	int vision = barricadeNode.child("stats").attribute("vision").as_int(0);
	int xp = barricadeNode.child("stats").attribute("xp").as_int(0);
	int buildingCost = barricadeNode.child("stats").attribute("buildingCost").as_int(0);
	int transparency = barricadeNode.child("stats").attribute("transparency").as_int(0);

	int maxHP = barricadeNode.child("stats").child("hitPoints").attribute("max").as_int(0);
	int currentHP = barricadeNode.child("stats").child("hitPoints").attribute("current").as_int(0);
	int recoveryHP = barricadeNode.child("stats").child("hitPoints").attribute("recoveryRate").as_int(0);

	sampleBarricade = new Barricade(fMPoint(0, 0), maxHP, currentHP, recoveryHP, xp, buildingCost, transparency, new Collider(horizontalRect, type, this), verticalRect, horizontalRect);

	return true;
}


bool ModuleEntityManager::LoadSampleUpgradeCenter(pugi::xml_node& upgradeCenterNode)
{
	upgradeCenterNode = upgradeCenterNode.first_child();

	//Collider
	int colW = upgradeCenterNode.child("collider").child("rect").attribute("w").as_int();
	int colH = upgradeCenterNode.child("collider").child("rect").attribute("h").as_int();
	COLLIDER_TYPE type = (COLLIDER_TYPE)upgradeCenterNode.child("collider").child("type").attribute("id").as_int();
	SDL_Rect rect = { 0, 0, colW, colH };

	//Stats
	int xp = upgradeCenterNode.child("stats").attribute("xp").as_int(0);
	int buildingCost = upgradeCenterNode.child("stats").attribute("buildingCost").as_int(0);
	int transparency = upgradeCenterNode.child("stats").attribute("transparency").as_int(0);

	int maxHP = upgradeCenterNode.child("stats").child("hitPoints").attribute("max").as_int(0);
	int currentHP = upgradeCenterNode.child("stats").child("hitPoints").attribute("current").as_int(0);
	int recoveryHP = upgradeCenterNode.child("stats").child("hitPoints").attribute("recoveryRate").as_int(0);

	int upgradeTurretCost = upgradeCenterNode.child("costs").attribute("upgradeTurret").as_int(0);
	int upgradeBarricadeCost = upgradeCenterNode.child("costs").attribute("upgradeBarricade").as_int(0);

	sampleUpgradeCenter = new UpgradeCenter(fMPoint(0, 0), upgradeTurretCost, upgradeBarricadeCost, maxHP, currentHP, recoveryHP, xp, buildingCost, transparency, new Collider(rect, type, this));

	return true;
}


bool ModuleEntityManager::LoadSampleSpawner(pugi::xml_node& spawnerNode)
{
	bool ret = true;

	SDL_Rect r;
	r.x = spawnerNode.child("sample").child("collider").child("rect").attribute("x").as_int(0);
	r.y = spawnerNode.child("sample").child("collider").child("rect").attribute("y").as_int(0);
	r.w = spawnerNode.child("sample").child("collider").child("rect").attribute("w").as_int(0);
	r.h = spawnerNode.child("sample").child("collider").child("rect").attribute("h").as_int(0);
	COLLIDER_TYPE cType = (COLLIDER_TYPE)spawnerNode.child("sample").child("collider").child("type").attribute("id").as_int(0);

	Collider* spawnerCollider = new Collider(r, cType, app->ai);

	int wannamingosPerWave = spawnerNode.child("sample").child("stats").attribute("wannamingosPerWave").as_int(0);
	float wannamingosSpawnRate = spawnerNode.child("sample").child("stats").attribute("wannamingosSpawnRate").as_float(0);

	int gigamingosPerWave = spawnerNode.child("sample").child("stats").attribute("gigamingosPerWave").as_int(0);
	float gigamingosSpawnRate = spawnerNode.child("sample").child("stats").attribute("gigamingosSpawnRate").as_float(0);

	int speedamingosPerWave = spawnerNode.child("sample").child("stats").attribute("speedamingosPerWave").as_int(0);
	float speedamingosSpawnRate = spawnerNode.child("sample").child("stats").attribute("speedamingosSpawnRate").as_float(0);

	int snipermingosPerWave = spawnerNode.child("sample").child("stats").attribute("snipermingosPerWave").as_int(0);
	float snipermingosSpawnRate = spawnerNode.child("sample").child("stats").attribute("snipermingosSpawnRate").as_float(0);


	sampleSpawner = new Spawner(fMPoint(0, 0), spawnerCollider, wannamingosPerWave, wannamingosSpawnRate, gigamingosPerWave, gigamingosSpawnRate, 
								speedamingosPerWave, speedamingosSpawnRate, snipermingosPerWave, snipermingosSpawnRate);

	return ret;

}


bool ModuleEntityManager::LoadSampleBuilding(pugi::xml_node& buildingNode)
{
	//collider
	bool ret = true;
	SDL_Rect r;
	r.x = buildingNode.child("sample").child("collider").child("rect").attribute("x").as_int(0);
	r.y = buildingNode.child("sample").child("collider").child("rect").attribute("y").as_int(0);
	r.w = buildingNode.child("sample").child("collider").child("rect").attribute("w").as_int(0);
	r.h = buildingNode.child("sample").child("collider").child("rect").attribute("h").as_int(0);
	COLLIDER_TYPE cType = (COLLIDER_TYPE)buildingNode.child("sample").child("collider").child("type").attribute("id").as_int(0);
	Collider* buildingCollider = new Collider(r, cType, this);

	//stats
	fMPoint pos;
	pos.x = buildingNode.child("sample").child("position").attribute("x").as_float(0);
	pos.y = buildingNode.child("sample").child("position").attribute("y").as_float(0);

	int maxHP = buildingNode.child("sample").child("stats").child("hitPoints").attribute("max").as_int(0);
	int currentHP = buildingNode.child("sample").child("stats").child("hitPoints").attribute("current").as_int(0);
	int recoveryHP = buildingNode.child("sample").child("stats").child("hitPoints").attribute("recoveryRate").as_int(0);

	int xp = buildingNode.child("sample").child("stats").attribute("xp").as_int(0);
	int buildingCost = buildingNode.child("sample").child("stats").attribute("buildingCost").as_int(0);
	int transparency = buildingNode.child("sample").child("stats").attribute("transparency").as_int(0);


	//sample
	sampleBuilding = new Building(pos, maxHP, currentHP, recoveryHP, xp, buildingCost, transparency, buildingCollider);

	return ret;
}


bool ModuleEntityManager::LoadSampleBase(pugi::xml_node& baseNode)
{
	bool ret = true;
	//colliders
	SDL_Rect r;
	r.x = baseNode.child("sample").child("collider").child("rect").attribute("x").as_int(0);
	r.y = baseNode.child("sample").child("collider").child("rect").attribute("y").as_int(0);
	r.w = baseNode.child("sample").child("collider").child("rect").attribute("w").as_int(0);
	r.h = baseNode.child("sample").child("collider").child("rect").attribute("h").as_int(0);
	COLLIDER_TYPE cType = (COLLIDER_TYPE)baseNode.child("sample").child("collider").child("type").attribute("id").as_int(0);
	Collider* baseCollider = new Collider(r, cType, this);

	r.x = baseNode.child("sample").child("baseAlarmCollider").child("rect").attribute("x").as_int(0);
	r.y = baseNode.child("sample").child("baseAlarmCollider").child("rect").attribute("y").as_int(0);
	r.w = baseNode.child("sample").child("baseAlarmCollider").child("rect").attribute("w").as_int(0);
	r.h = baseNode.child("sample").child("baseAlarmCollider").child("rect").attribute("h").as_int(0);
	cType = (COLLIDER_TYPE)baseNode.child("sample").child("baseAlarmCollider").child("type").attribute("id").as_int(0);

	Collider* baseAlarmCollider = new Collider(r, cType, app->ai);

	//stats
	int maxHP = baseNode.child("sample").child("stats").child("hitPoints").attribute("max").as_int(0);
	int currentHP = baseNode.child("sample").child("stats").child("hitPoints").attribute("current").as_int(0);
	int recoveryHP = baseNode.child("sample").child("stats").child("hitPoints").attribute("recoveryRate").as_int(0);

	int resourcesProd = baseNode.child("sample").child("stats").child("resources").attribute("produced").as_int(0);
	int resourcesRate = baseNode.child("sample").child("stats").child("resources").attribute("rate").as_int(0);

	int maxTurrets = baseNode.child("sample").child("stats").attribute("maxTurrets").as_int(0);
	int maxBarricades = baseNode.child("sample").child("stats").attribute("maxBarricades").as_int(0);
	int transparency = baseNode.child("sample").child("stats").attribute("transparency").as_int(0);

	//TODO FOR OSCAR: although is not used at the moment, the plan is to make bases interact with the FoW, so we will need this variable
	int vision = baseNode.child("sample").child("stats").attribute("vision").as_int(0);

	//sample	
	sampleBase = new Base(fMPoint{ 0, 0 }, baseCollider, maxTurrets, maxBarricades, nullptr, baseAlarmCollider, resourcesProd, resourcesRate,
		maxHP, currentHP, recoveryHP, transparency);

	return ret;

}


bool ModuleEntityManager::LoadSampleParticleSystemsAndEmitters(pugi::xml_node& particleSystemsNode)
{
	bool ret = true;


	//DATA--------------

	Animation anim1;
	anim1.PushBack(SDL_Rect{ 0, 0, 10, 10 }, 1, 0, 0);

	Animation anim2;
	anim2.PushBack(SDL_Rect{ 0, 12, 6, 6 }, 1, 0, 0);

	Animation anim3;
	anim3.PushBack(SDL_Rect{ 7, 11, 8, 8 }, 1, 0, 0);

	float auxPosX = 0;							float auxPos3X = 0;
	float auxPosY = 0;							float auxPos3Y = 0;
	float auxSpeedX = 0;						float auxSpeed3X = 0;
	float auxSpeedY = 4;						float auxSpeedY3 = 6;
	int particleVariationSpeedX = 0;			int particleVariationSpeedX3 = 0;
	int particleVariationSpeedY = 2;			int particleVariationSpeedY3 = 2;
	float particleAccelerationX = 0.2;			float particleAccelerationX3 = 0.2;
	float particleAccelerationY = -0.5;			float particleAccelerationY3 = -0.5;
	int particleVariationAccelerationX = 1;		int particleVariationAccelerationX3 = 1;
	int particleVariationAccelerationY = 0;		int particleVariationAccelerationY3 = 0;
	float particleAngularSpeed = 0;				float particleAngularSpeed3 = 0;
	int particleVariableAngularSpeed = 1;		int particleVariableAngularSpeed3 = 1;
	float particlesRate = 8;					float particlesRate3 = 13;
	float particlesLifeTime = 0.55;				float particlesLifeTime3 = 0.55;


	//SAMPLES--------------

	sampleParticleSystem = new ParticleSystem();

	sampleEmitter = new Emitter(auxPosX, auxPosY, auxSpeedX, auxSpeedY, particleVariationSpeedX, particleVariationSpeedY, particleAccelerationX, particleAccelerationY, particleVariationAccelerationX, particleVariationAccelerationY, particleAngularSpeed, particleVariableAngularSpeed, particlesRate, particlesLifeTime, nullptr, nullptr, anim1, true);
	sampleEmitter2 = new Emitter(auxPosX, auxPosY, -auxSpeedX, auxSpeedY, -particleVariationSpeedX, particleVariationSpeedY, -particleAccelerationX, particleAccelerationY, -particleVariationAccelerationX, particleVariationAccelerationY, particleAngularSpeed, particleVariableAngularSpeed, particlesRate, particlesLifeTime, nullptr, nullptr, anim1, true);

	sampleEmitter3 = new Emitter(auxPos3X, auxPos3Y, auxSpeed3X, auxSpeedY3, particleVariationSpeedX3, particleVariationSpeedY3, particleAccelerationX3, particleAccelerationY3, particleVariationAccelerationX3, particleVariationAccelerationY3, particleAngularSpeed3, particleVariableAngularSpeed3, particlesRate3, particlesLifeTime3, nullptr, nullptr, anim3, true);
	sampleEmitter4 = new Emitter(auxPos3X, auxPos3Y, -auxSpeed3X, auxSpeedY3, -particleVariationSpeedX3, particleVariationSpeedY3, -particleAccelerationX3, particleAccelerationY3, -particleVariationAccelerationX3, particleVariationAccelerationY3, particleAngularSpeed3, particleVariableAngularSpeed3, particlesRate3, particlesLifeTime3, nullptr, nullptr, anim3, true);

	return ret;
}



bool ModuleEntityManager::LoadSkillAreas(pugi::xml_node& areasNode)
{
	bool ret = true;

	skillArea area;
	int r;

	for (pugi::xml_node currentArea = areasNode.child("area"); currentArea; currentArea = currentArea.next_sibling("area"))
	{
		r = currentArea.child("measures").attribute("r").as_int(0);

		GenerateNewArea(r);
	}

	return ret;
}


Entity* ModuleEntityManager::SearchEntity(ENTITY_TYPE type)
{
	int entityNumber = entityVector.size();

	for (int i = 0; i < entityNumber; i++)
	{
		if (entityVector[i]->GetType() == type)
		{
			return entityVector[i];
		}
	}

	return nullptr;
}


bool ModuleEntityManager::Load(pugi::xml_node& data)
{
	int i = 0;

	Spawner* spawner = nullptr;
	Hero* hero = nullptr;
	Enemy* enemy = nullptr;
	Building* building = nullptr;
	Quest* quest = nullptr;

	Base* base = nullptr;
	Turret* turret = nullptr;
	UpgradeCenter* upgradeCenter = nullptr;
	Barricade* barricade = nullptr;

	for (pugi::xml_node iterator = data.first_child(); iterator != NULL; iterator = iterator.next_sibling(), i++)
	{
		P2SString type(iterator.attribute("type").as_string());

		if (type == "spawner")
		{
			spawner = (Spawner*)AddEntity(ENTITY_TYPE::SPAWNER, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());

			spawner->SetWannamingosToSpawn(iterator.attribute("wannamingos_to_spawn").as_int());
			spawner->SetWannamingoSpawnRate(iterator.attribute("wannamingos_spawn_rate").as_float());
			spawner->SetWannamingosPerWave(iterator.attribute("wannamingos_per_wave").as_int());

			spawner->SetGigamingosToSpawn(iterator.attribute("gigamingos_to_spawn").as_int());
			spawner->SetGigamingoSpawnRate(iterator.attribute("gigamingos_spawn_rate").as_float());
			spawner->SetGigamingosPerWave(iterator.attribute("gigamingos_per_wave").as_int());

			spawner->SetSpeedamingosToSpawn(iterator.attribute("speedamingos_to_spawn").as_int());
			spawner->SetSpeedamingoSpawnRate(iterator.attribute("speedamingos_spawn_rate").as_float());
			spawner->SetSpeedamingosPerWave(iterator.attribute("speedamingos_per_wave").as_int());

			spawner->SetSnipermingosToSpawn(iterator.attribute("snipermingos_to_spawn").as_int());
			spawner->SetSnipermingosSpawnRate(iterator.attribute("snipermingos_spawn_rate").as_float());
			spawner->SetSnipermingosPerWave(iterator.attribute("snipermingos_per_wave").as_int());

			if (iterator.attribute("active").as_bool() == true)
				spawner->Activate();

			else
				spawner->Desactivate();
		}


		if (type == "quest")
		{
			quest = (Quest*)AddEntity(ENTITY_TYPE::QUEST, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());

			quest->SetId(iterator.attribute("id").as_int());
		}


		else if (type == "hero_melee")
		{
			hero = (Hero*)AddEntity(ENTITY_TYPE::HERO_MELEE, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());

			hero->SetHeroLevel(iterator.attribute("level").as_int());
			hero->SetExpToLevelUp(iterator.attribute("exp_to_level_up").as_int());
			hero->SetHeroXP(iterator.attribute("hero_xp").as_int());

			hero->SetCurrentHP(iterator.attribute("hit_points").as_int());
			hero->SetMaxHP(iterator.attribute("max_hit_points").as_int());
			hero->SetRecoveryHitPointsRate(iterator.attribute("recovery_hit_points_rate").as_int());

			hero->SetEnergyPoints(iterator.attribute("energy_points").as_int());
			hero->SetMaxEnergyPoints(iterator.attribute("max_energy_points").as_int());
			hero->SetRecoveryEnergyRate(iterator.attribute("recovery_energy_rate").as_int());

			hero->SetRecoverHealth(iterator.attribute("recovering_health").as_float());
			hero->SetRecoveringEnergy(iterator.attribute("recovering_energy").as_float());
			hero->SetFeelingSecure(iterator.attribute("feeling_secure").as_float());

			hero->SetAttackDamage(iterator.attribute("attack_damage").as_int());
			hero->SetAttackRange(iterator.attribute("attack_range").as_int());
			hero->SetAttackSpeed(iterator.attribute("attack_speed").as_float());

			hero->SetVisionDistance(iterator.attribute("vision_distance").as_int());
			hero->SetVisionInPx(iterator.attribute("vision_in_px").as_float());

			hero->SetSkill1Cost(iterator.attribute("skill1_cost").as_int());
			hero->SetHeroSkillPoints(iterator.attribute("heroSkillPoints").as_int());

			Skill skill1;
			SKILL_ID skill1Id = (SKILL_ID)iterator.attribute("skill1Id").as_int();
			int skill1Lvl = iterator.attribute("skill1Lvl").as_int();

			RequestSkill(skill1, skill1Id, skill1Lvl);
			hero->ReplaceSkill1(skill1);

			Skill passiveSkill;
			SKILL_ID passiveSkillId = (SKILL_ID)iterator.attribute("passiveSkillId").as_int();
			int passiveSkillLvl = iterator.attribute("passiveSkillLvl").as_int();

			RequestSkill(passiveSkill, passiveSkillId, passiveSkillLvl);
			hero->ReplacePassiveSkill(passiveSkill);

			meleeLifeUpgradeValue = iterator.attribute("hp").as_float();
			meleeDamageUpgradeValue = iterator.attribute("damage").as_float();
			meleeEnergyUpgradeValue = iterator.attribute("energy").as_float();
			meleeAtkSpeedUpgradeValue = iterator.attribute("atkSpeed").as_float();
		}


		else if (type == "hero_ranged")
		{
			hero = (Hero*)AddEntity(ENTITY_TYPE::HERO_RANGED, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());

			hero->SetHeroLevel(iterator.attribute("level").as_int());
			hero->SetExpToLevelUp(iterator.attribute("exp_to_level_up").as_int());
			hero->SetHeroXP(iterator.attribute("hero_xp").as_int());

			hero->SetCurrentHP(iterator.attribute("hit_points").as_int());
			hero->SetMaxHP(iterator.attribute("max_hit_points").as_int());
			hero->SetRecoveryHitPointsRate(iterator.attribute("recovery_hit_points_rate").as_int());

			hero->SetEnergyPoints(iterator.attribute("energy_points").as_int());
			hero->SetMaxEnergyPoints(iterator.attribute("max_energy_points").as_int());
			hero->SetRecoveryEnergyRate(iterator.attribute("recovery_energy_rate").as_int());

			hero->SetRecoverHealth(iterator.attribute("recovering_health").as_float());
			hero->SetRecoveringEnergy(iterator.attribute("recovering_energy").as_float());
			hero->SetFeelingSecure(iterator.attribute("feeling_secure").as_float());

			hero->SetAttackDamage(iterator.attribute("attack_damage").as_int());
			hero->SetAttackRange(iterator.attribute("attack_range").as_int());
			hero->SetAttackSpeed(iterator.attribute("attack_speed").as_float());

			hero->SetVisionDistance(iterator.attribute("vision_distance").as_int());
			hero->SetVisionInPx(iterator.attribute("vision_in_px").as_float());

			hero->SetSkill1Cost(iterator.attribute("skill1_cost").as_int());
			hero->SetHeroSkillPoints(iterator.attribute("heroSkillPoints").as_int());

			Skill skill1;
			SKILL_ID skill1Id = (SKILL_ID)iterator.attribute("skill1Id").as_int();
			int skill1Lvl = iterator.attribute("skill1Lvl").as_int();

			RequestSkill(skill1, skill1Id, skill1Lvl);
			hero->ReplaceSkill1(skill1);

			Skill passiveSkill;
			SKILL_ID passiveSkillId = (SKILL_ID)iterator.attribute("passiveSkillId").as_int();
			int passiveSkillLvl = iterator.attribute("passiveSkillLvl").as_int();

			RequestSkill(passiveSkill, passiveSkillId, passiveSkillLvl);
			hero->ReplacePassiveSkill(passiveSkill);

			rangedLifeUpgradeValue = iterator.attribute("hp").as_float();
			rangedDamageUpgradeValue = iterator.attribute("damage").as_float();
			rangedEnergyUpgradeValue = iterator.attribute("energy").as_float();
			rangedAtkSpeedUpgradeValue = iterator.attribute("atkSpeed").as_float();
		}


		else if (type == "hero_gatherer")
		{
			hero = (Hero*)AddEntity(ENTITY_TYPE::HERO_GATHERER, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());

			hero->SetHeroLevel(iterator.attribute("level").as_int());
			hero->SetExpToLevelUp(iterator.attribute("exp_to_level_up").as_int());
			hero->SetHeroXP(iterator.attribute("hero_xp").as_int());

			hero->SetCurrentHP(iterator.attribute("hit_points").as_int());
			hero->SetMaxHP(iterator.attribute("max_hit_points").as_int());
			hero->SetRecoveryHitPointsRate(iterator.attribute("recovery_hit_points_rate").as_int());

			hero->SetEnergyPoints(iterator.attribute("energy_points").as_int());
			hero->SetMaxEnergyPoints(iterator.attribute("max_energy_points").as_int());
			hero->SetRecoveryEnergyRate(iterator.attribute("recovery_energy_rate").as_int());

			hero->SetRecoverHealth(iterator.attribute("recovering_health").as_float());
			hero->SetRecoveringEnergy(iterator.attribute("recovering_energy").as_float());
			hero->SetFeelingSecure(iterator.attribute("feeling_secure").as_float());

			hero->SetAttackDamage(iterator.attribute("attack_damage").as_int());
			hero->SetAttackRange(iterator.attribute("attack_range").as_int());
			hero->SetAttackSpeed(iterator.attribute("attack_speed").as_float());

			hero->SetVisionDistance(iterator.attribute("vision_distance").as_int());
			hero->SetVisionInPx(iterator.attribute("vision_in_px").as_float());

			hero->SetSkill1Cost(iterator.attribute("skill1_cost").as_int());
			hero->SetHeroSkillPoints(iterator.attribute("heroSkillPoints").as_int());

			Skill skill1;
			SKILL_ID skill1Id = (SKILL_ID)iterator.attribute("skill1Id").as_int();
			int skill1Lvl = iterator.attribute("skill1Lvl").as_int();

			RequestSkill(skill1, skill1Id, skill1Lvl);
			hero->ReplaceSkill1(skill1);

			Skill passiveSkill;
			SKILL_ID passiveSkillId = (SKILL_ID)iterator.attribute("passiveSkillId").as_int();
			int passiveSkillLvl = iterator.attribute("passiveSkillLvl").as_int();

			RequestSkill(passiveSkill, passiveSkillId, passiveSkillLvl);
			hero->ReplacePassiveSkill(passiveSkill);

			gathererLifeUpgradeValue = iterator.attribute("hp").as_float();
			gathererDamageUpgradeValue = iterator.attribute("damage").as_float();
			gathererEnergyUpgradeValue = iterator.attribute("energy").as_float();
			gathererAtkSpeedUpgradeValue = iterator.attribute("atkSpeed").as_float();
		}


		else if (type == "hero_robot")
		{
			hero = (Hero*)AddEntity(ENTITY_TYPE::HERO_ROBO, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());

			hero->SetHeroLevel(iterator.attribute("level").as_int());
			hero->SetExpToLevelUp(iterator.attribute("exp_to_level_up").as_int());
			hero->SetHeroXP(iterator.attribute("hero_xp").as_int());

			hero->SetCurrentHP(iterator.attribute("hit_points").as_int());
			hero->SetMaxHP(iterator.attribute("max_hit_points").as_int());
			hero->SetRecoveryHitPointsRate(iterator.attribute("recovery_hit_points_rate").as_int());

			hero->SetEnergyPoints(iterator.attribute("energy_points").as_int());
			hero->SetMaxEnergyPoints(iterator.attribute("max_energy_points").as_int());
			hero->SetRecoveryEnergyRate(iterator.attribute("recovery_energy_rate").as_int());

			hero->SetRecoverHealth(iterator.attribute("recovering_health").as_float());
			hero->SetRecoveringEnergy(iterator.attribute("recovering_energy").as_float());
			hero->SetFeelingSecure(iterator.attribute("feeling_secure").as_float());

			hero->SetAttackDamage(iterator.attribute("attack_damage").as_int());
			hero->SetAttackRange(iterator.attribute("attack_range").as_int());
			hero->SetAttackSpeed(iterator.attribute("attack_speed").as_float());

			hero->SetVisionDistance(iterator.attribute("vision_distance").as_int());
			hero->SetVisionInPx(iterator.attribute("vision_in_px").as_float());

			hero->SetSkill1Cost(iterator.attribute("skill1_cost").as_int());
			hero->SetHeroSkillPoints(iterator.attribute("heroSkillPoints").as_int());

			Skill skill1;
			SKILL_ID skill1Id = (SKILL_ID)iterator.attribute("skill1Id").as_int();
			int skill1Lvl = iterator.attribute("skill1Lvl").as_int();

			RequestSkill(skill1, skill1Id, skill1Lvl);
			hero->ReplaceSkill1(skill1);

			Skill passiveSkill;
			SKILL_ID passiveSkillId = (SKILL_ID)iterator.attribute("passiveSkillId").as_int();
			int passiveSkillLvl = iterator.attribute("passiveSkillLvl").as_int();

			RequestSkill(passiveSkill, passiveSkillId, passiveSkillLvl);
			hero->ReplacePassiveSkill(passiveSkill);



			robottoLifeUpgradeValue = iterator.attribute("hp").as_float();
			robottoDamageUpgradeValue = iterator.attribute("damage").as_float();
			robottoEnergyUpgradeValue = iterator.attribute("energy").as_float();
			robottoAtkSpeedUpgradeValue = iterator.attribute("atkSpeed").as_float();
		}


		else if (type == "enemy_ranged")
		{
			enemy = (Enemy*)AddEntity(ENTITY_TYPE::ENEMY_RANGED, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());

			enemy->SetLongTermObjective(fMPoint(iterator.attribute("objective_x").as_int(), iterator.attribute("objective_y").as_int()));
			enemy->SetCurrentHP(iterator.attribute("hit_points").as_int());
		}

		else if (type == "enemy_night")
		{
			enemy = (Enemy*)AddEntity(ENTITY_TYPE::ENEMY_NIGHT, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());

			enemy->SetLongTermObjective(fMPoint(iterator.attribute("objective_x").as_int(), iterator.attribute("objective_y").as_int()));
			enemy->SetCurrentHP(iterator.attribute("hit_points").as_int());
		}


		else if (type == "enemy_giga")
		{
			enemy = (Enemy*)AddEntity(ENTITY_TYPE::ENEMY_GIGA, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());

			enemy->SetLongTermObjective(fMPoint(iterator.attribute("objective_x").as_int(), iterator.attribute("objective_y").as_int()));
			enemy->SetCurrentHP(iterator.attribute("hit_points").as_int());
		}


		else if (type == "building")
		{
			int decor = iterator.attribute("decor").as_int();

			switch (decor)
			{
			case 0:
				AddDecorativeBuilding(BUILDING_DECOR::ST_01, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());
				break;

			case 1:
				AddDecorativeBuilding(BUILDING_DECOR::ST_02, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());
				break;

			case 2:
				AddDecorativeBuilding(BUILDING_DECOR::ST_03, iterator.attribute("x").as_int(), iterator.attribute("y").as_int());
				break;
			}
		}


		else if (type == "base")
		{

			base = (Base*)AddEntity(ENTITY_TYPE::BLDG_BASE, iterator.attribute("x").as_int(), iterator.attribute("y").as_int(), (ENTITY_ALIGNEMENT)iterator.attribute("aligment").as_int());

			base->SetCurrentHP(iterator.attribute("hit_points").as_int());

			for (pugi::xml_node iterator2 = iterator.first_child(); iterator2 != NULL; iterator2 = iterator2.next_sibling(), i++)
			{
				P2SString name(iterator2.attribute("name").as_string());

				if (name == "turret")
				{
					turret = (Turret*)AddEntity(ENTITY_TYPE::BLDG_TURRET, iterator2.attribute("x").as_int(), iterator2.attribute("y").as_int(), (ENTITY_ALIGNEMENT)iterator.attribute("aligment").as_int());

					turret->SetLevel(iterator2.attribute("level").as_int());
					turret->SetCurrentHP(iterator.attribute("hit_points").as_int());

					base->AddTurret(turret);
				}

				if (name == "barricade")
				{
					barricade = (Barricade*)AddEntity(ENTITY_TYPE::BLDG_BARRICADE, iterator2.attribute("x").as_int(), iterator2.attribute("y").as_int(), (ENTITY_ALIGNEMENT)iterator.attribute("alignement").as_int());

					barricade->SetLevel(iterator2.attribute("level").as_int());
					barricade->SetCurrentHP(iterator.attribute("hit_points").as_int());

					base->AddBarricade(barricade);
				}

				if (name == "upgrade_center")
				{
					upgradeCenter = (UpgradeCenter*)AddEntity(ENTITY_TYPE::BLDG_UPGRADE_CENTER, iterator2.attribute("x").as_int(), iterator2.attribute("y").as_int(), (ENTITY_ALIGNEMENT)iterator.attribute("alignement").as_int());

					upgradeCenter->SetTurretLevel(iterator2.attribute("turretLvl").as_int());
					upgradeCenter->SetBarricadeLevel(iterator2.attribute("barricadeLvl").as_int());

					base->AddUpgradeCenter(upgradeCenter);
				}
			}

		}
	}


	//deathEntitesSaving
	pugi::xml_node deadEntities = data.child("deadEntities");

	DeleteAllDeadHeroes();

	if (deadEntities.child("deadGatherer") != NULL)
	{
		LoadDeadHero(deadEntities.child("deadGatherer"), ENTITY_TYPE::HERO_GATHERER);
	}
	if (deadEntities.child("deadRanged") != NULL)
	{
		LoadDeadHero(deadEntities.child("deadRanged"), ENTITY_TYPE::HERO_RANGED);
	}
	if (deadEntities.child("deadMelee") != NULL)
	{
		LoadDeadHero(deadEntities.child("deadMelee"), ENTITY_TYPE::HERO_MELEE);
	}
	if (deadEntities.child("deadRobo") != NULL)
	{
		LoadDeadHero(deadEntities.child("deadRobo"), ENTITY_TYPE::HERO_ROBO);
	}

	return true;
}


bool ModuleEntityManager::Save(pugi::xml_node& data) const
{
	int entityNumber = entityVector.size();

	Spawner* spawner = nullptr;
	Hero* hero = nullptr;
	Enemy* enemy = nullptr;
	Building* building = nullptr;
	Quest* quest = nullptr;

	Base* base = nullptr;
	std::vector<Turret*>* turretVector = nullptr;
	std::vector<Barricade*>* barricadeVector = nullptr;
	UpgradeCenter* upgradeCenter = nullptr;

	pugi::xml_node iterator2;

	for (int i = 0; i < entityNumber; i++)
	{
		pugi::xml_node iterator = data.append_child("entity");

		if (entityVector[i]->missionEntity == false)
		{


			switch (entityVector[i]->GetType())
			{
			case ENTITY_TYPE::UNKNOWN:
				break;


			case ENTITY_TYPE::PARTICLE_SYSTEM:
				break;


			case ENTITY_TYPE::SPAWNER:

				iterator.append_attribute("type") = "spawner";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				spawner = (Spawner*)entityVector[i];

				iterator.append_attribute("wannamingos_to_spawn") = spawner->GetWannamingosToSpawn();
				iterator.append_attribute("wannamingos_spawn_rate") = spawner->GetWannamingosSpawnRate();
				iterator.append_attribute("wannamingos_per_wave") = spawner->GetWannamingosPerWave();

				iterator.append_attribute("gigamingos_to_spawn") = spawner->GetGigamingosToSpawn();
				iterator.append_attribute("gigamingos_spawn_rate") = spawner->GetGigamingosSpawnRate();
				iterator.append_attribute("gigamingos_per_wave") = spawner->GetGigamingosPerWave();

				iterator.append_attribute("speedamingos_to_spawn") = spawner->GetSpeedamingosToSpawn();
				iterator.append_attribute("speedamingos_spawn_rate") = spawner->GetSpeedamingosSpawnRate();
				iterator.append_attribute("speedamingos_per_wave") = spawner->GetSpeedamingosPerWave();

				iterator.append_attribute("snipermingos_to_spawn") = spawner->GetWannamingosToSpawn();
				iterator.append_attribute("snipermingos_spawn_rate") = spawner->GetWannamingosSpawnRate();
				iterator.append_attribute("snipermingos_per_wave") = spawner->GetWannamingosPerWave();


				iterator.append_attribute("active") = spawner->GetActive();
				break;


			case ENTITY_TYPE::HERO_MELEE:

				iterator.append_attribute("type") = "hero_melee";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				hero = (Hero*)entityVector[i];

				iterator.append_attribute("level") = hero->GetHeroLevel();
				iterator.append_attribute("exp_to_level_up") = hero->GetExpToLevelUp();
				iterator.append_attribute("hero_exp") = hero->GetHeroXP();

				iterator.append_attribute("hit_points") = hero->GetCurrentHP();
				iterator.append_attribute("max_hit_points") = hero->GetMaxHP();
				iterator.append_attribute("recovery_hit_points_rate") = hero->GetRecoveryHitPointsRate();

				iterator.append_attribute("energy_points") = hero->GetEnergyPoints();
				iterator.append_attribute("max_energy_points") = hero->GetMaxEnergyPoints();
				iterator.append_attribute("recovery_energy_rate") = hero->GetRecoveryEnergyRate();

				iterator.append_attribute("recovering_health") = hero->GetRecoveringHealth();
				iterator.append_attribute("recovering_energy") = hero->GetRecoveringEnergy();
				iterator.append_attribute("feeling_secure") = hero->GetFeelingSecure();

				iterator.append_attribute("attack_damage") = hero->GetAttackDamage();
				iterator.append_attribute("attack_range") = hero->GetAttackRange();
				iterator.append_attribute("attack_speed") = hero->GetAttackSpeed();

				iterator.append_attribute("vision_distance") = hero->GetVisionDistance();
				iterator.append_attribute("vision_in_px") = hero->GetVisionInPx();

				iterator.append_attribute("skill1_cost") = hero->GetSkill1Cost();
				iterator.append_attribute("heroSkillPoints") = hero->GetHeroSkillPoints();

				iterator.append_attribute("skill1Id") = (int)hero->GetSkill1().id;
				iterator.append_attribute("skill1Lvl") = hero->GetSkill1().lvl;

				iterator.append_attribute("passiveSkillId") = (int)hero->GetPassiveSkill().id;
				iterator.append_attribute("passiveSkillLvl") = hero->GetPassiveSkill().lvl;

				iterator.append_attribute("hp") = meleeLifeUpgradeValue;
				iterator.append_attribute("damage") = meleeDamageUpgradeValue;
				iterator.append_attribute("energy") = meleeEnergyUpgradeValue;
				iterator.append_attribute("atkSpeed") = meleeAtkSpeedUpgradeValue;

				break;


			case ENTITY_TYPE::HERO_RANGED:

				iterator.append_attribute("type") = "hero_ranged";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				hero = (Hero*)entityVector[i];

				iterator.append_attribute("level") = hero->GetHeroLevel();
				iterator.append_attribute("exp_to_level_up") = hero->GetExpToLevelUp();
				iterator.append_attribute("hero_exp") = hero->GetHeroXP();

				iterator.append_attribute("hit_points") = hero->GetCurrentHP();
				iterator.append_attribute("max_hit_points") = hero->GetMaxHP();
				iterator.append_attribute("recovery_hit_points_rate") = hero->GetRecoveryHitPointsRate();

				iterator.append_attribute("energy_points") = hero->GetEnergyPoints();
				iterator.append_attribute("max_energy_points") = hero->GetMaxEnergyPoints();
				iterator.append_attribute("recovery_energy_rate") = hero->GetRecoveryEnergyRate();

				iterator.append_attribute("recovering_health") = hero->GetRecoveringHealth();
				iterator.append_attribute("recovering_energy") = hero->GetRecoveringEnergy();
				iterator.append_attribute("feeling_secure") = hero->GetFeelingSecure();

				iterator.append_attribute("attack_damage") = hero->GetAttackDamage();
				iterator.append_attribute("attack_range") = hero->GetAttackRange();
				iterator.append_attribute("attack_speed") = hero->GetAttackSpeed();

				iterator.append_attribute("vision_distance") = hero->GetVisionDistance();
				iterator.append_attribute("vision_in_px") = hero->GetVisionInPx();

				iterator.append_attribute("skill1_cost") = hero->GetSkill1Cost();
				iterator.append_attribute("heroSkillPoints") = hero->GetHeroSkillPoints();

				iterator.append_attribute("skill1Id") = (int)hero->GetSkill1().id;
				iterator.append_attribute("skill1Lvl") = hero->GetSkill1().lvl;

				iterator.append_attribute("passiveSkillId") = (int)hero->GetPassiveSkill().id;
				iterator.append_attribute("passiveSkillLvl") = hero->GetPassiveSkill().lvl;

				iterator.append_attribute("hp") = rangedLifeUpgradeValue;
				iterator.append_attribute("damage") = rangedDamageUpgradeValue;
				iterator.append_attribute("energy") = rangedEnergyUpgradeValue;
				iterator.append_attribute("atkSpeed") = rangedAtkSpeedUpgradeValue;

				break;


			case ENTITY_TYPE::HERO_GATHERER:
				iterator.append_attribute("type") = "hero_gatherer";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				hero = (Hero*)entityVector[i];

				iterator.append_attribute("level") = hero->GetHeroLevel();
				iterator.append_attribute("exp_to_level_up") = hero->GetExpToLevelUp();
				iterator.append_attribute("hero_exp") = hero->GetHeroXP();

				iterator.append_attribute("hit_points") = hero->GetCurrentHP();
				iterator.append_attribute("max_hit_points") = hero->GetMaxHP();
				iterator.append_attribute("recovery_hit_points_rate") = hero->GetRecoveryHitPointsRate();

				iterator.append_attribute("energy_points") = hero->GetEnergyPoints();
				iterator.append_attribute("max_energy_points") = hero->GetMaxEnergyPoints();
				iterator.append_attribute("recovery_energy_rate") = hero->GetRecoveryEnergyRate();

				iterator.append_attribute("recovering_health") = hero->GetRecoveringHealth();
				iterator.append_attribute("recovering_energy") = hero->GetRecoveringEnergy();
				iterator.append_attribute("feeling_secure") = hero->GetFeelingSecure();

				iterator.append_attribute("attack_damage") = hero->GetAttackDamage();
				iterator.append_attribute("attack_range") = hero->GetAttackRange();
				iterator.append_attribute("attack_speed") = hero->GetAttackSpeed();

				iterator.append_attribute("vision_distance") = hero->GetVisionDistance();
				iterator.append_attribute("vision_in_px") = hero->GetVisionInPx();

				iterator.append_attribute("skill1_cost") = hero->GetSkill1Cost();
				iterator.append_attribute("heroSkillPoints") = hero->GetHeroSkillPoints();

				iterator.append_attribute("skill1Id") = (int)hero->GetSkill1().id;
				iterator.append_attribute("skill1Lvl") = hero->GetSkill1().lvl;

				iterator.append_attribute("passiveSkillId") = (int)hero->GetPassiveSkill().id;
				iterator.append_attribute("passiveSkillLvl") = hero->GetPassiveSkill().lvl;

				iterator.append_attribute("hp") = gathererLifeUpgradeValue;
				iterator.append_attribute("damage") = gathererDamageUpgradeValue;
				iterator.append_attribute("energy") = gathererEnergyUpgradeValue;
				iterator.append_attribute("atkSpeed") = gathererAtkSpeedUpgradeValue;

				break;


			case ENTITY_TYPE::HERO_ROBO:

				iterator.append_attribute("type") = "hero_robot";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				hero = (Hero*)entityVector[i];

				iterator.append_attribute("level") = hero->GetHeroLevel();
				iterator.append_attribute("exp_to_level_up") = hero->GetExpToLevelUp();
				iterator.append_attribute("hero_exp") = hero->GetHeroXP();

				iterator.append_attribute("hit_points") = hero->GetCurrentHP();
				iterator.append_attribute("max_hit_points") = hero->GetMaxHP();
				iterator.append_attribute("recovery_hit_points_rate") = hero->GetRecoveryHitPointsRate();

				iterator.append_attribute("energy_points") = hero->GetEnergyPoints();
				iterator.append_attribute("max_energy_points") = hero->GetMaxEnergyPoints();
				iterator.append_attribute("recovery_energy_rate") = hero->GetRecoveryEnergyRate();

				iterator.append_attribute("recovering_health") = hero->GetRecoveringHealth();
				iterator.append_attribute("recovering_energy") = hero->GetRecoveringEnergy();
				iterator.append_attribute("feeling_secure") = hero->GetFeelingSecure();

				iterator.append_attribute("attack_damage") = hero->GetAttackDamage();
				iterator.append_attribute("attack_range") = hero->GetAttackRange();
				iterator.append_attribute("attack_speed") = hero->GetAttackSpeed();

				iterator.append_attribute("vision_distance") = hero->GetVisionDistance();
				iterator.append_attribute("vision_in_px") = hero->GetVisionInPx();

				iterator.append_attribute("skill1_cost") = hero->GetSkill1Cost();
				iterator.append_attribute("heroSkillPoints") = hero->GetHeroSkillPoints();

				iterator.append_attribute("skill1Id") = (int)hero->GetSkill1().id;
				iterator.append_attribute("skill1Lvl") = hero->GetSkill1().lvl;

				iterator.append_attribute("passiveSkillId") = (int)hero->GetPassiveSkill().id;
				iterator.append_attribute("passiveSkillLvl") = hero->GetPassiveSkill().lvl;

				iterator.append_attribute("hp") = robottoLifeUpgradeValue;
				iterator.append_attribute("damage") = robottoDamageUpgradeValue;
				iterator.append_attribute("energy") = robottoEnergyUpgradeValue;
				iterator.append_attribute("atkSpeed") = robottoAtkSpeedUpgradeValue;

				break;


			case ENTITY_TYPE::ENEMY:

				iterator.append_attribute("type") = "enemy";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				enemy = (Enemy*)entityVector[i];

				iterator.append_attribute("objective_x") = enemy->GetLongTermObjectiveX();
				iterator.append_attribute("objective_y") = enemy->GetLongTermObjectiveY();

				iterator.append_attribute("hit_points") = enemy->hitPointsCurrent;

				break;


			case ENTITY_TYPE::ENEMY_RANGED:

				iterator.append_attribute("type") = "enemy_ranged";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				enemy = (Enemy*)entityVector[i];

				iterator.append_attribute("objective_x") = enemy->GetLongTermObjectiveX();
				iterator.append_attribute("objective_y") = enemy->GetLongTermObjectiveY();

				iterator.append_attribute("hit_points") = enemy->hitPointsCurrent;

				break;


			case ENTITY_TYPE::ENEMY_NIGHT:

				iterator.append_attribute("type") = "enemy_night";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				enemy = (Enemy*)entityVector[i];

				iterator.append_attribute("objective_x") = enemy->GetLongTermObjectiveX();
				iterator.append_attribute("objective_y") = enemy->GetLongTermObjectiveY();

				iterator.append_attribute("hit_points") = enemy->hitPointsCurrent;

				break;


			case ENTITY_TYPE::ENEMY_GIGA:

				iterator.append_attribute("type") = "enemy_giga";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				enemy = (Enemy*)entityVector[i];

				iterator.append_attribute("objective_x") = enemy->GetLongTermObjectiveX();
				iterator.append_attribute("objective_y") = enemy->GetLongTermObjectiveY();

				iterator.append_attribute("hit_points") = enemy->hitPointsCurrent;

				break;


			case ENTITY_TYPE::BUILDING:

				iterator.append_attribute("type") = "building";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				building = (Building*)entityVector[i];

				iterator.append_attribute("decor") = (int)building->GetDecor();

				break;


			case ENTITY_TYPE::BLDG_BASE:

				iterator.append_attribute("type") = "base";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				iterator.append_attribute("aligment") = (int)entityVector[i]->GetAlignment();

				base = (Base*)entityVector[i];

				iterator.append_attribute("hit_points") = base->hitPointsCurrent;

				turretVector = base->GetTurretVector();
				barricadeVector = base->GetBarricadeVector();
				upgradeCenter = base->GetUpgradeCenter();

				for (int i = 0; i < turretVector->size(); i++)
				{
					iterator2 = iterator.append_child("turret");
					iterator2.append_attribute("name") = "turret";
					iterator2.append_attribute("x") = turretVector->operator[](i)->position.x;
					iterator2.append_attribute("y") = turretVector->operator[](i)->position.y;

					iterator.append_attribute("hit_points") = turretVector->operator[](i)->hitPointsCurrent;
					iterator2.append_attribute("level") = turretVector->operator[](i)->GetLvl();
				}

				for (int i = 0; i < barricadeVector->size(); i++)
				{
					iterator2 = iterator.append_child("barricade");
					iterator2.append_attribute("name") = "barricade";
					iterator2.append_attribute("x") = barricadeVector->operator[](i)->position.x;
					iterator2.append_attribute("y") = barricadeVector->operator[](i)->position.y;

					iterator.append_attribute("hit_points") = barricadeVector->operator[](i)->hitPointsCurrent;
					iterator2.append_attribute("level") = barricadeVector->operator[](i)->GetLevel();
				}

				if (upgradeCenter != nullptr)
				{
					iterator2 = iterator.append_child("upgrade_center");
					iterator2.append_attribute("name") = "upgrade_center";

					iterator2.append_attribute("x") = upgradeCenter->position.x;
					iterator2.append_attribute("y") = upgradeCenter->position.y;

					iterator2.append_attribute("turretLvl") = upgradeCenter->GetTurretLevel();
					iterator2.append_attribute("barricadeLvl") = upgradeCenter->GetBarricadeLevel();
				}

				break;

			case ENTITY_TYPE::QUEST:

				iterator.append_attribute("type") = "quest";

				iterator.append_attribute("x") = entityVector[i]->position.x;
				iterator.append_attribute("y") = entityVector[i]->position.y;

				quest = (Quest*)entityVector[i];

				iterator.append_attribute("id") = quest->GetId();

				break;

			case ENTITY_TYPE::MAX_TYPE:
				break;
			default:
				break;
			}
		}
	}


	//deathEntitesSaving
	pugi::xml_node deadEntities = data.append_child("deadEntities");

	if (CheckIfHeroIsDead(ENTITY_TYPE::HERO_GATHERER) == true)
	{
		SaveDeadHero(deadEntities.append_child("deadGatherer"), ENTITY_TYPE::HERO_GATHERER);
	}
	if (CheckIfHeroIsDead(ENTITY_TYPE::HERO_RANGED) == true)
	{
		SaveDeadHero(deadEntities.append_child("deadRanged"), ENTITY_TYPE::HERO_RANGED);
	}
	if (CheckIfHeroIsDead(ENTITY_TYPE::HERO_MELEE) == true)
	{
		SaveDeadHero(deadEntities.append_child("deadMelee"), ENTITY_TYPE::HERO_MELEE);
	}
	if (CheckIfHeroIsDead(ENTITY_TYPE::HERO_ROBO) == true)
	{
		SaveDeadHero(deadEntities.append_child("deadRobo"), ENTITY_TYPE::HERO_ROBO);
	}


	return true;
}


void ModuleEntityManager::ResetUpgradeValues()
{
	gathererLifeUpgradeValue = 1;
	gathererDamageUpgradeValue = 1;
	gathererEnergyUpgradeValue = 1;
	gathererAtkSpeedUpgradeValue = 1;
	meleeLifeUpgradeValue = 1;
	meleeDamageUpgradeValue = 1;
	meleeEnergyUpgradeValue = 1;
	meleeAtkSpeedUpgradeValue = 1;
	rangedLifeUpgradeValue = 1;
	rangedDamageUpgradeValue = 1;
	rangedEnergyUpgradeValue = 1;
	rangedAtkSpeedUpgradeValue = 1;
	robottoLifeUpgradeValue = 1;
	robottoDamageUpgradeValue = 1;
	robottoEnergyUpgradeValue = 1;
	robottoAtkSpeedUpgradeValue = 1;
}

HeroStats::HeroStats() : maxHP(-1), damage(-1), maxEnergy(-1), atkSpeed(-1), recoveryHPRate(-1), recoveryEnergyRate(-1),
heroLevel(-1), movSpeed(-1), visionDistance(-1), attackRange(-1), xpToLvlUp(-1), currHP(-1), currEnergy(-1)
{}

HeroStats::HeroStats(HeroStats& newStats) : maxHP(newStats.maxHP), damage(newStats.damage), maxEnergy(newStats.maxEnergy), atkSpeed(newStats.atkSpeed), recoveryHPRate(newStats.recoveryHPRate),
recoveryEnergyRate(newStats.recoveryEnergyRate), heroLevel(newStats.heroLevel), movSpeed(newStats.movSpeed), visionDistance(newStats.visionDistance), attackRange(newStats.attackRange), xpToLvlUp(newStats.xpToLvlUp),
currHP(newStats.currHP), currEnergy(newStats.currEnergy)
{}

HeroStats HeroStats::operator=(HeroStats& newStats)
{
	this->atkSpeed = newStats.atkSpeed;
	this->attackRange = newStats.attackRange;
	this->currEnergy = newStats.currEnergy;
	this->currHP = newStats.currHP;
	this->damage = newStats.damage;
	this->heroLevel = newStats.heroLevel;
	this->maxEnergy = newStats.maxEnergy;
	this->maxHP = newStats.maxHP;
	this->movSpeed = newStats.movSpeed;
	this->recoveryEnergyRate = newStats.recoveryEnergyRate;
	this->recoveryHPRate = newStats.recoveryHPRate;
	this->visionDistance = newStats.visionDistance;
	this->xpToLvlUp = newStats.xpToLvlUp;


	return (*this);
}


int ModuleEntityManager::CheckPlayerBases()
{
	int numEntities = entityVector.size();
	int baseNumber = 0;

	for (int i = 0; i < numEntities; i++)
	{
		if (entityVector[i]->GetAlignment() == ENTITY_ALIGNEMENT::PLAYER && entityVector[i]->GetType() == ENTITY_TYPE::BLDG_BASE)
		{
			baseNumber++;
		}
	}

	return baseNumber;
}


