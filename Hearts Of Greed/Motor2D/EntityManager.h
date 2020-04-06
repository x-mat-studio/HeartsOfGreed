#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "SDL/include/SDL.h"
#include "Module.h"
#include "Animation.h"
#include "Entity.h"
#include <list>

class Hero;
class Building;
class DynamicEntity;
class Enemy;
class Spawner;

enum class SPRITE_POSITION
{
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


	void RemoveDeletedEntitys();


	Entity* CheckEntityOnClick(iMPoint mousePos);
	void CheckHeroOnSelection(SDL_Rect &selection, std::vector<Hero*> *heroVector);
	void CheckDynamicEntitysObjectives(Entity* entity);


	Entity* SearchEntityRect(SDL_Rect* rect, ENTITY_ALIGNEMENT alig);


	void GetEntityNeighbours(std::vector<DynamicEntity*>* close_entity_list, std::vector<DynamicEntity*>* colliding_entity_list, DynamicEntity* thisUnit);


	void PlayerBuildPreview(int x, int y, ENTITY_TYPE type);

private:

	void CheckIfStarted();

	void ExecuteEvent(EVENT_ENUM eventId);

	//Sprite ordering
	void SpriteOrdering(float dt);
	void EntityQuickSort(std::vector<Entity*>& vector, int low, int high);
	int EntityPartition(std::vector<Entity*>& vector, int low, int high);
	SPRITE_POSITION CheckSpriteHeight(Entity* movEntity, Entity* building) const;


public:

	SDL_Texture* debugPathTexture;


private:

	//The list where we will store all the entities
	std::vector <Entity*> entityVector;

	// Sprite sorting vectors
	std::vector <Entity*> renderVector;
	std::vector <Entity*> movableEntityVector;
	std::vector <Entity*> buildingVector;

	SDL_Texture* suitManTexture;
	SDL_Texture* armorMaleTexture;
	SDL_Texture* combatFemaleTexture;

	SDL_Texture* buildingTexture;
	SDL_Texture* base1Texture;
	SDL_Texture* base2Texture;

	SDL_Texture* enemyTexture;

	Hero* sampleMelee;

	Enemy* sampleEnemy;

	Spawner* testSpawner;

	Building* testBuilding;
	Building* blueBuilding;

};

#endif //__ENTITYMANAGER_H__