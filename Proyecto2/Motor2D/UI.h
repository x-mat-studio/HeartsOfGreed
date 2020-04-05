#ifndef __UI_H__
#define __UI_H__

#include "SDL/include/SDL_rect.h"
#include "p2Point.h"
#include "Animation.h"
#include "UIManager.h"
#include "p2SString.h"

struct SDL_Texture;

enum class UI_TYPE
{
	UI_IMG,
	UI_BUTTON,
	UI_TEXT
};

class UI
{
public:

	UI();
	UI(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, P2SString uiName, bool dragable);
	virtual ~UI();

	bool operator==(UI* element);

	virtual bool Start();
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);
	void Draw(SDL_Texture* texture);

	bool MouseUnderElement(int x, int y);
	void Drag(int x, int y);

protected:

	void Move();
	virtual void HandleInput();

public:
	
	bool debugBox;
	bool dragable;
	bool focused;
	bool toDelete;

protected:

	fMPoint worldPosition;
	fMPoint localPosition;

	SDL_Rect box;
	SDL_Texture* texture = nullptr;

	UI* parent;
	UI_TYPE type;

	P2SString name;

};

#endif//__UI_H__