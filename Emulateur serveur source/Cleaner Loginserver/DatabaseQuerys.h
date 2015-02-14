#ifndef __DATABASEQUERYS_H
#define __DATABASEQUERYS_H
#include <string>

namespace Nostale
{
	namespace Network
	{
		namespace DatabaseQuerys
		{
			const std::string loginRequestQuery = "SELECT ID, IsBlocked, BlockedUntil, BlockReason FROM accounts WHERE User = :user and Password = :pass";
		}
	}
}

#endif