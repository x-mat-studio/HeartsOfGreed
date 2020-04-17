#ifndef __UI_HEALTHBAR_H__
#define __UI_HEALTHBAR_H__

#include "UI.h"
#include "Entity.h"

class UI_Healthbar : public UI
{

public:

	UI_Healthbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, Entity* entity, DRAGGABLE draggable, bool isEnergy = false);
	~UI_Healthbar();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	void EntityDeath();

private:

	void HandleInput();
	void AdjustValue();

private:

	int* maxValue;
	int* currentValue;
	int previousValue;
	int originalWidth;
	Entity* entity;

	bool isEnergy;

};

#endif // UI_HEALTHBAR_H

