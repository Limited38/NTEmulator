#include "SessionList.h"

namespace Nostale
{
	namespace Network
	{
		void SessionList::join(const client_ptr& client)
		{
			_clients.insert(client);
		}

		void SessionList::leave(const client_ptr& client)
		{
			_clients.erase(client);
		}

		void SessionList::send(const std::string& buf)
		{
			// echo message to clients
			for(auto client = _clients.begin(); client != _clients.end(); ++client)
				(*client)->send(buf);
		}

		void SessionList::disconnect(void)
		{
			// disconnect all clients
			for(auto client = _clients.begin(); client != _clients.end(); ++client)
				(*client)->disconnect();
		}
	}
}