#ifndef __MAIN_MENU_SCENE__
#define __MAIN_MENU_SCENE__

#include "Module.h"



class ModuleMainMenuScene : public Module
{
public:

	ModuleMainMenuScene();

	// Destructor
	virtual ~ModuleMainMenuScene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();


	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:

	void ExecuteEvent(EVENT_ENUM eventId);

private:

	bool changeScene;

	

};

#endif // __MAIN_MENU_SCENE__