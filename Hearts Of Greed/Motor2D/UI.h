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
	PORTRAIT,

	MAX
};

class UI
{
public:

	UI();
	UI(fMPoint positionValue, UI* father, UI_TYPE uiType, SDL_Rect rect, bool interactable, bool dragable, SDL_Texture* texture);
	virtual ~UI();

	virtual bool PreUpdate(float dt);
	virtual bool Update(float dt);
	virtual bool PostUpdate(float dt);

	void Drag(int x, int y);

	bool OnAbove();

	fMPoint GetPosition();
	fMPoint GetLocalPosition();


protected:

	virtual void Draw(float dt);
	virtual void Move();
	

protected:

	bool focused;

	fMPoint position;
	fMPoint localPosition;

	SDL_Rect rect;

	SDL_Texture* texture;


private:

	bool dragable;
	bool interactable;

	UI_TYPE type;
	
	UI* father;
};

#endif//__UI_H__