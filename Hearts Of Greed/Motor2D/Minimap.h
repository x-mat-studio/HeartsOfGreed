#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "Module.h"
#include "SDL/include/SDL.h"

enum class MINIMAP_ICONS
{
	BASE,
	TURRET,
	HERO,
	ENEMY,
	ENEMY_BASE,
	NONE
};



class MinimapIcon
{
public:
	MinimapIcon(fMPoint* worldPos, MINIMAP_ICONS type, fMPoint &offSet);
	~MinimapIcon();
	void Draw(SDL_Rect sourceRect);
public:

	fMPoint* minimapPos;
	fMPoint  offSet;
	MINIMAP_ICONS type;

	bool toDelete;
};



class Minimap :public Module
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

	MinimapIcon* CreateIcon(fMPoint* worldPos, MINIMAP_ICONS type, fMPoint& offset);


public:
	float minimapScaleRelation;
	int minimapWidth;
	int height;
	iMPoint position;
private:
	void ExecuteEvent(EVENT_ENUM eventId);
	int width;

	std::vector<MinimapIcon*> minimapIcons;

	int minimapHeight;

	SDL_Texture* minimapTexture;
	SDL_Rect camRect;

	bool minimapLoaded;

};

#endif //!__MINIMAP_H__