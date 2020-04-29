#include "UI_Group.h"
#include "UI.h"

UI_Group::UI_Group(GROUP_TAG tag) :

	tag(tag)
{}


UI_Group::~UI_Group()
{
	tag = GROUP_TAG::NONE;

	int elementNumber = uiElementVector.size();

	for (int i = elementNumber - 1; i >= 0; i--)
	{
		delete uiElementVector[i];
		uiElementVector[i] = nullptr;

		uiElementVector.erase(uiElementVector.begin() + i);
	}
}


void UI_Group::PreUpdate(float dt)
{
	int elementNumber = uiElementVector.size();

	for (int i = 0; i < elementNumber; i++)
	{
		uiElementVector[i]->PreUpdate(dt);
	}
}


void UI_Group::Update(float dt)
{
	int elementNumber = uiElementVector.size();

	for (int i = 0; i < elementNumber; i++)
	{
		uiElementVector[i]->Update(dt);
	}
}


void UI_Group::PostUpdate(float dt)
{
	int elementNumber = uiElementVector.size();

	for (int i = 0; i < elementNumber; i++)
	{
		uiElementVector[i]->PostUpdate(dt);
	}	

}


void UI_Group::AddUiElement(UI* element)
{
	uiElementVector.push_back(element);
}


GROUP_TAG UI_Group::GetTag()
{
	return tag;
}


void UI_Group::SetTag(GROUP_TAG tagSet)
{
	tag = tagSet;
}


bool UI_Group::OnAbove()
{
	bool ret = false;

	int uiNumber = uiElementVector.size();

	for (int i = uiNumber - 1; i >= 0 && ret == false; i--)
	{
		if (uiElementVector[i]->OnAbove() == true)
		{
			i--;

			for (i; i >= 0; i--)
			{
				uiElementVector[i]->focused = false;
			}

			ret = true;
		}
	}

	return ret;
}


UI* UI_Group::SearchFocus()
{
	int numElem = uiElementVector.size();
	UI* focusUI;

	for (int i = 0; i < numElem; i++)
	{
		if (uiElementVector[i]->OnAbove())
		{
			return uiElementVector[i];
		}
	}

	return nullptr;
}


void UI_Group::UnFocus()
{
	int uiNumber = uiElementVector.size();

	for (int i = 0; i < uiNumber; i++)
	{
		uiElementVector[i]->focused = false;
	}
}