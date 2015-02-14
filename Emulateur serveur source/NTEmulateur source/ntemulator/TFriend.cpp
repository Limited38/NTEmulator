#include "stdafx.h"
#include "TFriend.h"

//Typedefinitionen
TFriend Friend;
//------------------------------------------------------------

TFriend::TFriend(void)
{
}

//------------------------------------------------------------

TFriend::~TFriend(void)
{
}
//------------------------------------------------------------
void TFriend::Send_Hello(TSocket *Client)
{
	int friendssize, mapsize, playerssize, size = Client->Character->Friends.size();
	std::stringstream message;

	this->Send_Friendlist(Client);

	if(size > 0)
	{
		for(int i = 0; i < size; i++)
		{
			//Alle Maps absuchen nach dem Spieler
			mapsize = Map.MapData.size();

			for(auto t = 0; t < mapsize; t++)
			{
				playerssize = Map.MapData[t].Players.size();

				//Alle Spieler auf der Map durchlaufen
				for(auto e = 0; e < playerssize; e++)
				{
					//Wenn der Name des Spielers übereinstimmt, anflüstern
					if(Map.MapData[t].Players[e]->Character->CharacterInfo.ID == Client->Character->Friends[i].id)
					{
							Client->Character->Friends[i].is_online = true;

							//Anderen Clienten die Freundesliste absuchen
							friendssize = Map.MapData[t].Players[e]->Character->Friends.size();
							
							for(auto f = 0; f < friendssize; f++)
							{
								if(Map.MapData[t].Players[e]->Character->Friends[f].id == Client->Character->CharacterInfo.ID)
								{
									Map.MapData[t].Players[e]->Character->Friends[f].is_online = true;
									f = friendssize;
								}
							}

							message.str("");
							message << "say 1 " << Map.MapData[t].Players[e]->Character->CharacterInfo.ID << " " << SYSTEM_CHAT << " " << Client->Character->CharacterInfo.Name << " is online.";
							Map.MapData[t].Players[e]->Send(message.str());
							message.str("");
							message << "finfo " << Client->Character->CharacterInfo.ID << ".1";
							Map.MapData[t].Players[e]->Send(message.str());
							message.str("");
							message << "finfo " << Map.MapData[t].Players[e]->Character->CharacterInfo.ID << ".1";
							Client->Send(message.str());
					}
				}
			}
		}
	}
}
//------------------------------------------------------------
void TFriend::Send_Bye(TSocket *Client)
{
	int friendssize, mapsize, playerssize, size = Client->Character->Friends.size();
	std::stringstream message;

	this->Send_Friendlist(Client);

	if(size > 0)
	{
		for(int i = 0; i < size; i++)
		{
			//Alle Maps absuchen nach dem Spieler
			mapsize = Map.MapData.size();

			for(auto t = 0; t < mapsize; t++)
			{
				playerssize = Map.MapData[t].Players.size();

				//Alle Spieler auf der Map durchlaufen
				for(auto e = 0; e < playerssize; e++)
				{
					//Wenn der Name des Spielers übereinstimmt, anflüstern
					if(Map.MapData[t].Players[e]->Character->CharacterInfo.ID == Client->Character->Friends[i].id)
					{
						Client->Character->Friends[i].is_online = true;

						//Anderen Clienten die Freundesliste absuchen
						friendssize = Map.MapData[t].Players[e]->Character->Friends.size();

						for(auto f = 0; f < friendssize; f++)
						{
							if(Map.MapData[t].Players[e]->Character->Friends[f].id == Client->Character->CharacterInfo.ID)
							{
								Map.MapData[t].Players[e]->Character->Friends[f].is_online = true;
								f = friendssize;
							}
						}

						message.str("");
						message << "say 1 " << Map.MapData[t].Players[e]->Character->CharacterInfo.ID << " " << SYSTEM_CHAT << " " << Client->Character->CharacterInfo.Name << " is offline.";
						Map.MapData[t].Players[e]->Send(message.str());
						message.str("");
						message << "finfo " << Client->Character->CharacterInfo.ID << ".0";
						Map.MapData[t].Players[e]->Send(message.str());
					}
				}
			}
		}
	}
}

//------------------------------------------------------------
void TFriend::Send_Friendlist(TSocket *Client)
{
	int size = Client->Character->Friends.size();
	std::stringstream message;
	message << "finit";

	for(int i = 0; i < size; i++)
	{
		message << " " << Client->Character->Friends[i].id << "|" << Client->Character->Friends[i].partner << "|" << Client->Character->Friends[i].is_online << "|" << Client->Character->Friends[i].name; //TODO: Name
	}

	Client->Send(message.str());	
}
//------------------------------------------------------------

void TFriend::Handle(TSocket *Client, int action, int id)
{

	switch(action)
	{
		//Send Friendrequest
	case 1:
		this->Send_Request(Client, id);
		break;
		//Accept Request
	case -1:
		this->Accept_Request(Client, id);
		break;
		//Request verweigern
	case -99:
		this->Decline_Request(Client, id);
		break;
	}
}
//------------------------------------------------------------
void TFriend::Decline_Request(TSocket *Client, int id)
{
	structMapData currentMap;
	int mapsize = Map.MapData.size();

	for(auto i = 0; i < mapsize; i++)
	{
		//Wenn Map gefunden zuweisen
		if(Client->Character->Map.id == Map.MapData[i].id)
		{
			currentMap = Map.MapData[i];
			break;
		}
	}

	int requestsize;

	//Check ob er ihn sich selber schickt
	if(id != Client->Character->CharacterInfo.ID)
	{
		//Alle Spieler auf der Map ablaufen
		int size = currentMap.Players.size();

		for(auto e = 0; e < size; e++)
		{
			//Schauen ob es die angeforderte ID ist
			if(currentMap.Players[e]->Character->CharacterInfo.ID == id)
			{
				//Friendrequest am laufen?
				requestsize = currentMap.Players[e]->Character->FriendRequests.size();
				for(int i = 0; i < requestsize; i++)
				{
					if(currentMap.Players[e]->Character->FriendRequests[i] == Client->Character->CharacterInfo.ID)
					{
						//Lösche Request
						currentMap.Players[e]->Character->FriendRequests.erase(currentMap.Players[e]->Character->FriendRequests.begin() + i);

						//Schicke Nachrichten --------------
						std::stringstream message;
						message << "say 1 " << Client->Character->CharacterInfo.ID << " 10 " << Client->Character->CharacterInfo.Name << " has declined your request.";
						currentMap.Players[e]->Send(message.str());
						//-----------------------------------
						return;
					}
				}
			}
		}
	}
}
//------------------------------------------------------------
void TFriend::Accept_Request(TSocket *Client, int id)
{
	structMapData currentMap;
	int mapsize = Map.MapData.size();

	for(auto i = 0; i < mapsize; i++)
	{
		//Wenn Map gefunden zuweisen
		if(Client->Character->Map.id == Map.MapData[i].id)
		{
			currentMap = Map.MapData[i];
			break;
		}
	}

	structFriends newfriend;
	int requestsize;

	//Check ob er ihn sich selber schickt
	if(id != Client->Character->CharacterInfo.ID)
	{
		//Zur Sicherheit nochmal schauen ob bereits Freund
		if(!this->Friend_Check(Client, id))
		{
			//Alle Spieler auf der Map ablaufen
			int size = currentMap.Players.size();

			for(auto e = 0; e < size; e++)
			{
				//Schauen ob es die angeforderte ID ist
				if(currentMap.Players[e]->Character->CharacterInfo.ID == id)
				{
					//Friendrequest am laufen?
					requestsize = currentMap.Players[e]->Character->FriendRequests.size();
					for(int i = 0; i < requestsize; i++)
					{
						if(currentMap.Players[e]->Character->FriendRequests[i] == Client->Character->CharacterInfo.ID)
						{
							//Lösche Request
							currentMap.Players[e]->Character->FriendRequests.erase(currentMap.Players[e]->Character->FriendRequests.begin() + i);
							//Adde die Freunde
							newfriend.is_online = true;
							newfriend.id = Client->Character->CharacterInfo.ID;
							newfriend.name = Client->Character->CharacterInfo.Name;
							newfriend.partner = 0;

							currentMap.Players[e]->Character->Friends.push_back(newfriend);

							newfriend.is_online = true;
							newfriend.id = currentMap.Players[e]->Character->CharacterInfo.ID;
							newfriend.name = currentMap.Players[e]->Character->CharacterInfo.Name;
							newfriend.partner = 0;

							Client->Character->Friends.push_back(newfriend);

							//Schicke Nachrichten --------------
							std::stringstream message;
							message << "say 1 " << Client->Character->CharacterInfo.ID << " 10 You are now best bro with " << Client->Character->CharacterInfo.Name << "!";
							currentMap.Players[e]->Send(message.str());
							message.str("");
							message << "say 1 " << currentMap.Players[e]->Character->CharacterInfo.ID << " 10 You are now best bro with " << currentMap.Players[e]->Character->CharacterInfo.Name << "!";
							Client->Send(message.str());
							//Send online
							this->Send_Friendlist(Client);
							this->Send_Friendlist(currentMap.Players[e]);
							//-----------------------------------
							return;
						}
					}
				}
			}
		}
	}
}
//------------------------------------------------------------

bool TFriend::Friend_Check(TSocket *Client, int id)
{
	int size = Client->Character->Friends.size();

	for(int i = 0; i < size; i++)
	{
		if(Client->Character->Friends[i].id == id)
		{
			return true;
		}
	}

	return false;
}

//------------------------------------------------------------

bool TFriend::Request_Check(TSocket *Client, int id)
{
	int size = Client->Character->FriendRequests.size();

	for(int i = 0; i < size; i++)
	{
		if(Client->Character->FriendRequests[i] == id)
		{
			return true;
		}
	}

	return false;
}

//------------------------------------------------------------
void TFriend::Send_Request(TSocket *Client, int id)
{
	structMapData currentMap;
	int mapsize = Map.MapData.size();

	for(auto i = 0; i < mapsize; i++)
	{
		//Wenn Map gefunden zuweisen
		if(Client->Character->Map.id == Map.MapData[i].id)
		{
			currentMap = Map.MapData[i];
			break;
		}
	}

	std::stringstream packet;

	//TODO: Blacklist check & bereits Freund
	bool friend_allready = this->Friend_Check(Client, id);
	bool blacklist = false;
	bool request_exist = this->Request_Check(Client, id);

	//Alle Spieler auf der Map ablaufen
	int size = currentMap.Players.size();
	//Check ob er ihn sich selber schickt
	if(id != Client->Character->CharacterInfo.ID)
	{
		if(!request_exist)
		{
			if(!friend_allready)
			{
				if(!blacklist)
				{
					//Alle Spieler der Map abgehen
					bool found = false;
					for(auto e = 0; e < size; e++)
					{
						//Schauen ob es die angeforderte ID ist
						if(currentMap.Players[e]->Character->CharacterInfo.ID == id)
						{
							//Request schicken
							packet << "dlg #fins^-1^" << Client->Character->CharacterInfo.ID << " #fins^-99^" << Client->Character->CharacterInfo.ID << " Werde ein Freund von [" << Client->Character->CharacterInfo.Name << "]?";
							currentMap.Players[e]->Send(packet.str());
							Client->Character->FriendRequests.push_back(id);
							found = true;
							break;
						}
					}

					if(!found)
					{
						Message.Message_Info(Client, "Player not found");
					}

				}else
				{
					Message.Message_Info(Client, "Player blocked you");
				}

			}else
			{
				Message.Message_Info(Client, "Player is already in your friendlist");
			}
		}

	}else
	{
		Message.Message_Info(Client, "You can't be your own friend");
	}

	return;
}

//------------------------------------------------------------

void TFriend::ParseFriends( TSocket *Client, int characterslot, std::string friendsarray )
{
	//Alle Freunde parsen
	std::vector<std::string> friendssplit = General.split(friendsarray, ' ');
	std::vector<std::string> friendsinfo;
	int friendsinfosize, friendssize = friendssplit.size();
	structFriends friends;

	//Alle Freunde durchlaufen
	for(auto i = 0; i < friendssize; i++)
	{
		//Freund Informationen parsen
		friendsinfo = General.split(friendssplit[i], '.');

		//Freund Informationen speichern
		friends.id = General.s_atoi(friendsinfo[0]);
		friends.name = friendsinfo[1];
		friends.partner = General.s_atoi(friendsinfo[2]);
		friends.is_online = false;
		Client->CharacterList[characterslot].Friends.push_back(friends);	
	}
}

//------------------------------------------------------------

void TFriend::FriendMessage( TSocket *Client, int friendid, std::vector<std::string> message )
{
	std::stringstream talk;
	std::string true_message;
	int messagesize = message.size();

	//Message in std::string schreiben
	for(auto i = 3; i < messagesize; i++)
	{
		true_message += ' ';
		true_message += message[i];
	}

	//Packet erstellen
	talk << "talk " << Client->Character->CharacterInfo.ID << true_message;

	int friendssize = Client->Character->Friends.size();
	int mapsize = Map.MapData.size();
	int playersize;

	//Alle Freunde ablaufen
	for(auto i = 0; i < friendssize; i++)
	{
		//Wenn Freund Online 
		if(Client->Character->Friends[i].is_online == true && Client->Character->Friends[i].id == friendid)
		{
			//Alle Maps ablaufen
			for(auto y = 0; y < mapsize; y++)
			{
				playersize = Map.MapData[y].Players.size();

				//Alle Spieler ablaufen
				for(auto e = 0; e < playersize; e++)
				{
					//Wenn der Spieler der Freund, Packet senden
					if(Map.MapData[y].Players[e]->Character->CharacterInfo.ID == friendid)
					{
						Map.MapData[y].Players[e]->Send(talk.str());
						return;
					}
				}
			}
		}
	}
}


//------------------------------------------------------------
void TFriend::DeleteFriend( TSocket *Client, int friendid )
{
	int targetfriendsize, friendssize = Client->Character->Friends.size();
	int mapsize = Map.MapData.size();
	int playersize;

	//Alle Freunde ablaufen
	for(auto i = 0; i < friendssize; i++)
	{
		//Wenns der richtige Freund ist
		if(Client->Character->Friends[i].id == friendid)
		{
			//Wenn der Freund Online ist
			if(Client->Character->Friends[i].is_online == true)
			{
				//Alle Maps ablaufen
				for(auto y = 0; y < mapsize; y++)
				{
					int playersize = Map.MapData[y].Players.size();

					//Alle Spieler ablaufen
					for(auto e = 0; e < playersize; e++)
					{
						//Wenns der richtige ist
						if(Map.MapData[y].Players[e]->Character->CharacterInfo.ID == friendid)
						{
							//Gegenüber von Freundesliste löschen & selbst
							targetfriendsize = Map.MapData[y].Players[e]->Character->Friends.size();

							for(auto f = 0; f < targetfriendsize; f++)
							{
								if(Map.MapData[y].Players[e]->Character->Friends[f].id == Client->Character->CharacterInfo.ID)
								{
									Map.MapData[y].Players[e]->Character->Friends.erase(Map.MapData[y].Players[e]->Character->Friends.begin() + f);
									this->Send_Friendlist(Map.MapData[y].Players[e]);
									Client->Character->Friends.erase(Client->Character->Friends.begin() + i);
									this->Send_Friendlist(Client);
									return;
								}
							}
						}
					}
				}
			}

			//Sich selbst löschen
			Client->Character->Friends.erase(Client->Character->Friends.begin() + i);
			this->Send_Friendlist(Client);
			return;
		}
	}
}



//------------------------------------------------------------