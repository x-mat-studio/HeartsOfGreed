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
	ENEMY_TURRET,
	QUEST,
	NONE
};

class Entity;

class MinimapIcon
{
public:
	MinimapIcon(fMPoint* worldPos, MINIMAP_ICONS type, fMPoint &offSet, Entity* parent = nullptr);
	~MinimapIcon();
	void Draw(SDL_Rect sourceRect);
	void SetActiveState(bool isActive);
	bool IsActive()const;

public:

	fMPoint* minimapPos;
	fMPoint  offSet;
	MINIMAP_ICONS type;

	Entity* parent;
	
	bool toDelete;

private:

	bool active;
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
	void UpdateMinimapFoW();
	void MinimapFoWNeedsUpdate();
	void DeleteMinimapIcons();


	bool ClickingOnMinimap(int x, int y);
	iMPoint WorldToMinimap(int x, int y);
	iMPoint ScreenToMinimapToWorld(int x, int y);

	MinimapIcon* CreateIcon(fMPoint* worldPos, MINIMAP_ICONS type, fMPoint& offset, Entity* parent = nullptr);

	void SetAllIconsActiveState(bool areActive);

private:
	void ExecuteEvent(EVENT_ENUM eventId);

public:
	float minimapScaleRelation;
	int minimapWidth;
	int height;
	iMPoint position;
private:
	int width;

	std::vector<MinimapIcon*> minimapIcons;

	int minimapHeight;

	SDL_Texture* minimapTexture;
	SDL_Texture* minimapFrame;
	SDL_Rect miniFrame;
	SDL_Rect positionFrame;
	SDL_Rect camRect;
	

	bool minimapLoaded;
	bool minimapFoWNeedsUpdate;

};

#endif //!__MINIMAP_H__