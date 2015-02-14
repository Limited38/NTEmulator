#pragma once
#ifndef __OBJECTACCESSOR_H
#define __OBJECTACCESSOR_H
#include <boost\noncopyable.hpp>

#include "Client.h"

namespace Nostale
{
	namespace Network
	{
		class PacketFactory;
		class ObjectAccessor : public boost::noncopyable
		{
		public:
			ObjectAccessor(const client_ptr& client);

			void handleAccountLogin(const PacketFactory& Packet);
		private:
			client_ptr _client;

			void sendErrorMsg(const std::string& msg) const;
			void updateAccount(int guid) const;
		};
	}
}

#endif