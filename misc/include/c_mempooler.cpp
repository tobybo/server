#include <stdlib.h>

#include "c_log.h"
#include "macro.h"

template<typename T>
CMemPooler<T>::CMemPooler(int _block_num)
{
	pthread_mutex_init(&m_mutex,nullptr);
	m_blockNum = _block_num;
	m_allocNum = 0;
	m_exNum    = 0;
	m_freeList = nullptr;
	m_memBegin = nullptr;
	m_memEnd   = nullptr;
	m_nodeSize = sizeof(BlockNode);
	createPool();
}

template<typename T>
CMemPooler<T>::~CMemPooler()
{
	if(m_memBegin)
		::free(m_memBegin);
	pthread_mutex_destroy(&m_mutex);
}

template<typename T>
void CMemPooler<T>::createPool()
{
	int one_size = sizeof(BlockNode) + sizeof(T);
	int alloc_size = m_blockNum*(one_size);
	m_memBegin = malloc(alloc_size);
	m_memEnd = (void*)((char*)m_memBegin + alloc_size);
	m_freeList = (BlockNode*)m_memBegin;
	char* temp_ptr;
	for(int i = 0; i < m_blockNum - 1; ++i)
	{
		temp_ptr = (char*)m_freeList + i * one_size;
		((BlockNode*)temp_ptr)->next = (BlockNode*)(temp_ptr + one_size);
	}
	((BlockNode*)(temp_ptr + one_size))->next = nullptr;
	log(2,"[MEMPOOLER] createPool succ, num: %d, size: %d, objsize: %d",m_blockNum,alloc_size,sizeof(T));
}

template<typename T>
T* CMemPooler<T>::fetchOne()
{
	T* obj;
	pthread_mutex_lock(&m_mutex);
	if(m_freeList)
	{
		obj = (T*)(m_freeList + 1); //cool code
		m_freeList = m_freeList->next;
		++m_allocNum;
		pthread_mutex_unlock(&m_mutex);
	}
	else
	{
		pthread_mutex_unlock(&m_mutex);
		obj = (T*)malloc(sizeof(T));
		++m_exNum;
	}
	log(2,"[MEMPOOLER] fetchOne succ, alloc_num: %d, ex_num: %d",m_allocNum,(int)m_exNum);
	return obj;
}

template<typename T>
void CMemPooler<T>::freeOne(T* _obj)
{
	pthread_mutex_lock(&m_mutex);
	if((void*)_obj >= m_memBegin && (void*)_obj < m_memEnd)
	{
		BlockNode* free_node = (BlockNode*)((char*)_obj - sizeof(T));
		free_node->next = m_freeList;
		m_freeList = free_node;
		--m_allocNum;
		pthread_mutex_unlock(&m_mutex);
	}
	else
	{
		pthread_mutex_unlock(&m_mutex);
		::free((void*)_obj);
		--m_exNum;
	}
	log(2,"[MEMPOOLER] freeOne succ, alloc_num: %d, ex_num: %d",m_allocNum,(int)m_exNum);
}

