#include "stdafx.h"
#include "TMessage.h"

//Typedefinitionen

TMessage Message;

//------------------------------------------------------------

TMessage::TMessage(void)
{
}

//------------------------------------------------------------


TMessage::~TMessage(void)
{
}

//------------------------------------------------------------

void TMessage::Message_Info(TSocket *Client, std::string message)
{
	//MessageBox Packet zum Clienten senden
	std::stringstream packet;
	packet << "info " << message;
	Client->Send(packet.str());
}

//------------------------------------------------------------

void TMessage::Dialog( TSocket *Client, std::string acceptpacket, std::string cancelpacket, std::string message )
{
	std::stringstream ss;
	ss << "dlg " << acceptpacket << " " << cancelpacket << " " << message;
	Client->Send(ss.str());
}

//------------------------------------------------------------
