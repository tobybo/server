#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "c_config.h"
#include "gameserver.h"

void usage();
int set_daemon(int _daemon);
void signal_action(int _sig);
void set_signal_action();
void break_dead_while();
void main_loop();
void write_pid();

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
	//设置信号处理
	set_signal_action();
	//设置守护模式
	printf("load config succ, daemon: %d\n",g_config->m_daemon);
	int ret = set_daemon(g_config->m_daemon);
	if(ret != 0)
	{
		goto lblexit;
	}
	write_pid();
	g_instance = new GlobalInstance; //初始化静态变量
	main_loop();
lblexit:
	return exitNum;
}

void usage()
{
	printf("exit   : file %s, line %d\n",__FILE__,__LINE__);
	printf("usage  : ./gameserver path_to_config; sizeof path_to_config must less than 100 bytes\n");
	printf("example: ./gameserver ../etc/gameserver.lua\n");
	printf("usage  : default config_path is etc/gameserver.lua, use ./gameserver to start the server\n");
	exit(-1);
}

int set_daemon(int _daemon)
{
	if(_daemon != 1) return 0;
	pid_t child_pid = fork();
	if(child_pid == -1)
	{
		printf("[START] set_daemon, fork err\n");
		return -1;
	}
	else if(child_pid > 0)
	{
		//父进程退出
		return 1;
	}
	if(setsid() < 0)
	{
		printf("[START] set_daemon, setsid err\n");
		return -1;
	}
	/*linux系统编程上这么写 实践发现是不行的
	open("/dev/null",O_RDWR);
	dup(0);
	dup(0);*/

	int fd = open("/dev/null",O_RDWR);
	if(dup2(fd,STDIN_FILENO) == -1){
		printf("[START] server start faild, set STDIN fd err\n");
		return -1;
	}
	if(dup2(fd,STDOUT_FILENO) == -1){
		printf("[START] server start faild, set STDOUT fd err\n");
		return -1;
	}
	if(fd > STDERR_FILENO){
		if(close(fd) == -1){
			printf("[START] server start faild, close fd err\n");
			return -1;
		}
	}
	return 0;
}

void set_signal_action()
{
	struct sigaction act,old_act;
	memset(&act,0,sizeof(act));
	memset(&old_act,0,sizeof(old_act));
	sigemptyset(&act.sa_mask);
	sigemptyset(&old_act.sa_mask);

	act.sa_handler = &signal_action;
	if(0 != sigaction(SIGINT,&act,&old_act)
		||0 != sigaction(SIGUSR1,&act,&old_act)
		  ||0 != sigaction(SIGUSR2,&act,&old_act))
	{
		usage();
		return;
	}
	printf("[SIGNAL] set_signal_action succ\n");
}

void signal_action(int _sig)
{
	printf("[SIGNAL] set_signal_action begin, sig: %d\n",_sig);
	if(!g_instance)
	{
		return;
	}
	if(!g_instance->m_active)
	{
		return;
	}
	switch(_sig)
	{
		case SIGINT:
			g_instance->m_reload = true;
			break;
		case SIGUSR1:
			g_instance->m_stop = true;
			g_instance->m_active = false; //应该等所有资源释放差不多了再设置的，这里为了前期调试先放在这
			break;
		case SIGUSR2:
			//处理脚本死循环
			break_dead_while();
			break;
		default:
			break;
	}
	printf("[SIGNAL] set_signal_action end, sig: %d\n",_sig);
}

void write_pid()
{
	pid_t pid = getpid();
	int fd = open("gmpid.log",O_RDWR|O_CREAT|O_TRUNC|O_APPEND);
	char pid_str[10];
	memset(pid_str,0,sizeof(pid_str));
	sprintf(pid_str,"%d",pid);
	write(fd,pid_str,strlen(pid_str));
	close(fd);
}

void break_dead_while()
{
	printf("[SIGNAL] break_dead_while\n");
}

void main_loop()
{
	for(;;)
	{
		if(g_instance->m_active == false)
			break;
		sleep(1);
		printf("[MAIN] main_loop\n");
	}
}

GlobalInstance::GlobalInstance()
{
	m_active = true;  //服务器是否开启中
	m_reload = false; //是否需要reload
	m_stop   = false; //是否需要停止服务器
}

GlobalInstance::~GlobalInstance()
{

}

GlobalInstance* g_instance = nullptr;
