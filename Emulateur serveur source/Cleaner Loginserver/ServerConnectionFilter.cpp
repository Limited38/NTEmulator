#include "ServerConnectionFilter.h"


namespace Nostale
{
	namespace Network
	{
		void ServerConnectionFilter::setConnectionFilter(const std::vector<boost::asio::ip::address>& connectionFilter)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_connectionFilter = std::move(connectionFilter);
		}

		bool ServerConnectionFilter::connectionFiltered(const boost::asio::ip::address& ip)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			for(auto it = _connectionFilter.begin(); it != _connectionFilter.end(); ++it)
			{
				if(it->to_v4() == ip.to_v4())
					return true;
			}
			return false;
		}

		void ServerConnectionFilter::addConnection(const boost::asio::ip::address& ip)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_connectionFilter.push_back(std::move(ip));
		}

		void ServerConnectionFilter::removeConnection(const boost::asio::ip::address& ip)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			for(auto it = _connectionFilter.begin(); it != _connectionFilter.end(); ++it)
			{
				if(it->to_v4() == ip.to_v4())
				{
					_connectionFilter.erase(it);
					break;
				}
			}
		}
	}
}