#ifndef __UI_GROUP_H__
#define __UI_GROUP_H__

#include "vector"

class UI;

enum class GROUP_TAG : int
{
	NONE = -1,

	LOGO_MENU,
	MAIN_MENU,

	IN_GAME,
	PAUSE_MENU,
	OPTIONS_MENU,
	CREDITS_MENU,
	DATA_PAGE,
	IN_HOVER_MENU,

	DIALOG,

	LOSE_SCENE,
	WIN_SCENE,

	MAX
};


class UI_Group
{
public:
	UI_Group(GROUP_TAG tag);
	~UI_Group();

	void PreUpdate(float dt);
	void Update(float dt);
	void PostUpdate(float dt);

	void AddUiElement(UI* element);
	
	GROUP_TAG GetTag();
	void SetTag(GROUP_TAG tagSet);

	bool OnAbove();
	UI* SearchFocus();
	void UnFocus();

private:
	std::vector<UI*> uiElementVector;
	GROUP_TAG tag;

};



#endif //__UI_GROUP_H__