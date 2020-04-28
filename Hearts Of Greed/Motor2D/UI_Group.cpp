#include "UI_Group.h"
#include "UI.h"

UI_Group::UI_Group(GROUP_TAG tag) :

	tag(tag)
{}

UI_Group::~UI_Group()
{
	tag = GROUP_TAG::NONE;

	int elementNumber = uiElementVector.size();

	for (int i = elementNumber; i >= 0; i--)
	{
		delete uiElementVector[i];
		uiElementVector[i] = nullptr;

		uiElementVector.erase(uiElementVector.begin() + i);
	}
}


void UI_Group::AddUiElement(UI* element)
{
	uiElementVector.push_back(element);
}