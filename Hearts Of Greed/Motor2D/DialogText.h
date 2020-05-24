#ifndef __DIALOG_TEXT_H__
#define __DIALOG_TEXT_H__

#include "UI.h"

class DialogText : public UI
{
public:
	DialogText(float x, float y, fMPoint& posText1, fMPoint& posText2, UI* parent, bool interactable);
	~DialogText();

private:

	void HandleInput();
	void Draw(float dt);
	
	SDL_Texture* ChangeTexture(SDL_Texture* tex, P2SString* string);


private:
	SDL_Texture* texture2;

	fMPoint posText1;
	fMPoint posText2;

	P2SString text1;
	P2SString text2;
};



#endif //__DIALOG_TEXT_H__