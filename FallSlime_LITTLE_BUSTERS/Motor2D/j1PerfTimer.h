#ifndef __j1PERFTIMER_H__
#define __j1PERFTIMER_H__

#include "p2Defs.h"
#include "j1Module.h"

class j1PerfTimer
{
public:

	// Constructor
	j1PerfTimer();

	void Start();
	double ReadMs() const;
	uint64 ReadTicks() const;
	float ReadSec() const;
private:
	uint64	started_at;
	static uint64 frequency;
};

#endif //__j1PERFTIMER_H__