#ifndef __DATA_PAGES_H__
#define __DATA_PAGES_H__

#include "UI.h"

class DataPages : public UI
{
public:

	DataPages(float x, float y, UI* parent, bool interactable);
	~DataPages();


private:

	void HandleInput();

	void ChangeTexture();


private:

	int resourcesNumber;
};



#endif //__DATA_PAGES_H__
