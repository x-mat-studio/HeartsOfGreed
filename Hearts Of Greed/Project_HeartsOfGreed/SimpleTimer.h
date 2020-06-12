#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Log.h"

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;

class 
	SimpleTimer
{
public:

	// Constructor
	SimpleTimer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;

private:
	uint32	started_at;
};

#endif //__j1TIMER_H__
