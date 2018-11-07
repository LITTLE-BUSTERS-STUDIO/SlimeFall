// ----------------------------------------------------
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "j1PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 j1PerfTimer::frequency = 0;

// ---------------------------------------------
j1PerfTimer::j1PerfTimer()
{
	
	frequency = SDL_GetPerformanceFrequency(); // to get the count per second of the high resolution counter.
	Start();
}

// ---------------------------------------------
void j1PerfTimer::Start()
{
	started_at = SDL_GetPerformanceCounter();  // to get the current value of the high resolution counter.
}

// ---------------------------------------------
uint64 j1PerfTimer::ReadTicks() const
{
	return (SDL_GetPerformanceCounter() - started_at);
}

// ---------------------------------------------
double j1PerfTimer::ReadMs() const
{
	return (double)(SDL_GetPerformanceCounter() - started_at) / (double)(frequency) * 1000.0;
}


