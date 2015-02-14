#include "ObjectAccessor.h"
#include "PacketEncoding.h"
#include "PacketFactory.h"
#include "PacketTypes.h"
#include "Database.h"
#include "DatabaseQuerys.h"
#include "GlobalConfig.h"
#include "Log.h"
#include "Tools.h"

namespace Nostale
{
	namespace Network
	{
		ObjectAccessor::ObjectAccessor(const client_ptr& client)
			: _client(std::move(client))
		{
		}

		void ObjectAccessor::handleAccountLogin(const PacketFactory& Packet)
		{
			PacketEncoding PacketEncoder;

			//  NoS0575 <unknown> <user> <password> <addr\tversion> <unknown> <hash>
			auto packethead = Packet.get<std::string>(0);
			auto unknown1 = Packet.get<int>(1);
			auto user = Packet.get<std::string>(2);
			auto password = PacketEncoder.createMD5Hash(PacketEncoder.decryptPassword(Packet.get<std::string>(3)));
			auto version = Packet.get<std::string>(4);
			auto unknown2 = Packet.get<int>(5);
			auto hash = Packet.get<std::string>(6);

			BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login_requests");
			BOOST_LOG(globalLog::get()) << "[LOGIN_REQUEST] from " << _client->getIP() << ":" << _client->getPort()
				<< " AccountID: " << user;

			// get client version from packet
			std::string::size_type index = version.find_first_of('\v');
			if(index != std::string::npos)
				version.erase(0, index + 1);

			// create client hash string
			std::stringstream stream;
			stream << GlobalConfig::DXClientHash << GlobalConfig::GLClientHash << user;

			if(GlobalConfig::Maintenance)
			{
				// maintenance enabled
				BOOST_LOG(globalLog::get()) << "[LOGIN_ABORTED] server is currently under maintenance.";
				sendErrorMsg("Server is currently under maintenance.");
			} else if(GlobalConfig::ClientVersion != version)
			{
				// client version is invalid
				BOOST_LOG(globalLog::get()) << "[LOGIN_ABORTED] client version is invalid.";
				sendErrorMsg("Your client is outdated, please update your client.");
			} else if(hash != PacketEncoder.createMD5Hash(stream.str()))
			{
				// client hash is invalid
				BOOST_LOG(globalLog::get()) << "[LOGIN_ABORTED] client hash is invalid";
				sendErrorMsg("Your client is outdated, please update your client.");
			} else
			{
				BOOST_LOG(globalLog::get()) << "[DATABASE_REQUEST] fetching account information user: " << user << " and password: " << password;

				// get database connection from the pool
				soci::session sql(globalDatabase->getPool());

				// GUID, IsBlocked, BlockedUntil, BlockedReason
				boost::tuple<boost::optional<int>, int, int, std::string> account;
				sql << DatabaseQuerys::loginRequestQuery, soci::use(user), soci::use(password), soci::into(account);

				// account guid not initialized then wrong login
				if(!account.get<0>().is_initialized())
				{
					BOOST_LOG(globalLog::get()) << "[LOGIN_ABORTED] wrong user or password.";
					sendErrorMsg("Login failed, wrong user or password.");
				} else if(account.get<1>() == 1)
				{
					// account blocked permanently
					BOOST_LOG(globalLog::get()) << "[LOGIN_ABORTED] account blocked permanently.";
					sendErrorMsg("Your account is blocked permanently. Reason: " + account.get<3>());
				} else if(account.get<1>() == 2 && account.get<2>() > Tools::getUnixTime())
				{
					// account blocked temporary
					BOOST_LOG(globalLog::get()) << "[LOGIN_ABORTED] account blocked temporary.";
					sendErrorMsg("Your account is blocked temporary until " + Tools::getDateTimeFromUnix(account.get<2>()) + " UTC. Reason: " + account.get<3>());
				} else
				{
					// login succeeded
					BOOST_LOG(globalLog::get()) << "[LOGIN_ACCEPTED] successfully logged in.";
					sendErrorMsg("Login succeeded");
				}
			}
		}

		void ObjectAccessor::sendErrorMsg(const std::string& msg) const
		{
			std::stringstream stream;
			stream << PacketTypes::SMSG_ERROR_MESSAGE << " " << std::move(msg) << "\n";
			_client->send(stream.str());
		}

		void ObjectAccessor::updateAccount(int guid) const
		{
			//	soci::session sql(globalDatabase->getPool());
			////	sql << "UPDATE accounts WHERE id = :guid SET LastLoginDate = :date AND LastLoginIP :ip", use(), use(_client->getIP())

			//	BOOST_LOG_SCOPED_THREAD_TAG("log", "server_login_requests");
			//	BOOST_LOG(globalLog::get()) << "Updating account set LastLoginDate #" << " " << " LastLoginIP #" << " ";
			//	BOOST_LOG(globalLog::get()) << sql.get_query();
		}
	}
}