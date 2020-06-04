#ifndef _C_CONFIG_H_
#define _C_CONFIG_H_

#include <stdlib.h>

class CConfig
{
	public:
		CConfig(){
			memset(m_logPrefix,0,30);
		};
		~CConfig(){};
	public:
		int LoadConfig(const char* _path);
	public:
		int m_daemon;
		char m_logPrefix[30];

};


#endif
