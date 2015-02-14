#pragma once
#ifndef __DATABASE_H
#define __DATABASE_H
#include <string>

#define SOCI_USE_BOOST
#include <soci.h>

namespace Nostale
{
	namespace Network
	{
		class Database
		{
		public:
			Database(
				const std::string& host,
				const std::string& port,
				const std::string& user,
				const std::string& password,
				const std::string& schema,
				std::size_t poolSize
				);

			soci::connection_pool& getPool(void);

		private:
			soci::connection_pool _connectionPool;
		};

		// global database declaration
		extern std::unique_ptr<Database> globalDatabase;
	}
}

#endif