#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "SDL/include/SDL.h"
#include "Module.h"
#include "vector"
#include "Entity.h"


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
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	//// Called before quitting
	bool CleanUp();

	//// Load / Save
	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const { return true; };


	//Adds
	bool AddEntity(ENTITY_TYPE type, int x, int y);

private:

	//The list where we will store all the entities
	std::vector <Entity> entityVector;


};

#endif //__ENTITYMANAGER_H__