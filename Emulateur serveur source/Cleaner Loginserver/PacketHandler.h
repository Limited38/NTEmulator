#pragma once
#ifndef __PACKETHANDLER_H
#define __PACKETHANDLER_H
#include "Client.h"
#include "ObjectAccessor.h"

#include <functional>
#include <string>
#include <vector>
#include <memory>

namespace Nostale
{
	namespace Network
	{
		typedef std::function<void(const class PacketFactory&)> callback_handler;
		class PacketHandler
		{
		public:
			PacketHandler(const client_ptr& client);
			void process(const std::string& packet) const;
		private:
			enum { unknown_param_count = -1 };

			struct RegisteredPacket
			{
				std::string packethead;
				std::size_t param_count;
				callback_handler callback;
			};

			void add(const std::string& packethead, std::size_t param_count, const callback_handler& callback);

			std::vector<RegisteredPacket> _registeredPackets;
			std::unique_ptr<ObjectAccessor> _objectAccessor;
		};
	}
}

#endif
