#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "SDL/include/SDL.h"
#include "Module.h"
#include "vector"

enum  class ENTITY_TYPES
{
	UNKNOWN,
	PARTICLE,
	EMITER,
	PARTICLE_SYSTEM,
	HERO_MELEE,
	HERO_RANGED,
	HERO_GATHERER,
	ENEMY,
	BLDG,
	BLDG_UPGRADE,
	BLDG_BASE,
	BLDG_BARRICADE,

	MAX_TYPE
};

class Entity;

class ModuleEntityManager : public Module
{
public:

	ModuleEntityManager();

	// Destructor
	virtual ~ModuleEntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	//bool Update(float dt);
	//bool PostUpdate();

	//// Called before quitting
	//bool CleanUp();

	//// Load / Save
	//bool Load(pugi::xml_node&);
	//bool Save(pugi::xml_node&) const;

private:

	//The list where we will store all the entities
	//std::vector <Entity> entityVector;


};

#endif //__ENTITYMANAGER_H__