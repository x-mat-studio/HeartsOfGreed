#ifndef __DATA_PAGES_H__
#define __DATA_PAGES_H__

#include "UI.h"

class Entity;
struct UIFactory;
enum class ENTITY_ALIGNEMENT;

enum class DATA_PAGE_ENUM
{
	FOCUSED_NONE,

	FOCUSED_BARRICADE,
	FOCUSED_BASE,
	FOCUSED_TURRET,
	FOCUSED_UPGRADE_CENTER,

	FOCUSED_GATHERER,
	FOCUSED_MELEE,
	FOCUSED_RANGED,

	FOCUSED_WANAMINGO,

	FOCUSED_UNKNOWN,
};

class DataPages : public UI
{
public:

	DataPages(float x, float y, UI* parent, Entity* entity);
	~DataPages();

	bool OnAbove();
	UI* SearchFocus();
	void UnFocus();

private:

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	bool CheckData(float previous, float current);
	void AdjustHealthBars(int newValue, int maxValue);
	void AdjustManaBars(int newValue, int maxValue);

	// Check Data Page Values
	void CheckHeroesValues();
	void CheckWanamingoValues();
	void CheckBaseValues();
	void CheckTurretValues();
	void CheckUpgradeCenterValues();
	void CheckBarricadeValues();

	// Get Data Page Values

	void GetHeroValue();
	void GetWanamingoValue();
	void GetBaseValue();
	void GetTurretValue();
	void GetUpgradeCenterValue();
	void GetBarricadeValue();
	void GetHealthBarValues();

	void DeleteCurrentData();


private:

	std::vector<UI*> dataPageVector;
	DATA_PAGE_ENUM state;
	Entity* focusEntity;
	UIFactory* factory;

	SDL_Rect* healthRect;
	SDL_Rect* manaRect;
	int originalBarsWidth;

	float life;
	float mana;
	float resources;
	float level;
	float attackDamage;
	float attackSpeed;
	float range;
	float vision;
	float hpRecovery;
	float xpToNextLevel;
	ENTITY_ALIGNEMENT alignment;

};



#endif //__DATA_PAGES_H__
