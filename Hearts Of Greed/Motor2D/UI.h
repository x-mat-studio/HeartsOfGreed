#ifndef __UI_H__
#define __UI_H__

#include "SDL/include/SDL_rect.h"
#include "App.h"
#include "p2Point.h"
#include "Animation.h"
#include "UIManager.h"
#include "p2SString.h"

struct SDL_Texture;

enum class UI_TYPE
{
	UI_TEXT,
	UI_IMG,
	UI_SCROLLBAR,
	UI_BUTTON,
	UI_HEALTHBAR,
	UI_PORTRAIT,

	UI_NONE
};

class UI
{
public:

	UI();
	UI(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE dragable);
	virtual ~UI();


	virtual bool Start();
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);
	virtual void CleanUp();
	void Draw(SDL_Texture* texture);

	bool MouseUnderElement(int x, int y);
	void Drag(int x, int y);

	bool OnAbove();

protected:

	virtual void Move();
	virtual void HandleInput();

public:
	
	bool debugBox;
	bool hover;

	bool hiding_unhiding;
	bool hidden;

	float hideSpeed;
	float defaultPosition;

	bool dragging;

	bool focused;
	bool enabled;
	bool interactable;

	fMPoint worldPosition;
	fMPoint localPosition;

	UI_TYPE type;
	SDL_Rect box;

	UI* parent;

protected:

	SDL_Texture* texture;
	DRAGGABLE draggable;

};

#endif//__UI_H__