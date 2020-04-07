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
	UI_IMG,
	UI_BUTTON,
	UI_TEXT,
	UI_HEALTHBAR,
	UI_SCROLLBAR,

	UI_NONE
};



class UI
{
public:

	UI();
	UI(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, DRAGGABLE dragable);
	virtual ~UI();

	bool operator==(UI* element);

	virtual bool Start();
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);
	virtual void CleanUp();
	void Draw(SDL_Texture* texture);

	bool MouseUnderElement(int x, int y);
	void Drag(int x, int y);

protected:

	void Move();
	virtual void HandleInput();

public:
	
	bool debugBox;
	bool hover;

	bool dragging;

	bool focused;
	bool toDelete;
	bool enabled;
	bool interactable;

protected:

	fMPoint worldPosition;
	fMPoint localPosition;

	SDL_Rect box;
	SDL_Texture* texture;

	UI* parent;
	UI_TYPE type;
	DRAGGABLE draggable;

	P2SString name;

};

#endif//__UI_H__