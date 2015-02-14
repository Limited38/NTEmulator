#pragma once
#ifndef __PACKETENCODING_H
#define __PACKETENCODING_H
#include <string>

namespace Nostale
{
	namespace Network
	{
		class PacketEncoding
		{
		public:
			std::string decryptPassword(const std::string& buf) const;
			std::string decryptLoginPacket(const std::string& buf) const;
			std::string encryptLoginPacket(const std::string& buf) const;
			std::string createMD5Hash(const std::string& buf) const;
		};
	}
}

#endif