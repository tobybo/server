#ifndef _C_THREAD_H_
#define _C_THREAD_H_

#include <iostream>
#include <pthread.h>

class CThread
{
	public:
		CThread();
		virtual ~CThread();
		void join();
		int start();
		virtual int stop();

	protected:
		pthread_t m_threadId;
		int m_active; /* run()函数内部用于结束线程的标志*/

		friend void* thread_start(void *p);
		virtual void run() = 0;
};



#endif
