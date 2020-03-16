#ifndef _BARRICADE_H_
#define _BARRICADE_H_

#include "Module.h"
#include "Building.h"
#include "Collision.h"


class Barricade : public Building
{

public:

	Barricade(int barricadeLvl);
	~Barricade();


	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);


private:
	int barricadeLvl;
};



#endif // _BARRICADE_H_
