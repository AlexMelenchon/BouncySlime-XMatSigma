#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructor
	j1Timer();

	void Start();
	void StartFrom(uint time);
	uint32 Read() const;
	float ReadSec() const;
	void Stop();
	void ReStart();

	bool	running;


private:
	uint32	started_at;
	uint32	stopped_at;

};

#endif //__j1TIMER_H__