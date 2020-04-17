#ifndef __UI_HEALTHBAR_H__
#define __UI_HEALTHBAR_H__

#include "UI.h"
#include "Entity.h"

class UI_Healthbar : public UI
{

public:

	UI_Healthbar(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, Entity* entity, DRAGGABLE draggable);
	~UI_Healthbar();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	void EntityDeath();

private:

	void HandleInput();
	void AdjustHealth();

private:

	int* maxHealth;
	int* currentHealth;
	int previousHealth;
	int originalWidth;
	Entity* entity;

};

#endif // UI_HEALTHBAR_H

