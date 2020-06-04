#include "c_log.h"
#include "macro.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <cstring>
#include <sys/time.h>

CLog::CLog()
{
	m_remainCount    = MAX_LOG_REAMIN;
	lplog_buff temp;
	for(int i = 0; i < m_remainCount; i++)
	{
		temp = new log_buff;
		temp->remain = 0;
		m_freeList.push_back_no_signal(temp);
	}
	m_nowRemainCount = MAX_LOG_REAMIN;
	memset(m_fd,0,sizeof(m_fd));
	m_active = 0;
}

void CLog::run()
{
	while(m_active == 0)
	{
		sleep(1);
	}
	while(g_instance->m_active)
	{
		lplog_buff log_str = m_writeList.pop_front();
		if(log_str)
		{
			int fd = m_fd[log_str->level];
			int len = sprintf(m_content,"%s%s",gTimeFormat,log_str->buff);
			int last_idx = len > MAX_LOG_CONTENT? MAX_LOG_CONTENT:len;
			m_content[len] = '\0';
			write(fd,m_content,strlen(m_content));
			if(log_str->remain == 0)
			{
				log_str->level = 0; //not necessary
				m_freeList.push_back_no_signal(log_str);
			}
		}
	}
}

int CLog::Log(int _level, const char* _format, ...)
{
	if(_level >= MAX_LOG_LEVEL) return -1;
	lplog_buff logObj = m_freeList.pop_front_no_wait();
	if(logObj == nullptr)
	{
		logObj = new log_buff;
		logObj->remain = 1;
	}
	logObj->level = _level;

	va_list aptr;
	int ret;
	va_start(aptr, _format);
	ret = vsprintf(logObj->buff, _format, aptr);
	va_end(aptr);

	m_writeList.push_back(logObj);

	return ret;
}

void CLog::BuildFd()
{
	struct tm tmDate;
	localtime_r((time_t*)&gTime,&tmDate);
	char path[100];
	for(int i = 0;i < MAX_LOG_LEVEL; i++)
	{
		int len = sprintf(path,"%s%04d%02d%02d%02d.log",g_config->m_logPrefix,
			LEVEL_NAME[i],tmDate.tm_year,tmDate.tm_mon,tmDate.tm_mday,tmDate.tm_hour);
		path[len] = '\0';
		if(m_fd[i] != 0)
		{
			close(m_fd[i]);
		}
		m_fd[i] = open(path,O_RDWR|O_CREAT|O_APPEND);
	}
}

void CLog::InitLog()
{
	if(m_active) return;
	BuildFd();
	m_active = 1;
}

