#include "lunar.h"
#include "c_config.h"

#include <cstring>

int CConfig::LoadConfig(const char* _path)
{
	if(_path == nullptr || strlen(_path) <= 0)
		return -1;
	lua_State* L = lua_open();
	if(Lua::DoFile(L, _path) == 1)
		return -2;

	if(Lua::GetTable(L, LUA_GLOBALSINDEX, "Config") <= 0)
	{
		return -3;
	}

	Lua::GetTableNumber(L, -1, "daemon", m_daemon);
	Lua::GetTableString(L, -1, "logpath", m_logPrefix, 99);
	lua_close(L);
	return 0;
}
