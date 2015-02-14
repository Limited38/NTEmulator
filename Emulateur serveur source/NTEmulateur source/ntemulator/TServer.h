#pragma once
#define MAX_CLIENTS 1000

#ifndef _H_SERVER
#define _H_SERVER

//Typedefintionen
class TServer;
extern TServer GameServer;
//----------------------------------

class TSocket
{
public:
	TSocket(); //Konstruktor
	~TSocket(); //Destruktor

	//Allgemeine Socket Variablen
	int packetlength;
	SOCKET socket;
	SOCKADDR_IN clientaddr;
	FD_SET fd;
	//---------------------------------

	//Ingame bezogene Variablen
	structAccount *Account;
	structCharacterList *CharacterList;
	structCharacterList *Character;
	//----------------------------------

	void StartGameRecv(); //Client Game Recv Thread starten
	bool Send(std::string packet); //Packet zum Client senden
	void Disconnect(); //Client verbindung schlieﬂen
};

class TServer
{
public:
	FD_SET fd;
	SOCKET acceptsocket;
	SOCKADDR_IN sockaddr;
	TSocket Client[MAX_CLIENTS]; //Socket Array

	bool Initialize(std::string ip, u_short port); //Server erstellen
	void End(); //Server beenden
	void AcceptConnections(); //Server Verbindungen annehmen
};

#endif _H_SERVER