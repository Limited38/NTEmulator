#include "Server.h"
#include <boost\bind.hpp>

#include "Session.h"
#include "Log.h"

namespace Nostale
{
	namespace Network
	{
		using namespace boost::asio;

		Server::Server(io_service& io_service, const std::string& host, signed short port)
			: _acceptor(io_service, tcp::endpoint(ip::address::from_string(std::move(host)), port)), _socket(io_service)
		{
			beginAccept();
		}

		void Server::send(const std::string& msg)
		{
			// echo message to clients
			_session_list.send(msg);
		}

		void Server::disconnect(void)
		{
			// disconnect all sockets
			_session_list.disconnect();
		}

		void Server::beginAccept(void)
		{
			// run asynchronous accept
			_acceptor.async_accept(_socket, [this](const tcp_error& ec)
			{
				if(!ec)
				{
					if(!connectionFiltered(_socket.remote_endpoint().address()))
					{
						{
							BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login_connections");
							BOOST_LOG(globalLog::get()) << "[NEW_CONNECTION] " << _socket.remote_endpoint().address().to_string()
								<< ":" << _socket.remote_endpoint().port();
						}

						std::make_shared<Session>(std::move(_socket), _session_list)->start();
					} else
					{
						_socket.close();
					}
				} 

				// accepting loop
				beginAccept();
			});
		}
	}
}