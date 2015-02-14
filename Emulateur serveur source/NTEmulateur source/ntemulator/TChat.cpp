#include "stdafx.h"
#include "TChat.h"

TChat Chat;
//------------------------------------------------------------

TChat::TChat(void)
{
}

//------------------------------------------------------------

TChat::~TChat(void)
{
}

//------------------------------------------------------------

void TChat::Whisper_Handle(TSocket *Client, std::vector<std::string> packet)
{
	std::stringstream message;
	std::stringstream name;
	std::stringstream error;
	int MapIndex = Map.GetMapIndex(Client->Character->Map.id);
	int MapPlayersSize = Map.MapData[MapIndex].Players.size();

	//Packet erstellen
	name << packet[1].erase(0, 1);
	message << "spk 1 " << Client->Character->CharacterInfo.ID << " " << WHISPER_CHAT << " " << Client->Character->CharacterInfo.Name;

	//Leerzeichen setzen im Text und dem Packet hinzufügen
	int packetsize = packet.size();
	
	for(int i = 2; i < packetsize; i++)
	{
		message << " " << packet[i];
	}

	//Alle Maps absuchen nach dem Spieler

	for(auto e = 0; e < MapPlayersSize; e++)
	{
		//Wenn der Name des Spielers übereinstimmt, anflüstern
		if(Map.MapData[MapIndex].Players[e]->Character->CharacterInfo.Name.compare(name.str()) == 0)
		{
			//TODO: Blacklist check
			Map.MapData[MapIndex].Players[e]->Send(message.str());
			Client->Send(message.str());
			return;
		}
	}
	
	//Message ausgeben falls der Spieler nicht online ist
	error << name.str() << " is not online.";
	Message.Message_Info(Client, error.str());

	return;
}
//------------------------------------------------------------

void TChat::Group_Handle(TSocket *Client, std::vector<std::string> packet)
{
	std::stringstream message;
	std::stringstream name;

	if(Client->Character->Group.ingroup)
	{
		//Packet erstellen
		name << packet[1].erase(0, 1);
		message << "spk 1 " << Client->Character->CharacterInfo.ID << " " << GROUP_CHAT << " " << Client->Character->CharacterInfo.Name;

		//Leerzeichen setzen im Text und dem Packet hinzufügen
		int packetsize = packet.size();

		for(int i = 1; i < packetsize; i++)
		{
			message << " " << packet[i];
		}

		int allocated_group = Group.Group_GetByID(Client->Character->Group.groupid);
		int groupsize = Group.groups[allocated_group].members.size();

		for(int i = 0; i < groupsize; i++)
		{
			Group.groups[allocated_group].members[i].client->Send(message.str());
		}

	}
	return;
}
//------------------------------------------------------------

void TChat::Say_Handle(TSocket *Client, std::vector<std::string> packet)
{
	std::stringstream message;
	int MapIndex = Map.GetMapIndex(Client->Character->Map.id);
	int MapPlayersSize = Map.MapData[MapIndex].Players.size();
	unsigned int distance;

	//Packet erstellen
	message << "say 1 " << Client->Character->CharacterInfo.ID << " " << NORMAL_CHAT;

	//Leerzeichen setzen im Text und dem Packet hinzufügen
	int packetsize = packet.size();

	for(auto i = 2; i < packetsize; i++)
	{
		message << " " << packet[i];
	}

	//Alle Spieler auf der Map ablaufen
	for(auto e = 0; e < MapPlayersSize; e++)
	{
		//Wenn nicht der gleiche Spieler, Text senden an die Spieler in einer bestimmten Distanz
		if(Map.MapData[MapIndex].Players[e]->Character->CharacterInfo.ID != Client->Character->CharacterInfo.ID)
		{
			distance = Math.Distance(Client->Character->Map.x, Map.MapData[MapIndex].Players[e]->Character->Map.x, Client->Character->Map.y, Map.MapData[MapIndex].Players[e]->Character->Map.y);

			if(distance <= SAY_DISTANCE)
			{
				//TODO: Blacklist check
				Map.MapData[MapIndex].Players[e]->Send(message.str());
			}
		}
	}

	return;
}

//------------------------------------------------------------

void TChat::ChatPromotion( TSocket *Client, int count, ... )
{
	std::stringstream str;
	va_list list;

	va_start(list, count);

	for (int i = 0; i < count; i++)
	{
		str << "bn " << i << " " << va_arg(list, char*);
		Client->Send(str.str());
		str.str("");
	}

	va_end(list);
}

//-----------------------------------------------------------

void TChat::LoadSystemMessages( TSocket *Client )
{
	std::stringstream message;
	message << "Welcome " << Client->Character->CharacterInfo.Name  << ",";

	this->ChatPromotion(Client, 2, "Smoke^weed^every^day", "Visit^McLovinGee^on^twitter");
}

//-----------------------------------------------------------

void TChat::ChatMessage( TSocket *Client, int type, std::string message )
{
	std::stringstream ss;
	ss << "say 1 " << Client->Character->CharacterInfo.ID << " " << type << " " << message;
	Client->Send(ss.str());
}

//-----------------------------------------------------------
