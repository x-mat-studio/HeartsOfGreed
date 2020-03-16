#include "Core.h"



Core::Core(int resourceProductionRate) :

	resourceProductionRate(resourceProductionRate)
{}

Core::Core(SDL_Point position, Core * copy)

{}

Core::~Core()
{
}

bool Core::Start()
{
	return true;
}


bool Core::PreUpdate(float dt)
{
	return true;
}


bool Core::Update(float dt)
{
	return true;
}


bool Core::PostUpdate(float dt)
{
	return true;
}

void Core::RessurectHero(Hero * hero)
{
}

void Core::GainResources()
{
}
