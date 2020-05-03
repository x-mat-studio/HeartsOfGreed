#ifndef __DATA_PAGES_H__
#define __DATA_PAGES_H__

#include "UI.h"

class Entity;
class UIFactory;

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

	DataPages(UI* parent, Entity* entity);
	~DataPages();


private:

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	bool CheckData(int previous, int current);
	void ChangeTexture();

	// Create Data Page
	void CreateGathererPage();
	void CreateMeleePage();
	void CreateRangedPage();
	void CreateWanamingoPage();
	void CreateBasePage();
	void CreateTurretPage();
	void CreateUpgradeCenterPage();
	void CreateBarricadePage();

	// Check Data Page Values
	void CheckGathererValues();
	void CheckMeleeValues();
	void CheckRangedValues();
	void CheckWanamingoValues();
	void CheckBaseValues();
	void CheckTurretValues();
	void CheckUpgradeCenterValues();
	void CheckBarricadeValues();


	void DeleteCurrentData();


private:

	std::vector<UI*> dataPageVector;
	DATA_PAGE_ENUM state;
	Entity* focusEntity;
	UIFactory* factory;

};



#endif //__DATA_PAGES_H__
