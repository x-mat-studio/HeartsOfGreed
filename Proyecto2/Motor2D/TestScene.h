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
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();


	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Zoom(float addZoomAmount, int windowTargetCenterX, int windowTargetCenterY, float currentScale);


private:
	
	void ExecuteEvent(EVENT_ENUM& eventId) const;

private:
	int prevMousePosX;
	int prevmousePosY;

};

#endif // __TEST_SCENE_H__