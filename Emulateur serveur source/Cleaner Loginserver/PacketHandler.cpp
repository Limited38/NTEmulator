#include "PacketHandler.h"
#include "PacketFactory.h"
#include "PacketTypes.h"

namespace Nostale
{
	namespace Network
	{
		PacketHandler::PacketHandler(const client_ptr& client)
			: _objectAccessor(std::unique_ptr<ObjectAccessor>(new ObjectAccessor(client)))
		{
			//register packets
			add(PacketTypes::CMSG_ACCOUNT_LOGIN, 7, std::bind(&ObjectAccessor::handleAccountLogin, _objectAccessor.get(), std::placeholders::_1));
		}

		void PacketHandler::process(const std::string& packet) const
		{
			PacketFactory packet_params(packet);
			if(packet_params.getSize())
			{
				// find registered packet and call callback
				for(auto it = _registeredPackets.begin(); it != _registeredPackets.end(); ++it)
				{
					if((it->param_count == packet_params.getSize() || it->param_count == unknown_param_count)
						&& it->packethead == packet_params.get<std::string>(0))
					{
						it->callback(packet_params);
						break;
					}
				}
			}
		}

		void PacketHandler::add(const std::string& packethead, std::size_t param_count, const callback_handler& callback)
		{
			if(!callback._Empty() && !packethead.empty())
			{
				// register packet with callback
				RegisteredPacket regPacket = {std::move(packethead), param_count, std::move(callback)};
				_registeredPackets.push_back(regPacket);
			}
		}
	}
}