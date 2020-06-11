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
	void ChangeTexture2();
	void ChangeTexture3();

	void Draw(float dt);


private:

	SDL_Texture* texture2;
	SDL_Texture* texture3;

	int resourcesNumber;
	int resourcesNumberSKill;
	int resourcesNumberBoost;
};



#endif //__RESOURCES_PORTRAIT_H__