#include "stdafx.h"
#include "TPacketHandler.h"

TPacketHandler PacketHandler;
//------------------------------------------------------------

TPacketHandler::TPacketHandler()
{
}

//------------------------------------------------------------

TPacketHandler::~TPacketHandler()
{
}

//------------------------------------------------------------

void TPacketHandler::DispatchGame(TSocket *Client, std::vector<std::vector<std::string>> packet)
{
	int PacketSize = packet.size();

	//Prüfen ob Packetsize größer 0
	if(PacketSize > 0)
	{
		//Lade Charaktere sobald Account Details empfangen
		if(Client->Account->user.empty() && Client->Account->password.empty())
		{
			Client->Account->user = packet[0][1];
			Client->Account->password = packet[1][1];
			Character.LoadCharacter(Client);

		} else
		{
			for(auto i = 0; i < PacketSize; i++)
			{
				//Normale Packets
				if(packet[i][1].compare("Char_NEW") == 0 && packet[i].size() == 7) //Neuen Charakter erstellen
				{
					Character.NewCharacter(Client, packet[i][2], General.s_atoi(packet[i][3]), General.s_atoi(packet[i][4]), General.s_atoi(packet[i][5]), General.s_atoi(packet[i][6]));

				} else if(packet[i][1].compare("Char_DEL") == 0 && packet[i].size() == 4) //Charakter löschen
				{
					Character.DeleteCharacter(Client, General.s_atoi(packet[i][2]), packet[i][3]);
				} else if(packet[i][1].compare("select") == 0 && packet[i].size() == 3) //Charakter auswählen
				{
					Character.SelectCharacter(Client, General.s_atoi(packet[i][2]));
				} else if(packet[i][1].compare("walk") == 0 && packet[i].size() == 6) //Charakter movement
				{
					CharacterMove.Move_Character(Client, General.s_atoi(packet[i][2]), General.s_atoi(packet[i][3]), General.s_atoi(packet[i][4]), General.s_atoi(packet[i][5]));
				} else if(packet[i][1].compare("say") == 0 && packet[i].size() >= 2) //Character Chatmessage
				{
					Chat.Say_Handle(Client, packet[i]);	
				} else if(packet[i][1][0] == '/' && packet[i].size() >= 2) //Character Whispermessage
				{
					Chat.Whisper_Handle(Client, packet[i]);	
				} else if(packet[i][1][0] == ';' && packet[i].size() >= 2) //Character Groupmessage
				{
					Chat.Group_Handle(Client, packet[i]);
				} else if(packet[i][1].compare("preq") == 0 && packet[i].size() == 2) //Character change Map request
				{
					Map.ChangeMapRequest(Client);
				} else if(packet[i][1].compare("pulse") == 0 && packet[i].size() == 4) //Character Alive 
				{
					Character.Pulse_Check(Client, General.s_atoi(packet[i][2]));
				} else if(packet[i][1].compare("fins") == 0 && packet[i].size() == 4) //Character Alive 
				{
					Friend.Handle(Client, General.s_atoi(packet[i][2]), General.s_atoi(packet[i][3]));
				} else if(packet[i][1][0] == '#' && packet[i].size() >= 1) //DispatchGame Responses
				{
					DispatchGame_Response(Client, packet[i]);
				} else if(packet[i][1].compare(".enemy") == 0 && packet[i].size() == 5) //DispatchGame Responses
				{
					Enemies.Enemy_Spawn(Client, General.s_atoi(packet[i][2]), General.s_atoi(packet[i][3]), General.s_atoi(packet[i][4]));
				} else if(packet[i][1].compare("b_i") == NULL && packet[i].size() == 4) //Item Löschen anfrage
				{
					Inventory.RemoveItemRequest(Client, General.s_atoi(packet[i][2]), General.s_atoi(packet[i][3]));
				} else if(packet[i][1].compare("mvi") == 0 && packet[i].size() == 6) //Item im Inventar verschieben
				{
					Inventory.MoveItem(Client, General.s_atoi(packet[i][2]), General.s_atoi(packet[i][3]), General.s_atoi(packet[i][4]), General.s_atoi(packet[i][5]));
				} else if(packet[i][1].compare("u_i") == 0 && packet[i].size() == 8) //Item benutzen anfrage
				{
					Inventory.UseItem(Client, General.s_atoi(packet[i][4]), General.s_atoi(packet[i][5]));
				} else if(packet[i][1].compare("req_exc") == 0 && packet[i].size() == 4) //Trade anfrage
				{
					Trade.Trade_Handle(Client, General.s_atoi(packet[i][2]), General.s_atoi(packet[i][3]));
				} else if(packet[i][1].compare("req_exc") == 0 && packet[i].size() == 3) //Trade Abbruch // Trade beenden
				{
					Trade.Trade_Handle(Client, General.s_atoi(packet[i][2]), 0);
				} else if(packet[i][1].compare("exc_list") == 0 && packet[i].size() >= 3) //Trade Anmelden
				{
					Trade.Execute_Handle(Client, packet[i]);
				} else if(packet[i][1].compare("btk") == 0 && packet[i].size() >= 4) //Friend Message
				{
					Friend.FriendMessage(Client, General.s_atoi(packet[i][2]), packet[i]);
				} else if(packet[i][1].compare("fdel") == 0 && packet[i].size() == 3) // Freund Entfernen
				{
					Friend.DeleteFriend(Client, General.s_atoi(packet[i][2]));
				} else if(packet[i][1].compare("put") == 0 && packet[i].size() == 5) // Item droppen
				{
					Inventory.DropItem(Client, General.s_atoi(packet[i][2]), General.s_atoi(packet[i][3]), General.s_atoi(packet[i][4]));
				} else if(packet[i][1].compare("get") == 0 && packet[i].size() == 5) // Item aufheben
				{
					Inventory.GetItem(Client, General.s_atoi(packet[i][2]), General.s_atoi(packet[i][3]), General.s_atoi(packet[i][4]));
				} else if(packet[i][1].compare("npc_req") == NULL && packet[i].size() == 4) // NPC Request
				{
					Npcs.OpenNpcDialog(Client, General.s_atoi(packet[i][3]));
				} else if(packet[i][1].compare("pjoin") == 0 && packet[i].size() == 4) // Gruppe einladen
				{
					Group.pjoin_handle(Client, General.s_atoi(packet[i][2]), General.s_atoi(packet[i][3]));
				} else if(packet[i][1].compare("pleave") == 0 && packet[i].size() == 2) // Gruppe verlassen
				{
					Group.Leave_Group(Client);
				} else if(packet[i][1].compare("buy") == 0 && packet[i].size() == 6) // Item im NPC Shop kaufen
				{
					NpcShop.BuyItem(Client, General.s_atoi(packet[i][3]), General.s_atoi(packet[i][4]), General.s_atoi(packet[i][5]));
				} else if(packet[i][1].compare("sell") == 0 && packet[i].size() == 7)
				{
					NpcShop.SellItem(Client, General.s_atoi(packet[i][3]), General.s_atoi(packet[i][4]), General.s_atoi(packet[i][5]), General.s_atoi(packet[i][6]));
				} else if(packet[i][1].compare("ncif") == 0 && packet[i].size() == 4)
				{
					Npcs.GetTargetStatus(Client, General.s_atoi(packet[i][2]), General.s_atoi(packet[i][3]));
				} else if(packet[i][1].compare("u_s") == 0 && packet[i].size() == 5)
				{
					Skills.Player_Cast(Client, General.s_atoi(packet[i][2]), General.s_atoi(packet[i][3]), General.s_atoi(packet[i][4]));
				}
			}
		}
	}
}

//------------------------------------------------------------

//------- Response Game Packets (#packet^p^i^v^a)
void TPacketHandler::DispatchGame_Response(TSocket *Client, std::vector<std::string> packet)
{
	//Schauen ob nochmehr Packets in den Bytes vorhanden
	std::vector<std::string> splitted = General.split(packet[1], '^');
	
	if(splitted[0].compare("#fins") == 0 && splitted.size() == 3)
	{
		Friend.Handle(Client, General.s_atoi(splitted[1]), General.s_atoi(splitted[2]));
	} else if(splitted[0].compare("#b_i") == 0 && splitted.size() == 4)
	{
		Inventory.RemoveItem(Client, General.s_atoi(splitted[1]), General.s_atoi(splitted[2]));
	} else if(splitted[0].compare("#req_exc") == 0 && splitted.size() == 3)
	{
		Trade.Trade_Handle(Client, General.s_atoi(splitted[1]), General.s_atoi(splitted[2]));
	} else if(splitted[0].compare("#pjoin") == 0 && splitted.size() == 3) //Gruppen invite akzeptieren / ablehnen
	{
		Group.pjoin_handle(Client, General.s_atoi(splitted[1]), General.s_atoi(splitted[2]));
	}
}

//------- Response GM Packets (^packet^p^i^v^a)
void TPacketHandler::DispatchGM_Response(TSocket *Client, std::vector<std::string> packet)
{
	//TODO GM CHECK
	//Schauen ob nochmehr Packets in den Bytes vorhanden
	std::vector<std::string> splitted = General.split(packet[1], '^');

	if(splitted[0].compare("^spawn.enemy") == 0 && splitted.size() == 3)
	{
		//Enemies.Enemy_Spawn(Client, General.s_atoi(splitted[1]), General.s_atoi(splitted[2]));
	}/* else if(splitted[0].compare("#b_i") == 0 && splitted.size() == 4)
	{
		Inventory.RemoveItem(Client, General.s_atoi(splitted[1]), General.s_atoi(splitted[2]));
	} else if(splitted[0].compare("#req_exc") == 0 && splitted.size() == 3)
	{
		Trade.Trade_Handle(Client, General.s_atoi(splitted[1]), General.s_atoi(splitted[2]));
	} else if(splitted[0].compare("#pjoin") == 0 && splitted.size() == 3) //Gruppen invite akzeptieren / ablehnen
	{
		Group.pjoin_handle(Client, General.s_atoi(splitted[1]), General.s_atoi(splitted[2]));
	}*/
}

//------------------------------------------------------------