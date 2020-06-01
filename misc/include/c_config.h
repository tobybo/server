#ifndef _C_CONFIG_H_
#define _C_CONFIG_H_

#include <stdlib.h>

class CConfig
{
	public:
		CConfig(){};
		~CConfig(){};
	public:
		int LoadConfig(const char* _path);
	public:
		int m_daemon;

};

#endif
