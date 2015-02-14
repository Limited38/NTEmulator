#pragma once
#ifndef __CLIENT_H
#define __CLIENT_H
#include <memory>
#include <string>

namespace Nostale
{
	namespace Network
	{
		class Client
		{
		public:
			virtual ~Client(void) {};
			virtual void send(const std::string& buf) = 0;
			virtual void disconnect(void) = 0;
			virtual std::string getIP(void) const = 0;
			virtual std::size_t getPort(void) const = 0;
		};

		typedef std::shared_ptr<Client> client_ptr;
	}
}

#endif