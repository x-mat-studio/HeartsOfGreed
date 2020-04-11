#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Minimap:public Module
{
public:
	Minimap();
	~Minimap();

	bool Awake(pugi::xml_node& config);
	bool Start();

	bool PreUpdate(float dt);

	bool Update(float dt);

	bool PostUpdate(float dt);

	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	void CreateMinimapText();
	void LoadMinimap();

public:
	float minimapScaleRelation;	
	int minimapWidth;

private:
	void ExecuteEvent(EVENT_ENUM eventId) const;
	int width;
	int height;

	int minimapHeight;
	iMPoint position;

	SDL_Surface* mapSurface;
	SDL_Renderer* minimapRenderer;
	SDL_Texture* minimapTexture;

	bool minimapLoaded;

};

#endif //!__MINIMAP_H__