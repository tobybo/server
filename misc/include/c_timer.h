#ifndef _C_TIMER_H_
#define _C_TIMER_H_

#include "c_thread.h"

class CTimer:public CThread
{
	public:
		void run();

	private:
};

#endif
