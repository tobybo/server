#include "c_thread.h"
#include <iostream>

CThread::CThread()
{
}

CThread::~CThread()
{

}

void CThread::join()
{
	pthread_join(m_threadId,NULL);
}

void* thread_start(void *p)
{
	CThread* lpObj = static_cast<CThread*>(p);
	lpObj->m_active = 1;
	lpObj->run();
	return (void*)0;
}

int CThread::start()
{
	return pthread_create(&m_threadId,NULL,thread_start,(void*)this);
}

int CThread::stop()
{
	m_active = 0;
	join();
	return 0;
}

