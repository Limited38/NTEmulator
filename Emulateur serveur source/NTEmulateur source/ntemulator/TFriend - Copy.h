#pragma once

#ifndef _H_FRIEND
#define _H_FRIEND

//Typedefintionen
class TFriend;
extern TFriend Friend;
//----------------------------

//Strukturen

struct structFriends
{
	std::string name;
	int id;
	bool is_online;
	int partner;
};

//-----------------------------

class TFriend
{
public:
	TFriend(void); //Konstruktor
	~TFriend(void); //Destruktor

	void Send_Friendlist(TSocket *Client); //Freundesliste schicken
	void Handle(TSocket *Client, int action, int id); //Allgemeines verwalten
	void Send_Request(TSocket *Client, int id); //Request an einen Char schicken
	void Accept_Request(TSocket *Client, int id); //Request wurde akzeptiert
	void Decline_Request(TSocket *Client, int id); //Request wurde abgelehnt
	void Send_Hello(TSocket *Client); //Spieler geht offline, Freunden mitteilen
	void Send_Bye(TSocket *Client); //Spieler kommt online, Freunden mitteilen
	bool Friend_Check(TSocket *Client, int id); //Schauen ob ID Freund ist
	bool Request_Check(TSocket *Client, int id); //Schauen ob Request existiert
	void ParseFriends(TSocket *Client, int characterslot, std::string friendsarray); //Datenbank Freunde parsen
	void FriendMessage(TSocket *Client, int friendid, std::vector<std::string> message); //Freunden Nachricht schicken
	void DeleteFriend(TSocket *Client, int friendid); //Freund löschen
};

#endif _H_FRIEND
