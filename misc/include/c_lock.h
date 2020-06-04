#ifndef _C_LOCK_H_
#define _C_LOCK_H_

#include <pthread.h>

class CLock
{
	public:
		CLock(pthread_mutex_t* mutex):m_mutex(mutex){
			pthread_mutex_lock(m_mutex);
		};
		~CLock(){
			pthread_mutex_unlock(m_mutex);
		};
	private:
		pthread_mutex_t* m_mutex;
};


#endif
