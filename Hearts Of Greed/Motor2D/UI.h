#ifndef __UI_H__
#define __UI_H__

#include "SDL/include/SDL_rect.h"
#include "App.h"
#include "p2Point.h"
#include "Animation.h"
#include "UIManager.h"
#include "p2SString.h"

struct SDL_Texture;

enum class UI_TYPE : int
{
	NONE = -1,

	TEXT,
	IMG,
	SCROLLBAR,
	BUTTON,
	HEALTHBAR,
	
	RESOURCES_COUNTER,

	MAX
};

class UI
{
public:

	UI();
	UI(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, bool interactable, bool dragable, SDL_Texture* texture);
	virtual ~UI();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void Drag(int x, int y);

	bool OnAbove();

	fMPoint GetPosition();
	fMPoint GetLocalPosition();

	bool GetDragable();

protected:

	virtual void HandleInput();
	virtual void Draw(float dt);
	virtual void Move();
	

public:

	bool focused;

	SDL_Rect rect;

protected:

	fMPoint position;
	fMPoint localPosition;

	SDL_Texture* texture;

	UI* father;

private:

	bool dragable;
	bool interactable;

	UI_TYPE type;
};

#endif//__UI_H__