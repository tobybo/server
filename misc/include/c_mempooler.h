#ifndef _C_MEMPOOLER_
#define _C_MEMPOOLER_

#include <pthread.h>
#include <atomic>
#include <stdlib.h>

template<class T>
class CMemPooler
{
public:
	CMemPooler(int _block_num); //初始化内存池
	~CMemPooler();              //释放内存池
	void createPool();

protected:
	struct BlockNode
	{
		BlockNode* next;
		BlockNode():next(nullptr){}
	};

public:
	T* fetchOne();              //取一个对象内存
	void freeOne(T* _obj);      //回收一个对象

private:
	BlockNode*           m_freeList;

	pthread_mutex_t      m_mutex;

	int                  m_blockNum;
	int                  m_allocNum;
	std::atomic<int>     m_exNum;
	int                  m_nodeSize;

	void*                m_memBegin;
	void*                m_memEnd;

};

#include "c_mempooler.cpp"

#define MEMPOOLER_SET(class_name) \
	public: \
		static CMemPooler<class_name>* m_memPooler; \
		void* operator new(size_t size){ return m_memPooler->fetchOne(); } \
		void operator delete(void* ptr){ m_memPooler->freeOne((class_name*)ptr); }

#endif
