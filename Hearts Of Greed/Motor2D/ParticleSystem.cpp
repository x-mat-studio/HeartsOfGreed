#include "ParticleSystem.h"
#include "Emitter.h"
#include "Particle.h"

ParticleSystem::ParticleSystem() :

	Entity(fMPoint(0, 0), ENTITY_TYPE::PARTICLE_SYSTEM, ENTITY_ALIGNEMENT::NEUTRAL, nullptr, 0, 0),
	
	active(false)
{}

ParticleSystem::ParticleSystem(float x, float y, ParticleSystem* copy, bool active) :

	Entity(fMPoint(x, y), ENTITY_TYPE::PARTICLE_SYSTEM, ENTITY_ALIGNEMENT::NEUTRAL, nullptr, 0, 0),

	emitterVector(copy->emitterVector),

	active(active)
{
	Move(position.x, position.y);
}

ParticleSystem::~ParticleSystem()
{
	emitterVector.clear();
}


bool ParticleSystem::Update(float dt)
{
	int numEmiters = emitterVector.size();

	for (int i = 0; i < numEmiters; i++)
	{
		emitterVector[i].Update(dt);
	}

	return true;
}


bool ParticleSystem::PostUpdate(float dt)
{
	int numEmiters = emitterVector.size();

	for (int i = 0; i < numEmiters; i++)
	{
		emitterVector[i].PostUpdate(dt);
	}

	return true;
}


void ParticleSystem::PushEmiter(Emitter& emiter)
{
	emitterVector.push_back(emiter);
}


void ParticleSystem::Desactivate()
{
	int numEmiters = emitterVector.size();

	for (int i = 0; i < numEmiters; i++)
	{
		emitterVector[i].Desactivate();
	}

	active = false;
}


void ParticleSystem::Activate()
{
	int numEmiters = emitterVector.size();

	for (int i = 0; i < numEmiters; i++)
	{
		emitterVector[i].Activate();
	}

	active = true;
}


bool ParticleSystem::IsActive()
{
	return active;
}

//Move the particle system, and its emiters in relation to the particle system
void ParticleSystem::Move(int x, int y)
{
	position.x = x;
	position.y = y;

	if (active)
	{
		int numEmiters = emitterVector.size();

		for (int i = 0; i < numEmiters; i++)
		{
			emitterVector[i].SetPosition(x, y);
		}
	}

}