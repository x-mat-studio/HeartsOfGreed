#ifndef __WIN_SCENE__
#define __WIN_SCENE__

#include "Module.h"



class ModuleWinScene : public Module
{
public:

	ModuleWinScene();

	// Destructor
	virtual ~ModuleWinScene();

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

	void ExecuteEvent(EVENT_ENUM eventId) const;

private:

};

#endif // __WIN_SCENE__