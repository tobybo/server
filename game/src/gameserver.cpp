#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "c_config.h"

void usage();

int main(int argc, const char** argv)
{
	int exitNum = 0;
	char configPath[100];
	memset(configPath,0,sizeof(configPath));
	if(argc > 1)
	{
		/*for(int i = 0; i<argc; i++)*/
			/*printf("arg%d: %s, size: %d\n",i,argv[i],strlen(argv[i]));*/
		if(sizeof(argv[1]) > 100)
		{
			usage();
		}
		memcpy(configPath,argv[1],strlen(argv[1]));
	}
	else
	{
		strcpy(configPath,"../etc/gameserver.lua");
	}
	//printf("configpath: %s\n",configPath);
	CConfig* g_config = new CConfig();
	if( g_config == nullptr || g_config->LoadConfig(configPath) != 0 )
		usage();
	printf("load config succ, daemon: %d\n",g_config->m_daemon);
	return exitNum;
}

void usage()
{
	printf("usage  : ./gameserver path_to_config; sizeof path_to_config must less than 100 bytes\n");
	printf("example: ./gameserver ../etc/gameserver.lua\n");
	printf("usage  : default config_path is etc/gameserver.lua, use ./gameserver to start the server\n");
	exit(-1);
}
