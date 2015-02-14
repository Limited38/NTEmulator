#pragma once
#ifndef __SESSIONLIST_H
#define __SESSIONLIST_H
#include <unordered_set>

#include "Client.h"

namespace Nostale
{
	namespace Network
	{
		class SessionList
		{
		public:
			virtual void join(const client_ptr& client);
			virtual void leave(const client_ptr& client);
			virtual void send(const std::string& buf);
			virtual void disconnect(void);
		private:
			std::unordered_set<client_ptr> _clients;
		};
	}
}

#endif