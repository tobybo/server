#ifndef _C_GAMESERVER_H_
#define _C_GAMESERVER_H_

class GlobalInstance
{
	public:
		GlobalInstance();
		~GlobalInstance();
	public:
		int Init();
	public:
		bool m_reload;
		bool m_stop;
		bool m_active;
};


#endif
