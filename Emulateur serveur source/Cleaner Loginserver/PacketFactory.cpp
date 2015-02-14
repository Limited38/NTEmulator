#include "PacketFactory.h"
#include <boost\algorithm\string.hpp>

namespace Nostale
{
	namespace Network
	{
		PacketFactory::PacketFactory(const std::string& buf)
		{
			if(!buf.empty())
			{
				// check the packet type and split the buffer with right delimiter
				if(buf.at(0) == '#')
				{
					boost::algorithm::split(_params, buf, std::bind2nd(std::equal_to<char>(), ('^')));
				} else
				{
					boost::algorithm::split(_params, buf, std::bind2nd(std::equal_to<char>(), (' ')));
				}
			}
		}

		PacketFactory::PacketFactory(const std::string& buf, unsigned char delim)
		{
			if(!buf.empty())
				boost::algorithm::split(_params, buf, std::bind2nd(std::equal_to<unsigned char>(), (delim)));
		}

		std::size_t PacketFactory::getSize(void) const
		{
			return _params.size();
		}
	}
}