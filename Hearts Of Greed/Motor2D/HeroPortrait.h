#ifndef __HERO_PORTRAIT_H__
#define __HERO_PORTRAIT_H__

#include "UI.h"
#include "vector"

class Hero;
enum class ENTITY_TYPE;
struct SDL_Rect;

class HeroPortrait : public UI
{
public:
	HeroPortrait(Hero* hero);
	~HeroPortrait();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void AddElement(UI* element);
	void AddHealthBar(UI* element);
	void AddManaBar(UI* element);

	Hero* GetHero();

private:
	Hero* hero;

	std::vector<UI*> uiElementsVector;

	ENTITY_TYPE heroType;

	int originalBarsWidth;
	SDL_Rect* healthRect;
	SDL_Rect* manaRect;

	float life;
	float mana;

	bool focused;
};



#endif //__HERO_PORTRAIT_H__