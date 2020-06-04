#ifndef _MACRO_H_
#define _MACRO_H_

#include "c_log.h"
#include "c_config.h"
#include "gameserver.h"

extern int gTime;
extern int gFrame;
extern unsigned long gMsec;
extern char gTimeFormat[];

extern CLog* g_log;
extern GlobalInstance* g_instance;
extern CConfig* g_config;

#endif
