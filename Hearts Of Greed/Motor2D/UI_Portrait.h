#ifndef __UI_PORTRAIT_H__
#define __UI_PORTRAIT_H__

#include "UI.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Healthbar.h"
#include "Hero.h"

struct Portrait
{
	fMPoint position;
	UI_Image* background;
	UI_Image* portrait;
	UI_Healthbar* healthbar;
	UI_Image* backgroundLevel;
	UI_Image* backgroundHealthbar;
	UI_Text* level;
	Hero* hero;
};

class UI_Portrait : public UI
{

public:

	UI_Portrait(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE draggable);
	~UI_Portrait();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void CreatePortrait(Hero* entity);
	void DeletePortrait(Portrait portrait);

private:

	void HandleInput();
	void Move();
	void CheckLevel();
	void ReWriteLevelTexture();
	SDL_Rect RectConstructor(int x, int y, int w, int h);

public:

	std::vector<Portrait> portraitVector;
	int nextVectorPosition;

private:

	SDL_Rect backgroundRect;
	SDL_Rect backgroundLevelRect;
	SDL_Rect backgroundHealthbarRect;
	SDL_Rect healthbarRect;
	SDL_Rect meleePortraitRect;
	SDL_Rect gathererPortraitRect;
	SDL_Rect rangedPortraitRect;

	bool no_move;

};

#endif // UI_HEALTHBAR_H
