#ifndef _TURRET_H_
#define _TURRET_H_

#include "Module.h"
#include "Building.h"
#include "Collision.h"


class Turret : public Building
{

public:

	Turret();
	Turret(SDL_Point position, Turret* copy);
	~Turret();


	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);


private:

};



#endif // _TURRET_H_
