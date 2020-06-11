#ifndef __INTRO_SCENE__
#define __INTRO_SCENE__

#include "Module.h"
#include "SDL/include/SDL.h"



class ModuleIntroScene : public Module
{
public:

	ModuleIntroScene();

	// Destructor
	virtual ~ModuleIntroScene();

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
	//images
	SDL_Texture* logoXMat;
	SDL_Texture* logoBG;

	//sounds
	int logoSound;
	int titleSound;

	float logoStartingValue;
	float bgStartingValue;

	float logoAlphaCounter; 
	float bgAlphaCounter;

	float logoTransitionConst;
	float bgTransitionConst;

	float logoTopValue;
	float bgTopValue;

	bool logoValueDown;
	bool bgValueDown;

};

#endif // __INTRO_SCENE__