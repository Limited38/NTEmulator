#include "Database.h"
#pragma comment(lib, "soci_core_3_2.lib")

namespace Nostale
{
	namespace Network
	{
		// global database declaration
		std::unique_ptr<Database> globalDatabase;

		Database::Database(
			const std::string& host,
			const std::string& port,
			const std::string& user,
			const std::string& password,
			const std::string& schema,
			std::size_t poolSize) : _connectionPool(poolSize)
		{
			try
			{
				std::stringstream stream;
				stream << "mysql://host=" << std::move(host)
					<< " port=" << std::move(port)
					<< " user=" << std::move(user)
					<< " password=" << std::move(password)
					<< " dbname=" << std::move(schema);

				// connect all sessions to database
				for(std::size_t i = 0; i < poolSize; ++i)
				{
					auto& session = _connectionPool.at(i);
					session.open(stream.str());
				}
			} catch(...)
			{
				throw;
			}
		}

		soci::connection_pool& Database::getPool(void)
		{
			return _connectionPool;
		}
	}
}