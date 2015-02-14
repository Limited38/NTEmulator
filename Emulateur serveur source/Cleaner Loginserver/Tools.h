#pragma once
#ifndef __TOOLS_H
#define __TOOLS_H
#include <string>

namespace Nostale
{
	namespace Tools
	{
		// Universal time clock format
		std::string getCurrentDateTime(void);
		std::string getDateTimeFromUnix(__int64 unixtime);
		__int64 getUnixTime(void);
	}
}

#endif