#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "Module.h"
#include "SDL/include/SDL.h"

enum class MINIMAP_ICONS
{
	BASE,
	TURRET,
	HEROE,
	ENEMY,
	ENEMY_BASE,
	NONE
};



class MinimapIcon
{
public:
public:
	fMPoint pos;
	MINIMAP_ICONS type;

	bool toDelete;
};

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

	bool ClickingOnMinimap(int x, int y);
	iMPoint WorldToMinimap(int x, int y);
	iMPoint ScreenToMinimapToWorld(int x, int y);



public:
	float minimapScaleRelation;	
	int minimapWidth;
	int height;

private:
	void ExecuteEvent(EVENT_ENUM eventId);
	int width;
	

	int minimapHeight;
	iMPoint position;

	SDL_Texture* minimapTexture;
	SDL_Rect camRect;

	bool minimapLoaded;

};

#endif //!__MINIMAP_H__