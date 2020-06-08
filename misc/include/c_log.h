#ifndef _C_LOG_H_
#define _C_LOG_H_

#include <list>
#include <cstring>
#include <sys/time.h>
#include "c_thread.h"
#include "c_mylist.h"

const int MAX_LOG_CONTENT = 1024;
const int MAX_LOG_LEVEL   = 3;
const int MAX_LOG_REAMIN  = 10;

const char LEVEL_NAME[][10] = {
	"error_","info_","log_"
};

typedef struct
{
	char buff[MAX_LOG_CONTENT];
	int remain; //0 保留 1 用完释放
	int level;  //日志级别 0 1 2
}log_buff,*lplog_buff;

class CLog:public CThread
{
	public:
		CLog();
		~CLog();
	public:
		void run();
	public:
		int Log(int _level, const char* form, ...);
		void BuildFd();
		void InitLog();
	private:
		int m_newCount;    //额外new buff 的次数，根据这个次数来动态修改保留数量
		int m_remainCount; //保留buff数量
		int m_nowRemainCount; //当前保留buff数量
		int m_fd[MAX_LOG_LEVEL]; //日志文件fd
		struct tm *m_fileDate; //当前文件描述符对应的日期
		CMylist<lplog_buff> m_freeList;  //空闲buff队列 回收和使用的时候需要加锁
		CMylist<lplog_buff> m_writeList; //待写队列 需要加锁操作
		char m_content[MAX_LOG_CONTENT];
};


#endif
