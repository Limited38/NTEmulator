#pragma once

#ifndef _H_PACKETHANDLER
#define _H_PACKETHANDLER

//Typedefinitionen
class TPacketHandler;
extern TPacketHandler PacketHandler;

//-----------------------

class TPacketHandler
{
public:
	TPacketHandler(); //Konstruktor
	~TPacketHandler(); //Destruktor

	void DispatchGame(TSocket *Client, std::vector<std::vector<std::string>> packet); //Game Packet Handler
	void DispatchGame_Response(TSocket *Client, std::vector<std::string> packet); //Game Response (#) Packet Handler
	void DispatchGM_Response(TSocket *Client, std::vector<std::string> packet); //GM Response (^) Packet Handler
};

#endif _H_PACKETHANDLER
