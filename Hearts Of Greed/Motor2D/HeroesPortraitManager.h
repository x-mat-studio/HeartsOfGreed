#ifndef __HEROES_PORTRAIT_MANAGER_H__
#define __HEROES_PORTRAIT_MANAGER_H__

#include "UI.h"
#include "vector"

class HeroPortrait;

class HeroesPortraitManager : public UI
{
public:
	HeroesPortraitManager(float x, float y, UI* parent, bool dragable);
	~HeroesPortraitManager();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void AddPortrait(HeroPortrait* portrait);

	void DeletePortrait(HeroPortrait* portrait);
private:

	std::vector<HeroPortrait*> heroPortraitsVector;
};




#endif //__HEROES_PORTRAIT_MANAGER_H__