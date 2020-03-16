#ifndef _CORE_H_
#define _CORE_H_

#include "Module.h"
#include "Building.h"
#include "Collision.h"


class Core : public Building 
{

public:

	Core();
	Core(SDL_Point position, Core* copy);
	~Core();


	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);


private: 

};



#endif // _CORE_H_