#pragma once
#ifndef __SESSION_H
#define __SESSION_H
#include <boost\asio.hpp>
#include <boost\timer\timer.hpp>
#include <deque>

#include "SessionList.h"
#include "PacketEncoding.h"
#include "PacketHandler.h"

namespace Nostale
{
	namespace Network
	{
		typedef boost::asio::ip::tcp tcp;
		typedef boost::system::error_code tcp_error;

		class Session
			: public std::enable_shared_from_this<Session>, public Client
		{
		public:
			Session(tcp::socket socket, SessionList& session_list);

			void start(void);
			void send(const std::string& buf);
			void disconnect(void);
			std::string getIP(void) const;
			std::size_t getPort(void) const;
		private:
			SessionList& _session_list;
			tcp::socket _socket;
			PacketEncoding _packetEncoding;
			std::unique_ptr<PacketHandler> _packetHandler;
			boost::asio::streambuf _bufReceive;
			std::deque<std::string> _bufSend;
			boost::asio::deadline_timer _timer;

			void beginRead(void);
			void beginWrite(void);
			void handleTimer(const tcp_error& ec);
		};
	}
}

#endif
