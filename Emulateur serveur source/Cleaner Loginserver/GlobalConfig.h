#ifndef __GLOBALCONFIG_H
#define __GLOBALCONFIG_H
#include <string>
#include <vector>

namespace Nostale
{
	namespace GlobalConfig
	{
		// global config declarations
		extern bool Maintenance;
		extern std::string DXClientHash;
		extern std::string GLClientHash;
		extern std::string ClientVersion;
		extern std::string ServerListUserAuth;
		extern std::string ServerListPasswordAuth;
	}
}

#endif