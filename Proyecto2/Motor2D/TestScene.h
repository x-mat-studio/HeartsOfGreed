#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "Module.h"



class ModuleTestScene : public Module
{
public:

	ModuleTestScene();

	// Destructor
	virtual ~ModuleTestScene();

	// Called before render is available
	bool Awake(pugi::xml_node&);
	
	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:
	

private:

};

#endif // __TEST_SCENE_H__