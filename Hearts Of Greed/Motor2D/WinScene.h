#ifndef __WIN_SCENE__
#define __WIN_SCENE__

#include "Module.h"
#include "SDL/include/SDL.h"

#include "EasingFunctions.h"

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
	//images
	SDL_Texture* youWon;
	SDL_Texture* medalWin;

	fMPoint medalPos;

	float fadeTime;
	
	//sounds
	int medalBounce;

	Easing iconPosY;
	float bufferPos;
	bool medalRest;

};

#endif // __WIN_SCENE__