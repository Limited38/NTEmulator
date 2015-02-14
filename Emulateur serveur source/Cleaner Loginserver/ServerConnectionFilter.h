#pragma once
#ifndef __SERVERCONNECTIONFILTER_H
#define __SERVERCONNECTIONFILTER_H
#include <boost\asio.hpp>
#include <mutex>

namespace Nostale
{
	namespace Network
	{
		class ServerConnectionFilter
		{
		public:
			void setConnectionFilter(const std::vector<boost::asio::ip::address>& connectionFilter);
			bool connectionFiltered(const boost::asio::ip::address& ip);
			void addConnection(const boost::asio::ip::address& ip);
			void removeConnection(const boost::asio::ip::address& ip);
		private:
			std::mutex _mutex;
			std::vector<boost::asio::ip::address> _connectionFilter;
		};
	}
}

#endif
