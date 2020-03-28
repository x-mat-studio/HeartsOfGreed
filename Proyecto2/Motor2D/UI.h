#ifndef __UI_H__
#define __UI_H__

#include "SDL/include/SDL_rect.h"
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;

enum class UI_TYPE
{
};

class UI
{
public:

	UI();
	UI(fMPoint positionValue, UI* father, UI_TYPE uiType, P2SString uiName);
	virtual ~UI();

	bool operator==(UI* element);

	virtual bool Start(SDL_Texture* texture);
	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);
	virtual bool Draw();

	bool MouseUnderElement(int x, int y);
	void Drag(int x, int y);

protected:

	virtual void Move();
	virtual void HandleInput();

public:
	
	bool debugBox;
	bool dragable;
	bool focused;

protected:

	fMPoint worldPosition;
	fMPoint localPosition;

	SDL_Rect* box;
	SDL_Texture* texture;
	UI* parent;
	UI_TYPE type;

	bool toDelete;
	bool started;

	P2SString name;

};

#endif//__UI_H__