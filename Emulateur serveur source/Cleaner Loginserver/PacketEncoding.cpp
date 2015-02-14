#include "PacketEncoding.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <md5.h>
#include <hex.h>
#pragma comment(lib, "cryptlib.lib")

#include <sstream>

namespace Nostale
{
	namespace Network
	{
		std::string PacketEncoding::createMD5Hash(const std::string& buf) const
		{
			CryptoPP::Weak1::MD5 hash;
			std::string output;

			// create md5 hash
			CryptoPP::StringSource(buf, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(output))));
			return output;
		}

		std::string PacketEncoding::decryptPassword(const std::string& buf) const
		{
			std::string buffer = std::move(buf);
			std::size_t skip_bytes = buffer.size() % 4;

			// erase fake bytes
			buffer.erase(0, skip_bytes ? skip_bytes : 4);
	
			// get check buffer and password buffer
			std::string password_buffer;
			for(std::size_t i = 1; i < buffer.size(); ++i, ++i) 
				password_buffer += std::move(buffer.at(i));

			// convert hex string to string
			std::string output;
			CryptoPP::StringSource(password_buffer, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(output)));
			return output;
		}

		std::string PacketEncoding::decryptLoginPacket(const std::string& buf) const
		{
			std::string output;

			// decrypt login buffer
			std::transform(buf.begin(), buf.end(), std::back_inserter(output), [](unsigned char c) {
				return (c - 0xF) ^ 0xC3;
			});

			return output;
		}

		std::string PacketEncoding::encryptLoginPacket(const std::string& buf) const
		{
			std::string output;

			// encrypt login buffer
			std::transform(buf.begin(), buf.end(), std::back_inserter(output), [](char c) {
				return c + 0xF;
			});

			return output;
		}
	}
}