#ifndef _C_MYLIST_EX_
#define _C_MYLIST_EX_

#include <iostream>
#include <list>
#include <pthread.h>

#include "c_lock.h"

template<class T>
class CMylistEx:public list<T>
{
public:
	CMylistEx():m_mutex(PTHREAD_MUTEX_INITIALIZER),m_cond(PTHREAD_COND_INITIALIZER),m_wait(0)
	{
	}
	~CMylistEx()
	{
		pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_cond);
	}
public:
	T front_with_lock();
	void push_back_with_lock(const T&);

private:
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
	int m_wait;
}

template<typename T>
T CMylistEx<T>::front_with_lock()
{
	CLock(&m_mutex);
	if(this->empty())
	{
		wait = 1;
		pthread_cond_wait(&m_cond,&m_mutex);
		wait = 0;
	}
	T temp = this->front();
	if(temp) this->pop_front();
	return temp;
}

template<typename T>
void CMylistEx<T>::push_back_with_lock(const T& _val)
{
	pthread_mutex_lock(&m_mutex);
	this->push_back(_val);
	pthread_mutex_unlock(&m_mutex);
	if(wait) pthread_cond_signal(&m_cond);
}


#endif

