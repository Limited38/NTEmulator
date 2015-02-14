#pragma once
#ifndef __PACKETFACTORY_H
#define __PACKETFACTORY_H
#include <string>
#include <vector>
#include <boost\lexical_cast.hpp>

namespace Nostale
{
	namespace Network
	{
		class PacketFactory
		{
		public:
			PacketFactory(const std::string& buf);
			PacketFactory(const std::string& buf, unsigned char delim);
			template<typename T> T get(int index) const
			{
				if(index > signed(_params.size()))
					return 0;
				return boost::lexical_cast<T>(_params.at(index));
			}

			std::size_t getSize(void) const;
		private:
			std::vector<std::string> _params;
		};
	}
}

#endif
