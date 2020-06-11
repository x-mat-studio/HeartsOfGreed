// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "SimpleTimer.h"
#include "SDL\include\SDL_timer.h"


// ---------------------------------------------
SimpleTimer::SimpleTimer()
{
	Start();
}


// ---------------------------------------------
void SimpleTimer::Start()
{
	started_at = SDL_GetTicks();
}


// ---------------------------------------------
uint32 SimpleTimer::Read() const
{
	return SDL_GetTicks() - started_at;
}


// ---------------------------------------------
float SimpleTimer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}