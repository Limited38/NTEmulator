#pragma once
#ifndef __SERVER_H
#define __SERVER_H
#include <boost\asio.hpp>
#include <boost\noncopyable.hpp>

#include "SessionList.h"
#include "ServerConnectionFilter.h"

namespace Nostale
{
	namespace Network
	{
		typedef boost::asio::ip::tcp tcp;
		typedef boost::system::error_code tcp_error;

		class Server : public boost::noncopyable, public Nostale::Network::ServerConnectionFilter
		{
		public:
			Server(boost::asio::io_service& io_service, const std::string& host, signed short port);
			void send(const std::string& msg);
			void disconnect(void);
			
		private:
			void beginAccept(void);
			
			tcp::acceptor _acceptor;
			tcp::socket _socket;
			SessionList _session_list;
		};
	}
}

#endif