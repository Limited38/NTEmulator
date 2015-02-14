#include "stdafx.h"
#include "TServer.h"

//Typedefinitionen
TServer GameServer;

//------------------------------------------------------------

bool TServer::Initialize( std::string ip, u_short port )
{
	WSADATA wsa;
	int result;
	const char *option;
	u_long argp = 1;

	//Winsock initialisieren
	result = WSAStartup(MAKEWORD(2,0), &wsa);

	if(result == SOCKET_ERROR)
	{
		std::cout << "WSAStartup() failed, Error Code: " << WSAGetLastError() << std::endl;
		std::cout << "Please press Enter to exit" << std::endl;
		return 1;
	}

	//Socket erstellen
	this->acceptsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(this->acceptsocket == SOCKET_ERROR)
	{
		std::cout << "socket() failed, Error Code: " << WSAGetLastError() << std::endl;
		std::cout << "Please press Enter to exit" << std::endl;
		return 1;
	}

	//Socket optionen setzen
	setsockopt(this->acceptsocket, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option));

	this->sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
	this->sockaddr.sin_family = AF_INET;
	this->sockaddr.sin_port = htons(port);

	//Socket am Server binden
	result = bind(this->acceptsocket, (SOCKADDR*)&sockaddr, sizeof(sockaddr));

	if(result == SOCKET_ERROR)
	{
		std::cout << "bind() failed, Error Code: " << WSAGetLastError() << std::endl;
		std::cout << "Please press Enter to exit" << std::endl;
		return 1;
	}

	//Socket ins listen modus setzen
	result = listen(acceptsocket, MAX_CLIENTS);

	if(result == SOCKET_ERROR)
	{
		std::cout << "listen() failed, Error Code: " << WSAGetLastError() << std::endl;
		std::cout << "Please press Enter to exit" << std::endl;
		return 1;
	}

	//I/O modus setzen
	ioctlsocket(this->acceptsocket, FIONBIO, &argp);

	u_long iMode = 1;

	for(auto i = 0; i < MAX_CLIENTS; i++)
	{
		this->Client[i].socket = INVALID_SOCKET;
	}

	return 0;
}

//------------------------------------------------------------

void TServer::End()
{
	//Server aufräumen
	closesocket(this->acceptsocket);
	WSACleanup();
}

//------------------------------------------------------------

void TServer::AcceptConnections()
{
	//Inhalt leeren
	FD_ZERO(&this->fd);

	//Socket der Verbindungen annimmt hinzufügen
	FD_SET(this->acceptsocket, &this->fd);

	//Alle Clients ablaufen und die gültigen Sockets hinzufügen
	for(auto i = 0; i < MAX_CLIENTS; i++)
	{
		if(this->Client[i].socket != INVALID_SOCKET)
		{
			FD_SET(this->Client[i].socket, &this->fd);
		}
	}

	select(NULL, &this->fd, NULL, NULL, NULL);

	//Schauen ob acceptsocket im FD enthalten ist
	if(FD_ISSET(this->acceptsocket, &this->fd))
	{
		//Freien Platz für Clienten suchen und annehmen

		for(auto i = 0; i < MAX_CLIENTS; i++)
		{
			if(this->Client[i].socket == INVALID_SOCKET)
			{
				this->Client[i].packetlength = sizeof(SOCKADDR);
				this->Client[i].socket = accept(this->acceptsocket, (SOCKADDR*)&this->Client[i].clientaddr, &this->Client[i].packetlength);
				std::cout << "Client with ip " << inet_ntoa(this->Client[i].clientaddr.sin_addr) << " connected!" << std::endl;
				
				//Client Zeug allozieren
				this->Client[i].Account = new structAccount;
				this->Client[i].Character = new structCharacterList;
				Client[i].Account->session = NULL;
				break;
			}
		}
	}

	//Prüfen ob Client sockets im FD enthalten sind

	for(auto i = 0; i < MAX_CLIENTS; i++)
	{
		//Kein Verbunder Client an der position
		if(this->Client[i].socket == INVALID_SOCKET)
		{
			continue;
		} else
		{
			if(FD_ISSET(this->Client[i].socket, &this->fd))
			{
				this->Client[i].StartGameRecv();
			}
		}
	}
}

//------------------------------------------------------------

TSocket::TSocket()
{
}

//------------------------------------------------------------

TSocket::~TSocket()
{
}


//------------------------------------------------------------

void TSocket::StartGameRecv()
{
	std::vector<char> packetbuffer(1024);
	std::vector<std::string> decryptbuffer;
	std::vector<std::vector<std::string>> packet;

	this->packetlength = recv(this->socket, &packetbuffer.front(), packetbuffer.size(), 0);

	//Wenn error, Client Verbindung schließen
	if(this->packetlength == NULL || this->packetlength >= 1024)
	{
		this->Disconnect();
		return;
	}

	//Wenn Packetlänge gültig, dann Session setzen (wenn nicht gesetzt), und anschließend Packete entschlüsseln
	if(this->packetlength > 0)
	{
		if(this->Account->session == 0)
		{
			this->Account->session = PacketCypher.DecryptSessionPacket(&packetbuffer.front());
		} else
		{
			packet = PacketCypher.DecryptGamePacket(this->Account->session, (unsigned char*)&packetbuffer.front(), this->packetlength);
				
			//Packete an Handler weiterleiten
			PacketHandler.DispatchGame(this, packet); 
		}
	}
}

//------------------------------------------------------------

bool TSocket::Send( std::string packet )
{
	//Entsprechende Verschlüsselung anhand des Servertypes auswählen
	timeval time = {1, 0};
	packet = PacketCypher.EncryptGamePacket(packet);
	
	FD_ZERO(&this->fd);
	FD_SET(this->socket, &this->fd);

	select(NULL, NULL, &this->fd, NULL, &time);

	if(FD_ISSET(this->socket, &this->fd))
	{
		//Packet senden
		this->packetlength = send(this->socket, packet.c_str(), packet.length(), 0);

		//Wenn error, Client Verbindung schließen
		if(this->packetlength == SOCKET_ERROR)
		{
			this->Disconnect();
			return 1;
		}
	}

	return 0;
}

//------------------------------------------------------------

void TSocket::Disconnect()
{
	if(this->socket != INVALID_SOCKET)
	{
		FD_ZERO(&this->fd);
		this->packetlength = 0;

		std::cout << "Client with ip " << inet_ntoa(this->clientaddr.sin_addr) << " disconnected!" << std::endl;

		////Wenn Client ingame, dann freigeben/speichern whatever
		if(this->Account->isIngame == true)
		{
			//	//Freunden offline melden
			Friend.Send_Bye(this);
			//Aus Gruppe entfernen
			Group.Leave_Group(this);
			//	//Spieler von der Map entfernen
			Map.RemovePlayerFromMap(this); 
		}

		//Freigeben
		delete this->Account;
		delete[] this->CharacterList;

		//Client schließen
		closesocket(this->socket);
		this->socket = INVALID_SOCKET;
	}	
}

//------------------------------------------------------------