#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "SDL/include/SDL.h"
#include "Module.h"
#include "Animation.h"
#include "Entity.h"
#include <list>

class Hero;
class DynamicEntity;


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


	Entity* AddEntity(ENTITY_TYPE type, int x, int y);


	void RemoveDeletedEntitys();


	Entity* CheckEntityOnClick(iMPoint mousePos);
	void CheckHeroOnSelection(SDL_Rect &selection, std::vector<Hero*> *heroVector);
	bool CheckEntityExists(Entity* entity);
	Entity* CheckEnemyObjective(SDL_Rect* rect);

	void GetEntityNeighbours(std::list<DynamicEntity*>* close_entity_list, std::list<DynamicEntity*>* colliding_entity_list, DynamicEntity* thisUnit);

public:
	SDL_Texture* debugPathTexture;

private:

	void CheckIfStarted();
	void ExecuteEvent(EVENT_ENUM& eventId) const;
	void SpriteOrdering(float dt);
	void EntityQuickSort(std::vector<Entity*>& vector, int low, int high);
	int EntityPartition(std::vector<Entity*>& vector, int low, int high);

private:

	//The list where we will store all the entities
	std::vector <Entity*> entityVector;

	// Sprite sorting vectors
	std::vector <Entity*> renderVector;
	std::vector <Entity*> backEntitiesVector;
	std::vector <Entity*> frontEntitiesVector;
	std::vector <Entity*> buildingVector;

	SDL_Texture* texture;

	Hero* tmpHero;

};

#endif //__ENTITYMANAGER_H__