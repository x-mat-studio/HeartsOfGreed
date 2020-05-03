#ifndef __RESOURCES_PORTRAIT_H__
#define __RESOURCES_PORTRAIT_H__

#include "UI.h"

class ResourcesPortrait : public UI
{
public:

	ResourcesPortrait(float x, float y, UI* parent, bool interactable);
	~ResourcesPortrait();


private:

	void HandleInput();

	void ChangeTexture();


private:

	int resourcesNumber;
};



#endif //__RESOURCES_PORTRAIT_H__