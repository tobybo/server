#ifndef _C_BUFFER_H_
#define _C_BUFFER_H_

#include "c_mempooler.h"
#include "macro.h"
#include "c_log.h"

class CBuffer
{
	public:
		CBuffer(){
			log(2,"[BUFFER] get one buffer");
		}
		~CBuffer(){
			log(2,"[BUFFER] delete one buffer");
		}
		MEMPOOLER_SET(CBuffer);
};

CMemPooler<CBuffer>* CBuffer::m_memPooler = nullptr;

#endif
