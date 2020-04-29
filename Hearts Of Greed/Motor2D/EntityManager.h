#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "SDL/include/SDL.h"
#include "Module.h"
#include "Animation.h"
#include "Entity.h"
#include <list>
#include <unordered_map>

class Hero;
class GathererHero;
class MeleeHero;
class RangedHero;

class Building;
class DynamicEntity;
class Enemy;
class Spawner;
class Base;
class Turret;

enum class AREA_TYPE
{
	NO_TYPE = -1,

	CIRCLE,
	QUAD,
	CONE,

};


struct skillArea
{
	unsigned short* area = nullptr;
	AREA_TYPE form = AREA_TYPE::NO_TYPE;
	int radius = 0, width = 0, heigth = 0;
};

enum class SPRITE_POSITION : int
{
	INVALID = -1,

	BEHIND_BUILDING,
	HIGHER_THAN_BUILDING,
	LOWER_THAN_BUILDING,
	NULL_BUILDING,
	NULL_MOVABLE_ENTITY,
	BOTH_NULL
};

class ModuleEntityManager : public Module
{
public:

	ModuleEntityManager();
	virtual ~ModuleEntityManager();


	bool Awake(pugi::xml_node&);
	bool Start();


	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	
	bool CleanUp();

	
	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) { return true; };


	void OnCollision(Collider*, Collider*);


	Entity* AddEntity(ENTITY_TYPE type, int x, int y, ENTITY_ALIGNEMENT alignement = ENTITY_ALIGNEMENT::NEUTRAL);
	Entity* GetSample(ENTITY_TYPE);


	void RemoveDeletedEntities();
	void DeleteAllEntities();


	Entity* CheckEntityOnClick(iMPoint mousePos, bool focus = true);
	void CheckHeroOnSelection(SDL_Rect &selection, std::vector<Hero*> *heroVector);
	void CheckDynamicEntitysObjectives(Entity* entity);


	void SearchHeroesAlive();
	void SearchEnemiesAlive();


	Entity* SearchEntityRect(SDL_Rect* rect, ENTITY_ALIGNEMENT alig);


	void GetEntityNeighbours(std::vector<DynamicEntity*>* close_entity_list, std::vector<DynamicEntity*>* colliding_entity_list, DynamicEntity* thisUnit);


	void PlayerBuildPreview(int x, int y, ENTITY_TYPE type);

	Hero* CheckUIAssigned(int& anotherHeroWithoutUI);

	Entity* SearchUnitsInRange(float distance, Entity* from);


	void ActivateGodModeHeroes();
	void DesactivateGodModeHeroes();

	void KillAllEnemies();

	skillArea* RequestArea(SKILL_ID id, std::vector<iMPoint>* toFill, iMPoint center);

	//This & skill Struct need re-work to accept single target
	int ExecuteSkill(int dmg, iMPoint pivot, skillArea* area, ENTITY_ALIGNEMENT target, SKILL_TYPE type,bool hurtYourself = false,  Entity* objective = nullptr);

	//function used for minimap
	void DrawOnlyStaticBuildings();

	void ResetEntityManager();

private:

	void CheckIfStarted();

	void ExecuteEvent(EVENT_ENUM eventId);

	//Sprite ordering
	void SpriteOrdering(float dt);
	void EntityQuickSort(std::vector<Entity*>& vector, int low, int high);
	int EntityPartition(std::vector<Entity*>& vector, int low, int high);
	SPRITE_POSITION CheckSpriteHeight(Entity* movEntity, Entity* building) const;

	//Area Related
	bool BuildArea(skillArea* areaToGenerate, int width, int heigth, int radius);
	unsigned short* BuildCircleArea(int radius);
	unsigned short* BuildQuadArea(int width, int height);

	void GenerateDynArea(std::vector <iMPoint>* toFill, skillArea* area, iMPoint center);

	bool LoadSampleHero(ENTITY_TYPE heroType, pugi::xml_node& heroNode, pugi::xml_node& config);
	bool LoadSampleEnemy(pugi::xml_node& enemyNode);
	bool LoadSampleTurret(pugi::xml_node& turretNode);


public:

	SDL_Texture* selectedTexture;
	SDL_Texture* targetedTexture;
	SDL_Texture* debugPathTexture;
	SDL_Texture* moveCommandTile;
	SDL_Texture* explosionTexture;

	
	//Sounds
	int wanamingoRoar;
	int wanamingoRoar2;
	int wanamingoGetsHit;
	int wanamingoDies;
	int wanamingoDies2;	
	
	int suitmanGetsHit;
	int suitmanGetsHit2;
	int suitmanGetsDeath;
	int suitmanGetsDeath2;

	int buildingGetsHit;
	int buildingGetsHit2;

	int turretShooting;

	int suitman1Skill;

	int suitman1Skill2;
	int armored1Skill2;

	int noise1Suitman;
	int noise2Suitman;
	int noise3Suitman;
	int noise4Suitman;

	int noise1Armored;
	int noise2Armored;
	int noise3Armored;
	int noise4Armored;

	int lvlup;
	int selectHero;
	int moveHero;


	//public textures
	SDL_Texture* base2Texture;
	SDL_Texture* base2TextureEnemy;
	SDL_Texture* base2TextureSelected;
	SDL_Texture* base2TextureSelectedEnemy;

private:

	//The list where we will store all the entities
	std::vector <Entity*> entityVector;

	// Sprite sorting vectors
	std::vector <Entity*> renderVector;
	std::vector <Entity*> movableEntityVector;
	std::vector <Entity*> buildingVector;
	std::vector <Entity*> selectedVector;

	SDL_Texture* suitManTexture;
	SDL_Texture* armorMaleTexture;
	SDL_Texture* combatFemaleTexture;
	SDL_Texture* buildingTexture;

	
	SDL_Texture* base1Texture;

	

	SDL_Texture* deco3Selected;

	SDL_Texture* turretTexture;

	SDL_Texture* enemyTexture;

	//Samples

	GathererHero* sampleGatherer;
	MeleeHero* sampleMelee;
	RangedHero* sampleRanged;

	Enemy* sampleEnemy;

	Spawner* sampleSpawner;

	Building* sampleBuilding;
	Base* sampleBase;

	Turret* sampleTurret;

	std::unordered_map <SKILL_ID, skillArea> skillAreas;
};

#endif //__ENTITYMANAGER_H__