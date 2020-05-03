#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "UI.h"

class UI_Text : public UI
{
public:

	UI_Text(float x, float y, UI* parent, char* text, bool interactable);
	~UI_Text();


};

#endif//__UI_TEXT_H__