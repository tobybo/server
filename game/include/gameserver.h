#ifndef _C_GAMESERVER_H_
#define _C_GAMESERVER_H_

class GlobalInstance
{
	public:
		GlobalInstance();
		~GlobalInstance();
	public:
		bool m_reload;
		bool m_stop;
		bool m_active;
};

extern GlobalInstance* g_instance;

#endif
