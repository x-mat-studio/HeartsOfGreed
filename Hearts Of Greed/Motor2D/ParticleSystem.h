#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "vector"
#include "p2Point.h"

class Emitter;

class ParticleSystem
{
public:
	ParticleSystem();
	ParticleSystem(float x, float y);

	~ParticleSystem();

	void PushEmiter(Emitter& emitter);

	void Update(float dt);
	void PostUpdate(float dt);

	void Desactivate();
	void Activate();

	bool IsActive();

	void Move(int x, int y);

private:
	std::vector<Emitter> emitterVector;
	fMPoint	position;

	bool active;
};



#endif // __PARTICLE_SYSTEM_H__