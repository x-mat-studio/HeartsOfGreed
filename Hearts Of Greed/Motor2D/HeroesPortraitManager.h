#ifndef __HEROES_PORTRAIT_MANAGER_H__
#define __HEROES_PORTRAIT_MANAGER_H__

#include "UI.h"
#include "vector"

class HeroPortrait;
class Hero;

class HeroesPortraitManager : public UI
{
public:

	HeroesPortraitManager(float x, float y, UI* parent, bool dragable);
	~HeroesPortraitManager();

	void AddPortrait(HeroPortrait* portrait);

	void DeletePortrait(Hero* portrait);

	bool OnAbove();
	UI* SearchFocus();
	void UnFocus();

private:

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);


private:

	std::vector<HeroPortrait*> heroPortraitsVector;
};



#endif //__HEROES_PORTRAIT_MANAGER_H__