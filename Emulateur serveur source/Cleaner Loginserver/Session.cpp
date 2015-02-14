#include "Session.h"
#include <boost\bind.hpp>
#include <boost\algorithm\string.hpp>
#include "Log.h"

namespace Nostale
{
	namespace Network
	{
		using namespace boost::asio;

		const unsigned char buf_delim = 0xD8;
		const signed short max_recv_size = 512;

		Session::Session(tcp::socket socket, SessionList& session_list)
			: _socket(std::move(socket)), _timer(socket.get_io_service()), _session_list(session_list), _bufReceive(max_recv_size)
		{
		}

		void Session::start(void)
		{
			// initialize packet handler
			_packetHandler = std::unique_ptr<PacketHandler>(new PacketHandler(shared_from_this()));

			// add session to client room
			_session_list.join(shared_from_this());

			beginRead();
		}

		void Session::disconnect(void)
		{
			_socket.cancel();
		}

		void Session::send(const std::string& buf)
		{
			// encrypt the buffer and add him to the send list
			_bufSend.push_back(_packetEncoding.encryptLoginPacket(std::move(buf)));

			// begin send when data is available
			if(!_bufSend.empty())
				beginWrite();
		}

		void Session::beginRead(void)
		{
			auto self(shared_from_this());

			//initialize timer of 5 seconds
			_timer.expires_from_now(boost::posix_time::seconds(5));
			_timer.async_wait(boost::bind(&Session::handleTimer, self, placeholders::error));

			// begin asynchronous read
			boost::asio::async_read_until(_socket, _bufReceive, buf_delim,
				[this, self](const tcp_error& ec, std::size_t bytes_transfered)
			{
				// cancel timer
				_timer.cancel();

				if(!ec.value() && bytes_transfered <= max_recv_size && _bufReceive.size() <= max_recv_size)
				{
					{
						BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login_connections");
						BOOST_LOG(globalLog::get()) << "[RECEIVED_BYTES] " << _bufReceive.size() << " from "
							<< getIP() << ":" << getPort();
					}

					// read receive buffer into std::string
					streambuf::const_buffers_type buf = _bufReceive.data();
					std::string packetSequences(buffers_begin(buf), buffers_end(buf));

					// clear receive buffer
					_bufReceive.consume(_bufReceive.size());

					std::size_t findSequence = packetSequences.rfind(buf_delim);
					if(findSequence != std::string::npos)
					{
						++findSequence;

						// copy incomplete packet and erase the part from string buffer
						std::string uncompleted_packet = packetSequences.substr(findSequence);
						packetSequences.erase(findSequence);

						// parse and handle all complete packets inside the string buffer
						std::vector<std::string> splitted_packets;
						boost::algorithm::split(splitted_packets, packetSequences, std::bind2nd(std::equal_to<unsigned char>(), (0xD8)));
						for(auto packet : splitted_packets)
							_packetHandler->process(_packetEncoding.decryptLoginPacket(packet));

						// write the incomplete packet to recv buffer
						std::ostream out(&_bufReceive);
						out.write(uncompleted_packet.c_str(), uncompleted_packet.size());
					}

					// asynchronous read loop until an error occurs
					beginRead();
				} else
				{
					{
						BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login_connections");
						BOOST_LOG(globalLog::get()) << "[LOST_CONNECTION] " << getIP() << ":" << getPort();
					}

					// cleanup
					_session_list.leave(shared_from_this());
					_packetHandler.reset();
				}
			});
		}

		void Session::handleTimer(const tcp_error& ec)
		{
			// asynchronous receive timeout
			if(!ec)
				disconnect();
		}

		void Session::beginWrite(void)
		{
			auto self(shared_from_this());

			// begin asynchronous write operations
			boost::asio::async_write(_socket, boost::asio::buffer(_bufSend.front(), _bufSend.front().size()),
				[this, self](const tcp_error& ec, std::size_t bytes_transfered)
			{
				if(!ec)
				{
					{
						BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login_connections");
						BOOST_LOG(globalLog::get()) << "[SEND_BYTES] " << bytes_transfered << " to " << getIP() << "." << getPort();
					}

					// remove last packet
					_bufSend.pop_front();

					//  asynchronous write loop until no packets are available
					if(!_bufSend.empty())
						beginWrite();
				} else
				{
					{
						BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login_connections");
						BOOST_LOG(globalLog::get()) << "[LOST_CONNECTION] " << getIP() << ":" << getPort();
					}

					// cleanup
					_session_list.leave(shared_from_this());
					_packetHandler.reset();
				}
			});
		}

		std::string Session::getIP(void) const
		{
			return _socket.remote_endpoint().address().to_string();
		}

		std::size_t Session::getPort(void) const
		{
			return _socket.remote_endpoint().port();
		}
	}
}