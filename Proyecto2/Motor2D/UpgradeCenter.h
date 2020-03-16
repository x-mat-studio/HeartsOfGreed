#ifndef _UPGRADECENTER_H_
#define _UPGRADECENTER_H_

#include "Module.h"
#include "Building.h"
#include "Collision.h"


class UpgradeCenter : public Building
{

public:

	UpgradeCenter();
	UpgradeCenter(SDL_Point position, UpgradeCenter* copy);
	~UpgradeCenter();


	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);


private:

};



#endif // _UPGRADECENTER_H_
