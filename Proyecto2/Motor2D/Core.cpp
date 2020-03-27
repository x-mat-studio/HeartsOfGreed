#include "Core.h"



Core::Core(int resourceProductionRate) :

	// WHEN A CONSTRUCTOR IS MADE, CTRL + F AND ERASE THE COMMENT BARS ON THE LINES WITH "CORE_CONSTRUCTOR_NEEDED"
	resourceProductionRate(resourceProductionRate)
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
