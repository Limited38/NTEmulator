#pragma once

#ifndef _H_GROUP
#define _H_GROUP

#define ACTION_GP_INVITE 0
#define ACTION_GP_INVITE_EX 1
#define ACTION_GP_ACCEPT 3
#define ACTION_GP_DECLINE 4

//Typedefintionen
class TGroup;
extern TGroup Group;
//----------------------------

//Strukturen
struct structGroupMembers
{
	TSocket *client;
};

struct structGroups
{
	int groupid;
	int leadercharid;
	std::vector<structGroupMembers> members;
};
//-----------------------------

class TGroup
{
public:
	TGroup(void); //Konstruktor
	~TGroup(void); //Destruktor
	void pjoin_handle(TSocket *Client, int action, int partnerid); // Allg. Handle
	void Send_Invite(TSocket *Client, int partnerid); // Char einladen
	void Invite_Decline(TSocket *Client, int partnerid); //Invite ablehnen
	void Invite_Accept(TSocket *Client, int partnerid); //Invite akzeptieren
	bool Request_Check(TSocket *Client, int partnerid); //Schauen ob Request besteht
	bool Requests_Delete(TSocket *Client, TSocket *Partner); //Request löschen
	void Send_Group(TSocket *Client); //Gruppenliste an den Char schicken
	void Leave_Group(TSocket *Client); //Gruppe verlassen
	bool Leader_Check(TSocket *Client); //Schauen ob Leader
	int Get_Groupsize(int groupid); //Anzahl Members
	int Group_Create(int leaderid); //Gruppe erstellen
	int Group_GetByID(int groupid);

	std::vector<structGroups> groups;
};

#endif _H_GROUP
