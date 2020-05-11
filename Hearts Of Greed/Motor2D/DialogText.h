#ifndef __DIALOG_TEXT_H__
#define __DIALOG_TEXT_H__

#include "UI.h"

class DialogText : public UI
{
public:
	DialogText(float x, float y, UI* parent, bool interactable);
	~DialogText();

private:

	void HandleInput();

	void ChangeTexture();


private:

	P2SString text;
};



#endif //__DIALOG_TEXT_H__