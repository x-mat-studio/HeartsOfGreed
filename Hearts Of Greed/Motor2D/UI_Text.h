#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "UI.h"

class UI_Text : public UI
{
public:

	UI_Text(float x, float y, UI* parent, char* text, bool interactable);
	~UI_Text();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	void CleanUp();

};

#endif//__UI_TEXT_H__