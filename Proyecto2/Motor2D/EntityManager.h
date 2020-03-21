#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "SDL/include/SDL.h"
#include "Module.h"
#include "Animation.h"
#include "Entity.h"

class Hero;



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


	Entity* CheckEntityOnClick(SDL_Point mousePos);
	void CheckHeroOnSelection(SDL_Rect &selection, std::vector<Hero*> *heroVector);
	bool CheckEntityExists(Entity* entity);

private:

	void CheckIfStarted();

private:

	//The list where we will store all the entities
	std::vector <Entity*> entityVector;
	std::vector <Hero*> heroVector;

	// Sprite sorting vectors
	std::vector <Entity*> renderVector;
	std::vector <Entity*> movableEntitiesVector;
	std::vector <Entity*> buildingVector;

	SDL_Texture* texture;

	Hero* tmpHero;

};

#endif //__ENTITYMANAGER_H__