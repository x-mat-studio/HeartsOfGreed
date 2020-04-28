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

	LOSE_SCENE,
	WIN_SCENE,

	MAX
};


class UI_Group
{
public:
	UI_Group(GROUP_TAG tag);
	~UI_Group();

	void AddUiElement(UI* element);

private:
	std::vector<UI*> uiElementVector;
	GROUP_TAG tag;

};



#endif //__UI_GROUP_H__