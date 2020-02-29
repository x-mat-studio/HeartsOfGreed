// ----------------------------------------------------
// j1PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "PerfectTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 PerfectTimer::frequency = 0;

// ---------------------------------------------
PerfectTimer::PerfectTimer()
{
	// TODO 2: Fill Constructor, Start(),ReadMs() and ReadTicks() methods
	// they are simple, one line each!
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}

// ---------------------------------------------
void PerfectTimer::Start()
{
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
double PerfectTimer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

// ---------------------------------------------
uint64 PerfectTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}


