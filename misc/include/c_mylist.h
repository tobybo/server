#ifndef _C_MYLIST_H_
#define _C_MYLIST_H_

#include <iostream>
#include <list>
#include <pthread.h>

#include "c_lock.h"

template<class T> //T must be a pointer
class CMylist
{
	public:
		CMylist();
		~CMylist();
	public:
		int size();
		T front();
		T pop_front();
		T pop_front_no_wait();
		void push_back(T);
		void push_back_no_signal(T);
		void just_signal();
	private:
		pthread_mutex_t m_mutex;
		pthread_cond_t m_cond;
		std::list<T> m_list;
		int m_wait;

};

template<class T>
CMylist<T>::CMylist()
{
	m_mutex = PTHREAD_MUTEX_INITIALIZER;
	m_cond = PTHREAD_COND_INITIALIZER;
}

template<class T>
CMylist<T>::~CMylist()
{
	pthread_mutex_lock(&m_mutex);
	T temp;
	int len = size();
	printf("list len: %d\n",len);
	for(int i = 0; i < len; i++)
	{
		temp = m_list.front();
		m_list.pop_front();
		delete temp;
	}
	pthread_mutex_unlock(&m_mutex);
	pthread_mutex_destroy(&m_mutex);
}

template<class T>
int CMylist<T>::size()
{
	return m_list.size();
}

template<class T>
T CMylist<T>::front()
{
	CLock lock(&m_mutex);
	T temp = m_list.front();
	return temp;
}

template<class T>
T CMylist<T>::pop_front()
{
	CLock lock(&m_mutex);
	if(m_list.empty())
	{
		m_wait = 1;
		pthread_cond_wait(&m_cond,&m_mutex);
		m_wait = 0;
		if(m_list.empty())
		{
			return NULL;
		}
	}
	T temp = m_list.front();
	m_list.pop_front();
	return temp;
}

template<class T>
T CMylist<T>::pop_front_no_wait()
{
	CLock lock(&m_mutex);
	T temp = nullptr;
	if(m_list.size() == 0)
	{
		return temp;
	}
	temp = m_list.front();
	m_list.pop_front();
	return temp;
}

template<class T>
void CMylist<T>::push_back(T _val)
{
	CLock lock(&m_mutex);
	m_list.push_back(_val);
	if(m_wait) pthread_cond_signal(&m_cond);
}

template<class T>
void CMylist<T>::push_back_no_signal(T _val)
{
	CLock lock(&m_mutex);
	m_list.push_back(_val);
}

template<class T>
void CMylist<T>::just_signal()
{
	pthread_cond_signal(&m_cond);
}

#endif
